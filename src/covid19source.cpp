#include "covid19source.h"
#include "data/covidLoader.h"
#include "ofxTime.h"

#include <algorithm>
#include <cmath>

using namespace std;

const string DATA_FILENAME = "covid_data.csv";
const string SAMPLE_DATA_FILENAME = "covid_sample_data.csv";
const string DATE_FORMAT = "%Y-%m-%d";
const float MAX_CASE_SCALE = 500000.f;

void Covid19::setup() {
	font.load("Montserrat-Medium.ttf", 16);

	screenSize = ofGetWindowSize();
	particlePayload.speed = &speed;
	particlePayload.screenSize = &screenSize;
	// TODO: Build list of palettes to rotate.
	particlePayload.colorPalette = new PaletteSource("palettes/MonteCarlo.jpg");

	name = "Covid19";
	allocate(768, 1024);

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
		particles.emplace(entry, new Particle(particlePayload));
	}
	size = 0.f;

	clock.setup(covidData.dateRange.first, covidData.dateRange.second);
	clock.speed = 250.;
	reset();
}

void Covid19::reset() {
	for (auto p : particles) {
		p.second->randomize();
	}

	// ofClear(0.);
	resetTime = ofGetElapsedTimef();
}

void Covid19::update() {
	if (clock.update()) {
		ofClear(0.);
		size = 0.;
		// ofExit();
	}

	if (ofGetElapsedTimef() - resetTime > 120.f) {
		reset();
	}

	index = clock.index;
	if (index != lastIndex) {
		scaledSize = ofMap(size, 0, MAX_CASE_SCALE, 3, 50);

		int32_t growth = covidData.getDataFor(clock.currentDate(), "US");
		size += growth;
		float nextScaledSize = ofMap(size, 0, MAX_CASE_SCALE, 3, 50);
		scaledStep = nextScaledSize - scaledSize;
	}

	for(auto particle : particles) {
		particle.second->update(clock, scaledSize + scaledStep * clock.transitionPercentage);
	}
	lastIndex = index;
}

void Covid19::draw() {
	if (clearScreen) {
		ofSetColor(255.);
		ofDrawRectangle(0., 0., fbo->getWidth(), fbo->getHeight());
		ofSetColor(0.);
		ofDrawRectangle(5., 5., fbo->getWidth() - 10., fbo->getHeight() - 10.);
	} else {
		ofSetColor(0.,5);
		ofDrawRectangle(0., 0., fbo->getWidth(), fbo->getHeight());
	}

	for (auto& particle : particles) {
		particle.second->draw();
	}

	auto formattedDate =
		Poco::DateTimeFormatter::format(clock.currentDate(), DATE_FORMAT);

	ofRectangle textField = font.getStringBoundingBox(
		formattedDate, fbo->getWidth() / 2.f, fbo->getHeight() / 2.f);
	textField.scaleFromCenter(2.0f);
	ofSetColor(0.);
	ofDrawRectangle(textField.x, textField.y, textField.width, textField.height*3 + 10.);
	ofSetColor(255.);
	font.drawString(formattedDate, fbo->getWidth() / 2.f,
					fbo->getHeight() / 2.f);
	// font.drawString(to_string(particle->scaledSize), fbo->getWidth() / 2.f, fbo->getHeight() / 2.f + textField.height + 5.f);
	font.drawString(to_string(size), fbo->getWidth() / 2.f, fbo->getHeight() / 2.f + textField.height*2 + 10.f);
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