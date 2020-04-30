#include "covid19source.h"
#include "data/covidLoader.h"
#include "ofxTime.h"

#include <algorithm>
#include <cmath>

using namespace  std;

const string DATA_FILENAME = "covid_data.csv";
const string SAMPLE_DATA_FILENAME = "covid_sample_data.csv";
const string DATE_FORMAT = "%Y-%m-%d";

namespace {
float interpolate(float pct) { return ofMap(pct, 0, 1, 10, 200); }
} // namespace

void Covid19::setup() {
	font.load("Montserrat-Medium.ttf", 16);
	colorSource = new PaletteSource("palettes/MonteCarlo.jpg");

	screenSize = ofGetWindowSize();
	particlePayload.speed = &speed;
	particlePayload.screenSize = &screenSize;
	particlePayload.colorPalette = colorSource;

	particle = new Particle(particlePayload);

	name = "Covid19";
	allocate(768, 1024);
	reset();

	ofLog() << "Loading the data";
	loadCovidCsv();
	sortDataByDate();
	ofLog() << "Loaded " << covidData.data.size() << " entries";
	ofLog() << "Date Range: " <<
		Poco::DateTimeFormatter::format(covidData.dateRange.first, DATE_FORMAT) << " to " <<
		Poco::DateTimeFormatter::format(covidData.dateRange.second, DATE_FORMAT);
	ofLog() << "Total Dimensions: " << covidData.buckets.size();
	for(auto entry : covidData.buckets) {
		ofLog() << entry;
	}
	size = 0.f;
}

void Covid19::reset() {
	particle->randomize();
	ofClear(0.);
	resetTime = ofGetElapsedTimef();
}

void Covid19::update() {

	if (ofGetElapsedTimef() - resetTime > 120.f) {
		reset();
	}

	index = (int)ceil(ofGetFrameNum() * 10.) % covidData.data.size();
	if (index != lastIndex) {
		int growth = covidData.data[index].difference;
		// size = ofMap(growth, -20000, 20000, 10, 200);
		size += growth;
		scaledSize = interpolate(ofNormalize(size, 0, 100000));

		// ofLog(OF_LOG_NOTICE)
		// 	<< growth << " -> " << size << " -> " << scaledSize;
	}
  // TODO: move scaled size to particle
	particle->scaledSize = scaledSize;
	particle->update();

	if (index < lastIndex) {
		ofExit();
	}
	lastIndex = index;

	// TODO: Move this to particle
	glm::ivec2 offset((int)(ofRandom(50) - 25));
	colorLocation += offset;
	colorLocation.x = ofWrap(colorLocation.x, 0, colorSource->numColors().x);
	colorLocation.y = ofWrap(colorLocation.y, 0, colorSource->numColors().y);
	particle->color = colorSource->getColorAt(colorLocation);
}

void Covid19::draw() {
	if (clearScreen) {
		ofSetColor(255.);
		ofDrawRectangle(0., 0., fbo->getWidth(), fbo->getHeight());
		ofSetColor(0.);
		ofDrawRectangle(5., 5., fbo->getWidth() - 10., fbo->getHeight() - 10.);
	}

	particle->draw();

	auto formattedDate =
		Poco::DateTimeFormatter::format(covidData.data[index].date, DATE_FORMAT);

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
 // NOP
}

void Covid19::onSpeedChange(float &f) { speed = f; }

void Covid19::onDrawChange(bool &b) { clearScreen = !b; }