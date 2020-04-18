#include "ofApp.h"

//--------------------------------------------------------------
void ofControlApp::setup(){
	ofxGuiEnableHiResDisplay();
	gui.setup("panel");
	gui.add(fullScreen.set("full screen", false));
	gui.add(cameraDistance.set("camera distance", 0., 0., 100.));
	gui.add(box.set("box size", glm::vec3(10.0), glm::vec3(1.0), glm::vec3(100.0)));


	gui.add(attConstant.set("constant", 1.));
	gui.add(attLinear.set("linear", 0.));
	gui.add(attQuadratic.set("quadratic", 0.));

	fullScreen.addListener(this, &ofControlApp::fullscreenToggled);
	gui.loadFromFile("settings.xml");
}

//--------------------------------------------------------------
void ofControlApp::update(){

}

//--------------------------------------------------------------
void ofControlApp::draw(){
	ofBackground(ofColor::bisque);
	gui.draw();
}

void ofControlApp::keyPressed(int key) {
	float f = 0.;
	if(key == 's') {
		gui.saveToFile("settings.xml");
		ofNotifyEvent(onSaveMesh, f);
	}
	if(key == 'l') {
		gui.loadFromFile("settings.xml");
		ofNotifyEvent(onLoadMesh, f);
	}
}

void ofControlApp::fullscreenToggled(bool & e) {
	ofSetFullscreen(e);
}

void ofDisplayApp::setup() {
	controls->fullScreen.addListener(this, &ofDisplayApp::fullscreenToggled);
	ofAddListener(controls->onSaveMesh, this, &ofDisplayApp::onSaveMesh);
	ofAddListener(controls->onLoadMesh, this, &ofDisplayApp::onLoadMesh);

	piMapper.setup();
}

void ofDisplayApp::update() {
	piMapper.update();
}

void ofDisplayApp::draw() {
	ofBackgroundGradient(controls->bg1, controls->bg2);
	piMapper.draw();
}

void ofDisplayApp::fullscreenToggled(bool & e) {
	ofSetFullscreen(e);
}

bool ofDisplayApp::onSaveMesh(float &f) {

	return true;

}

bool ofDisplayApp::onLoadMesh(float &f) {

	return true;
}

void ofDisplayApp::keyPressed(int key){
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