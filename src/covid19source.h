#pragma once

#include "FboSource.h"
#include "covidLoader.h"
#include "ofMain.h"
#include "clock.h"
#include "palette/palette.h"
#include "particle/particle.h"

#include <map>

class Covid19 : public ofx::piMapper::FboSource {
public:
	void setup();
	void update();
	void draw();
	void reset();
	void onSpeedChange(float &s);
	void onDrawChange(bool &b);

	ofTrueTypeFont font;

private:
	glm::vec2 screenSize;
	glm::ivec2 colorLocation;
	float speed = 1.0f;
	float resetTime;
	float size = 20.0f;
	float scaledSize = 0.0f;
	float scaledStep = 0.0f;
	bool clearScreen = true;

	void loadCovidCsv();
	void sortDataByDate();
	LoadedCovidData covidData;
	int index, lastIndex;
	Clock clock;

	ParentReference particlePayload;

	std::map<BucketKey, Particle*> particles;
};