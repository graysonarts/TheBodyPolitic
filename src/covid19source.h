#pragma once

#include "ofMain.h"
#include "FboSource.h"

class Covid19 : public ofx::piMapper::FboSource {
	public:
		void setup();
		void update();
		void draw();

	public:
		void onSpeedChange(float &s);
		void onDrawChange(bool &b);

	private:
		glm::vec2 location;
		glm::vec2 velocity;
		ofColor color;
		float speed = 1.0f;
		bool clearScreen = true;
};