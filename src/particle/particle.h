#pragma once

#include "ofMain.h"
#include "../palette/palette.h"
#include "../clock.h"

struct ParentReference {
	float *speed;
	PaletteSource *colorPalette;
	glm::vec2 *screenSize;
};

class Particle {
	public:
		glm::vec2 location;
		glm::vec2 velocity;
		ofColor color;
		glm::vec2 colorLocation;
		const ParentReference& parent;
		float scaledSize = 1.; // TODO: Update Size

		Particle(const ParentReference &parent)
			: location({0., 0.}), velocity({1., 1.}), colorLocation({1., 1.}), parent(parent)
		{ updateColor(); }

		void setup() {
			randomize();
		}

		void update(float size) {
			scaledSize = size;

			location += velocity * (*parent.speed);

			if (location.x <= scaledSize || location.x >= parent.screenSize->x - scaledSize) {
				velocity.x *= -1;
			}

			if (location.y <= scaledSize || location.y >= parent.screenSize->y - scaledSize) {
				velocity.y *= -1;
			}
		}

		void draw() const {
			ofSetColor(color);
			ofDrawCircle(location.x, location.y, scaledSize);
		}

		void randomize() {
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