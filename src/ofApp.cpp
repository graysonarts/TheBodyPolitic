#include "ofApp.h"
#include "covidLoader.h"

void ofDisplayApp::setup() {
	ofxGuiEnableHiResDisplay();
	showGui = false;
	speed = speed.set("speed", 1., 1., 10.);
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