#pragma once

#include "ofMain.h"

class PaletteSource {
	public:
		PaletteSource(string path);
		bool switchSource(string path);
		ofColor getColorAt(glm::ivec2 position) const;
		glm::ivec2 numColors() const;

	private:
		ofImage *source;

};