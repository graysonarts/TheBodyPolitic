#include "covid19source.h"
#include "covidLoader.h"
#include "ofxTime.h"

#include <algorithm>
#include <cmath>

const std::string DATA_FILENAME = "covid_data.csv";
const std::string SAMPLE_DATA_FILENAME = "covid_sample_data.csv";

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
	index = (int)ceil(ofGetElapsedTimef()) % covidData.size();
	int growth = covidData[index].difference;
	size = ofMap(growth, -20000, 20000, 10, 200);
	bool bounced = false;

	if (location.x <= 0 || location.x >= fbo->getWidth() - size) {
		velocity.x *= -1;
		bounced = true;
	}

	if (location.y <= 0 || location.y >= fbo->getHeight() - size) {
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
	ofDrawCircle(location.x, location.y, size);

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