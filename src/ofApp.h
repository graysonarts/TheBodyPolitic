#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxPiMapper.h"
#include "covid19source.h"

class ofDisplayApp : public ofBaseApp {
	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);

	private:
		ofxPiMapper piMapper;
		ofxPanel gui;
		bool showGui;
		ofParameter<float> speed;
		ofParameter<bool> drawLine;

		Covid19 covid19;
};
