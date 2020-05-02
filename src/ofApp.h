#pragma once

#include "covid19source.h"
#include "ofMain.h"
#include "ofxGui.h"
#include "ofxPiMapper.h"
#include "ofxMidi.h"
#include <string>

class ofDisplayApp : public ofBaseApp, public ofxMidiListener {
public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void newMidiMessage(ofxMidiMessage& eventArgs);
	void exit();

private:
	ofxPiMapper piMapper;
	ofxPanel gui;
	bool showGui;
	ofParameter<float> speed;
	ofParameter<float> tempo;
	ofParameter<bool> drawLine;
	ofParameter<bool> clearScreen;
	ofxMidiIn midiIn;

	Covid19 covid19;

	void onControlChange(int channel, int value);
};
