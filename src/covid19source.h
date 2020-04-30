#pragma once

#include "FboSource.h"
#include "covidLoader.h"
#include "ofMain.h"
#include "palette/palette.h"
#include "particle/particle.h"

class Covid19 : public ofx::piMapper::FboSource {
public:
	void setup();
	void update();
	void draw();
	void reset();

public:
	void onSpeedChange(float &s);
	void onDrawChange(bool &b);

private:
	glm::vec2 screenSize;
	glm::ivec2 colorLocation;
	float speed = 1.0f;
	float resetTime;
	float size = 20.0f;
	float scaledSize = 0.0f;
	bool clearScreen = true;
	PaletteSource *colorSource;
	ofTrueTypeFont font;

	void loadCovidCsv();
	void sortDataByDate();
	LoadedCovidData covidData;
	int index, lastIndex;

	ParentReference particlePayload;
	Particle *particle;
};