#include "covid19source.h"

void Covid19::setup() {
	colorSource = new PaletteSource("palettes/MonteCarlo.jpg");
	name = "Covid19";
	allocate(768, 1024);
	reset();
	color = colorSource->getColorAt(colorLocation);
}

void Covid19::reset() {
	location.x = ofRandom(fbo->getWidth());
	location.y = ofRandom(fbo->getHeight());
	velocity.x = ofRandom(4.0)-2.0;
	velocity.y = ofRandom(4.0)-2.0;
	ofClear(0.);
	resetTime = ofGetElapsedTimef();
}

void Covid19::update() {

	if (ofGetElapsedTimef() - resetTime > 120.f) {
		reset();
	}

	location += velocity * speed;
	bool bounced = false;

	if (location.x <= 0 || location.x >= fbo->getWidth()-20.0) {
		velocity.x *= -1;
		bounced = true;
	}

	if (location.y <=0 || location.y >= fbo->getHeight()-20.0) {
		velocity.y *= -1;
		bounced = true;
	}

	if (bounced) {
		glm::ivec2 offset((int)ofRandom(50));
		colorLocation = (colorLocation + offset) % colorSource->numColors();
		color = colorSource->getColorAt(colorLocation);
	}
}

void Covid19::draw() {
	if (clearScreen) {
		ofSetColor(255.);
		ofDrawRectangle(0., 0., fbo->getWidth(), fbo->getHeight());
		ofSetColor(0.);
		ofDrawRectangle(5., 5., fbo->getWidth()-10., fbo->getHeight()-10.);
	}

	ofSetColor(color);
	ofDrawCircle(location.x, location.y, 20.0);

}

void Covid19::onSpeedChange(float &s) {
	speed = s;
}

void Covid19::onDrawChange(bool &b) {
	clearScreen = !b;
}