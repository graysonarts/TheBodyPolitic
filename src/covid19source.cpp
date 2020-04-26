#include "covid19source.h"
#include "data/covidLoader.h"
#include "ofxTime.h"

#include <algorithm>
#include <cmath>

const std::string DATA_FILENAME = "covid_data.csv";
const std::string SAMPLE_DATA_FILENAME = "covid_sample_data.csv";

namespace {
float interpolate(float pct) { return ofMap(pct, 0, 1, 10, 200); }
} // namespace

void Covid19::setup() {
	font.load("Montserrat-Medium.ttf", 16);
	colorSource = new PaletteSource("palettes/MonteCarlo.jpg");
	name = "Covid19";
	allocate(768, 1024);
	reset();
	color = colorSource->getColorAt(colorLocation);

	ofLog() << "Loading the data";
	loadCovidCsv();
	sortDataByDate();
	ofLog() << "Loaded " << covidData.size() << " entries";

	ofLog() << "Loaded country . " << covidData[0].countryRegion;
	ofLog() << "Loaded country . " << covidData[1].countryRegion;
	size = 0.f;
}

void Covid19::reset() {
	location.x = ofRandom(fbo->getWidth());
	location.y = ofRandom(fbo->getHeight());
	velocity.x = ofRandom(4.0) - 2.0;
	velocity.y = ofRandom(4.0) - 2.0;
	ofClear(0.);
	resetTime = ofGetElapsedTimef();
}

void Covid19::update() {

	if (ofGetElapsedTimef() - resetTime > 120.f) {
		reset();
	}

	location += velocity * speed;
	index = (int)ceil(ofGetFrameNum()) % covidData.size();
	if (index != lastIndex) {
		int growth = covidData[index].difference;
		// size = ofMap(growth, -20000, 20000, 10, 200);
		size += growth;
		scaledSize = interpolate(ofNormalize(size, 0, 100000));

		ofLog(OF_LOG_NOTICE)
			<< growth << " -> " << size << " -> " << scaledSize;
	}

	if (index < lastIndex) {
		ofExit();
	}
	lastIndex = index;
	bool bounced = false;

	if (location.x <= scaledSize || location.x >= fbo->getWidth() - scaledSize) {
		velocity.x *= -1;
		bounced = true;
	}

	if (location.y <= scaledSize || location.y >= fbo->getHeight() - scaledSize) {
		velocity.y *= -1;
		bounced = true;
	}

	glm::ivec2 offset((int)(ofRandom(50) - 25));
	colorLocation += offset;
	colorLocation.x = ofWrap(colorLocation.x, 0, colorSource->numColors().x);
	colorLocation.y = ofWrap(colorLocation.y, 0, colorSource->numColors().y);
	color = colorSource->getColorAt(colorLocation);
}

void Covid19::draw() {
	if (clearScreen) {
		ofSetColor(255.);
		ofDrawRectangle(0., 0., fbo->getWidth(), fbo->getHeight());
		ofSetColor(0.);
		ofDrawRectangle(5., 5., fbo->getWidth() - 10., fbo->getHeight() - 10.);
	}

	ofSetColor(color);
	ofDrawCircle(location.x, location.y, scaledSize);

	auto formattedDate =
		Poco::DateTimeFormatter::format(covidData[index].date, "%Y-%d-%m");

	ofRectangle textField = font.getStringBoundingBox(
		formattedDate, fbo->getWidth() / 2.f, fbo->getHeight() / 2.f);
	textField.scaleFromCenter(2.0f);
	ofSetColor(0.);
	ofDrawRectangle(textField);
	ofSetColor(255.);
	font.drawString(formattedDate, fbo->getWidth() / 2.f,
					fbo->getHeight() / 2.f);
}

void Covid19::loadCovidCsv() {
	if (ofFile::doesFileExist(DATA_FILENAME)) {
		covidData = loadCovidData(DATA_FILENAME);
	} else {
		covidData = loadCovidData(SAMPLE_DATA_FILENAME);
	}
}

void Covid19::sortDataByDate() {
	std::sort(covidData.begin(), covidData.end());
}

void Covid19::onSpeedChange(float &f) { speed = f; }

void Covid19::onDrawChange(bool &b) { clearScreen = !b; }