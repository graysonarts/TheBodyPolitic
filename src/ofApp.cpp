#include "ofApp.h"
#include "covidLoader.h"

void ofDisplayApp::setup() {
	ofxGuiEnableHiResDisplay();
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
	gui.setPosition(1000.0f, 0.f);
	gui.add(speed);
	gui.add(drawLine);
	gui.add(tempo);
	gui.add(clearScreen.set("Clear Screen", false));

	speed.addListener(&covid19, &Covid19::onSpeedChange);
	drawLine.addListener(&covid19, &Covid19::onDrawChange);
	tempo.addListener(&covid19, &Covid19::onTempoChange);
	clearScreen.addListener(&covid19, &Covid19::onClearChange);

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
		case 14: // Knob 1
			tempo = ofMap(value, 0, 127, 250., 5000.);
			break;
		case 44: // Record Button
			showGui = value > 64;
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