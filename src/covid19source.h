#pragma once

#include "FboSource.h"
#include "ofMain.h"
#include "covidData.h"
#include "palette/palette.h"


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
	glm::vec2 location;
	glm::vec2 velocity;
	glm::ivec2 colorLocation;
	ofColor color;
	float speed = 1.0f;
	float resetTime;
	bool clearScreen = true;
	PaletteSource *colorSource;

	void loadCovidCsv();
	std::vector<CovidData> covidData;
};