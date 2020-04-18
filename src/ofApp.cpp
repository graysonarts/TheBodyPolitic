#include "ofApp.h"

void ofDisplayApp::setup() {
	ofxGuiEnableHiResDisplay();
	showGui = false;
	gui.setup("panel");
	gui.setPosition(1000.0f, 0.f);

	piMapper.setup();
}

void ofDisplayApp::update() {
	piMapper.update();
}

void ofDisplayApp::draw() {
	ofBackground(0.0f);
	piMapper.draw();
	if (showGui) gui.draw();
}

void ofDisplayApp::keyPressed(int key){
	if (key == 'i') {
		showGui = !showGui;
	} if (key == 'f') {
		ofToggleFullscreen();
	}
	piMapper.keyPressed(key);
}

void ofDisplayApp::keyReleased(int key){
	piMapper.keyReleased(key);
}

void ofDisplayApp::mousePressed(int x, int y, int button){
	piMapper.mousePressed(x, y, button);
}

void ofDisplayApp::mouseReleased(int x, int y, int button){
	piMapper.mouseReleased(x, y, button);
}

void ofDisplayApp::mouseDragged(int x, int y, int button){
	piMapper.mouseDragged(x, y, button);
}