#include "covid19source.h"
#include "covidLoader.h"

#include <cmath>

void Covid19::setup() {
	font.load("Montserrat-Medium.ttf", 16);
	colorSource = new PaletteSource("palettes/MonteCarlo.jpg");
	name = "Covid19";
	allocate(768, 1024);
	reset();
	color = colorSource->getColorAt(colorLocation);

	ofLog() << "Loading the data";
	loadCovidCsv();
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

	glm::ivec2 offset((int)(ofRandom(50)-25));
	colorLocation = (colorLocation + offset) % colorSource->numColors();
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

	ofSetColor(0.);
	ofDrawRectangle(font.getStringBoundingBox(covidData[index].date, fbo->getWidth() / 2.f, fbo->getHeight() / 2.f));
	ofSetColor(255.);
	font.drawString(covidData[index].date, fbo->getWidth() / 2.f, fbo->getHeight() / 2.f);
}

void Covid19::loadCovidCsv() { covidData = loadCovidData(); }

void Covid19::onSpeedChange(float &s) { speed = s; }

void Covid19::onDrawChange(bool &b) { clearScreen = !b; }