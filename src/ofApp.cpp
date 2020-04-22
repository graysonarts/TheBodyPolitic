#include "ofApp.h"
#include "covidData.h"
#include "covidLoader.h"

const std::string DATA_FILENAME = "covid_data.csv";
const std::string SAMPLE_DATA_FILENAME = "covid_sample_data.csv";

void ofDisplayApp::setup() {
	ofxGuiEnableHiResDisplay();
	showGui = false;
	speed = speed.set("speed", 5., 1., 10.);
	drawLine = drawLine.set("clear screen per frame", false);

	gui.setup("panel");
	gui.setPosition(1000.0f, 0.f);
	gui.add(speed);
	gui.add(drawLine);
	gui.add(velx);
	gui.add(vely);

	speed.addListener(&covid19, &Covid19::onSpeedChange);
	drawLine.addListener(&covid19, &Covid19::onDrawChange);

	piMapper.registerFboSource(covid19);
	piMapper.setup();

	loadCovidCsv();

	ofLog() << "Loaded country . " << covidData[0].countryRegion;
	ofLog() << "Loaded country . " << covidData[1].countryRegion;
}

void ofDisplayApp::loadCovidCsv() {
	if (ofFile::doesFileExist(ofToDataPath(DATA_FILENAME))) {
		covidData = loadCovidData(DATA_FILENAME);
	} else {
		ofLog(OF_LOG_WARNING) << "Unable to find " << DATA_FILENAME << ", loading " << SAMPLE_DATA_FILENAME;
		covidData = loadCovidData(SAMPLE_DATA_FILENAME);
	}
}

void ofDisplayApp::update() { piMapper.update(); }

void ofDisplayApp::draw() {
	ofBackground(0.0f);
	piMapper.draw();
	if (showGui)
		gui.draw();
}

void ofDisplayApp::keyPressed(int key) {
	if (key == 'i') {
		showGui = !showGui;
	}
	if (key == 'f') {
		ofToggleFullscreen();
	}
	if (key == ' ') {
		drawLine.set(!drawLine.get());
	}
	piMapper.keyPressed(key);
}

void ofDisplayApp::keyReleased(int key) { piMapper.keyReleased(key); }

void ofDisplayApp::mousePressed(int x, int y, int button) {
	piMapper.mousePressed(x, y, button);
}

void ofDisplayApp::mouseReleased(int x, int y, int button) {
	piMapper.mouseReleased(x, y, button);
}

void ofDisplayApp::mouseDragged(int x, int y, int button) {
	piMapper.mouseDragged(x, y, button);
}