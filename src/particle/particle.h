#pragma once

#include "ofMain.h"
#include "../palette/palette.h"
#include "../data/covidLoader.h"
#include "../clock.h"

const float MAX_CASE_SCALE = 100000.f;
const float MAX_SIZE = 50.f;
const float MIN_SIZE = 1.f;

struct ParentReference {
	float *speed;
	PaletteSource *colorPalette;
	glm::vec2 *screenSize;
	LoadedCovidData *data;
};

class Particle {
	public:
		glm::vec2 location;
		glm::vec2 velocity;
		ofColor color;
		glm::vec2 colorLocation;
		const ParentReference& parent;
		float scaledSize = 1.; // TODO: Update Size
		float scaledStep = 0.;
		float currentSize = 0.;
		float size = 0.;
		bool dayWasZero = false;
		BucketKey key;

		Particle(const ParentReference &parent, const BucketKey &key)
			: location({0., 0.}), velocity({1., 1.}), colorLocation({1., 1.}), parent(parent), key(key)
		{ updateColor(); }

		void setup() {
			randomize();
		}

		void update(const Clock &clock, bool transitioned) {
			if (transitioned)
				calculateSize(clock);

			currentSize = scaledSize + scaledStep * clock.transitionPercentage;

			// ofLog(OF_LOG_NOTICE) << "tP" << clock.transitionPercentage << " cS" << currentSize;

			location += velocity * (*parent.speed);

			if (location.x <= currentSize || location.x >= parent.screenSize->x - currentSize) {
				velocity.x *= -1;
			}

			if (location.y <= currentSize || location.y >= parent.screenSize->y - currentSize) {
				velocity.y *= -1;
			}

			location.x = ofClamp(location.x, currentSize, parent.screenSize->x - currentSize);
			location.y = ofClamp(location.y, currentSize, parent.screenSize->y - currentSize);

			cycleColor();
		}

		void calculateSize(const Clock &clock) {
			float oldScaledSize = scaledSize + scaledStep * clock.transitionPercentage;;
			scaledSize = ofMap(size, 0, MAX_CASE_SCALE, MIN_SIZE, MAX_SIZE);
			// ofLog(OF_LOG_NOTICE) << "CS: " << oldScaledSize << " == " << scaledSize;

			int32_t growth = parent.data->getDataFor(clock.currentDate(), key);
			dayWasZero = growth == 0;
			size += growth;
			float nextScaledSize = ofMap(size, 0, MAX_CASE_SCALE, MIN_SIZE, MAX_SIZE);
			scaledStep = nextScaledSize - scaledSize;
		}

		void cycleColor() {
				glm::ivec2 offset((int)(ofRandom(50) - 25));
				colorLocation += offset;
				colorLocation.x = ofWrap(colorLocation.x, 0, parent.colorPalette->numColors().x);
				colorLocation.y = ofWrap(colorLocation.y, 0, parent.colorPalette->numColors().y);
				updateColor();
		}

		void draw() const {
			ofSetColor(color, 200.f);
			ofDrawCircle(location.x, location.y, currentSize);
			if (dayWasZero) {
				ofSetColor(ofColor::red);
				ofRectangle(location.x, location.y, 25, 25);
			}
		}

		void randomize() {
			// location.x = parent.screenSize->x /2 - 25;
			// location.y =parent.screenSize->y / 2 - 25;
			// velocity = {0,0};
			location.x = ofRandom(parent.screenSize->x);
			location.y = ofRandom(parent.screenSize->y);
			velocity = {ofRandom(10.)-5., ofRandom(10.)-5. };
			glm::vec2 numColors = parent.colorPalette->numColors();
			colorLocation = {ofRandom(numColors.x), ofRandom(numColors.y)};
			updateColor();
		}

	private:
		void updateColor() {
			color = parent.colorPalette->getColorAt(colorLocation);
		}
};