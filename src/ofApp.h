#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxPiMapper.h"

class ofControlApp : public ofBaseApp{

	public:
		ofControlApp() : ofBaseApp(), bg1(ofColor::red), bg2(ofColor::blue) { }
		void setup();
		void update();
		void draw();

		void fullscreenToggled(bool & e);

		void keyPressed(int key);
		// void keyReleased(int key);
		// void mouseMoved(int x, int y );
		// void mouseDragged(int x, int y, int button);
		// void mousePressed(int x, int y, int button);
		// void mouseReleased(int x, int y, int button);
		// void mouseEntered(int x, int y);
		// void mouseExited(int x, int y);
		// void windowResized(int w, int h);
		// void dragEvent(ofDragInfo dragInfo);
		// void gotMessage(ofMessage msg);

		ofParameter<ofColor> bg1, bg2;
		ofParameter<float> cameraDistance;
		ofParameter<glm::vec3> box;
		ofParameter<bool> fullScreen;

		ofParameter<float> attConstant;
		ofParameter<float> attLinear;
		ofParameter<float> attQuadratic;

		ofEvent<float> onSaveMesh;
		ofEvent<float> onLoadMesh;

	private:
		ofxPanel gui;
};

class ofDisplayApp : public ofBaseApp {
	public:
		ofDisplayApp(std::shared_ptr<ofControlApp> controls) : ofBaseApp(), controls(controls) { }
		void setup();
		void update();
		void draw();

		void fullscreenToggled(bool & e);

		bool onSaveMesh(float &f);
		bool onLoadMesh(float &f);

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);

	private:
		std::shared_ptr<ofControlApp> controls;
		ofxPiMapper piMapper;
};
