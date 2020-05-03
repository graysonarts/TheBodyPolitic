#include "ofApp.h"
#include "covidLoader.h"

const string PARAM_FILE = "parameters.xml";

void ofDisplayApp::setup() {
	// ofxGuiEnableHiResDisplay();
	ofEnableSmoothing();
	ofSetVerticalSync(true);

	midiIn.listInPorts();
	midiIn.openPort(1);
	midiIn.ignoreTypes(false, false, false); // Receive all messages for now
	midiIn.addListener(this);

	showGui = false;
	speed = speed.set("speed", 1., 1., 10.);
	drawLine = drawLine.set("clear screen per frame", false);
	tempo = tempo.set("tempo", 250., 100., 10000.);

	gui.setup("panel");
	gui.setPosition(ofGetWidth()/2.f, ofGetHeight()/2.f);
	gui.add(speed);
	gui.add(drawLine);
	gui.add(tempo);
	gui.add(clearScreen.set("Clear Screen", false));
	gui.add(offsetX.set("label offset x", 0, 0, ofGetWidth() / 2.f));
	gui.add(offsetY.set("label offset y", 0, 0, ofGetHeight() / 2.f));
	gui.add(labelRotation.set("label rotation", 0, 0, 360));

	speed.addListener(&covid19, &Covid19::onSpeedChange);
	drawLine.addListener(&covid19, &Covid19::onDrawChange);
	tempo.addListener(&covid19, &Covid19::onTempoChange);
	clearScreen.addListener(&covid19, &Covid19::onClearChange);
	offsetX.addListener(&covid19, &Covid19::onXOffsetChange);
	offsetY.addListener(&covid19, &Covid19::onYOffsetChange);
	labelRotation.addListener(&covid19, &Covid19::onLabelRotationChange);

	gui.loadFromFile(PARAM_FILE);

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

void ofDisplayApp::newMidiMessage(ofxMidiMessage& eventArgs) {
	if (eventArgs.channel != 1) return;

	switch(eventArgs.status) {
		case MIDI_CONTROL_CHANGE:
			onControlChange(eventArgs.control, eventArgs.value);
			break;
		default:
			ofLog(OF_LOG_NOTICE) << ofxMidiMessage::getStatusString(eventArgs.status);
	}

}

void ofDisplayApp::onControlChange(int control, int value) {
	switch (control) {
		case 2: // Slider 1
			speed = ofMap(value, 0, 127, 1, 10);
			break;
		case 3: // Slider 2
			offsetX = ofMap(value, 0, 127, 0, ofGetWidth() / 2.f);
			break;
		case 4:
			offsetY = ofMap(value, 0, 127, 0, ofGetHeight() / 2.f);
			break;
		case 14: // Knob 1
			tempo = ofMap(value, 0, 127, 250., 5000.);
			break;
		case 15: // Knob 2
			labelRotation = ofMap(value, 0, 127, 0, 360);
			break;
		case 44: // Record Button
			gui.saveToFile(PARAM_FILE);
			break;
		case 46: // Stop Button
			showGui = value > 64;
			break;
		case 48: // Next Button (next palette)
			if (value > 64) ofNotifyEvent(covid19.onNextPalette);
			break;
		case 49: // Loop Button
			clearScreen = value > 64;
			break;

		default:
			ofLog(OF_LOG_NOTICE) << "control: " << control << " value: " << value;
	}
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

void ofDisplayApp::exit() {
	midiIn.closePort();
	midiIn.removeListener(this);
}