#include "covid19source.h"
#include "data/covidLoader.h"
#include "ofxTime.h"

#include <algorithm>
#include <cmath>

using namespace std;

const string DATA_FILENAME = "covid_data.csv";
const string SAMPLE_DATA_FILENAME = "covid_sample_data.csv";
const string DATE_FORMAT = "%Y-%m-%d";

// #define SINGLE

void Covid19::setup() {
	font.load("Montserrat-Medium.ttf", 16);

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

	size = 0.f;

	screenSize = ofGetWindowSize();
	particlePayload.speed = &speed;
	particlePayload.screenSize = &screenSize;
	particlePayload.data = &covidData;
	// TODO: Build list of palettes to rotate.
	enumerate_palettes();
	particlePayload.colorPalette = new PaletteSource("palettes/MonteCarlo.jpg");
	next_palette();

#ifdef SINGLE
	auto entry = "US"; {
#else
	for(auto entry : covidData.buckets) {
#endif
		ofLog() << entry;
		particles.emplace(entry, new Particle(particlePayload, entry));
	}

	clock.setup(covidData.dateRange.first, covidData.dateRange.second);
	reset();

	ofAddListener(this->onNextPalette, this, &Covid19::handleNextPaletteEvent);
	ofClear(0.);
}

void Covid19::reset() {
	for (auto p : particles) {
		p.second->randomize();
	}

	resetTime = ofGetElapsedTimef();
}

void Covid19::update() {
	resetTriggered = clock.update();
	if (resetTriggered) {
		reset();
		next_palette();

	}

	index = clock.index;

	for(auto particle : particles) {
		if (resetTriggered) {
			particle.second->size = 0.;
		}
		particle.second->update(clock, index != lastIndex);
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
		ofSetColor(0., 5.);
		ofDrawRectangle(0, 0, fbo->getWidth(), fbo->getHeight());
	}

	if (resetTriggered) {
		ofSetColor(0.,32);
		ofDrawRectangle(0., 0., fbo->getWidth(), fbo->getHeight());
	}

	if (triggerClear) {
		triggerClear = false;
		ofClear(0.);
	}

	for (auto& particle : particles) {
		particle.second->draw();
	}

	auto formattedDate =
		Poco::DateTimeFormatter::format(clock.currentDate(), DATE_FORMAT);

	ofRectangle textField = font.getStringBoundingBox(
		formattedDate, labelOffset.x, labelOffset.y);
	textField.scaleFromCenter(2.0f);
	ofPushMatrix();
		ofSetColor(0.f);
		ofTranslate(fbo->getWidth() / 2.f - textField.width / 2.f, fbo->getHeight() / 2.f - textField.height / 2.f);
		ofTranslate(textField.x, textField.y);
		ofRotateZDeg(labelRotation);

		ofDrawRectangle(-textField.width / 2.f, -textField.height / 2.f, textField.width, textField.height);
		ofSetColor(255.);
		font.drawString(formattedDate, -textField.width / 4.f, textField.height /4.f);
	ofPopMatrix();

	resetTriggered = false;
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

void Covid19::onTempoChange(float &f) { clock.speed = f; }

void Covid19::onClearChange(bool &b) { triggerClear = b; }

void Covid19::onXOffsetChange(float &f) { labelOffset.x = f; }
void Covid19::onYOffsetChange(float &f) { labelOffset.y = f; }
void Covid19::onLabelRotationChange(float &f) { labelRotation = f; }

void Covid19::handleNextPaletteEvent() {
	next_palette();
}

void Covid19::enumerate_palettes() {
	ofLog(OF_LOG_NOTICE) << "Enumerating Palettes";
	ofDirectory dir("./palettes");
	dir.allowExt("jpg");
	dir.listDir();

	for(size_t i =0; i < dir.size(); ++i) {
		string name = dir.getName(i);
		ofLog(OF_LOG_NOTICE) << "Palette: " << name;
		palettes.push_back("palettes/" + name);
	}
	selectedPalette = 0;
}

void Covid19::next_palette() {
	selectedPalette = (selectedPalette + 1) % palettes.size();
	ofLog() << "Next palette: " << palettes[selectedPalette];
	particlePayload.colorPalette->switchSource(palettes[selectedPalette]);
}