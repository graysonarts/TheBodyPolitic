#include "palette.h"

PaletteSource::PaletteSource(string path) : source(nullptr) {
	switchSource(path);
}

bool PaletteSource::switchSource(string path) {
	if (source != nullptr) {
		delete source;
		source = nullptr;
	}

	source = new ofImage(path);
	bool loadedSuccessfully = source->isAllocated();
	if (!loadedSuccessfully) {
		delete source;
		source = nullptr;
	}

	return loadedSuccessfully;
}

ofColor PaletteSource::getColorAt(const glm::ivec2 position) const {
	if (source == nullptr) {
		return ofColor::chartreuse;
	}

	return ofColor(source->getColor(position.x, position.y), 128.f);
}

glm::ivec2 PaletteSource::numColors() const {
	if (source == nullptr) {
		return {0., 0.};
	}

	return {(int)source->getWidth(), (int)source->getHeight()};
}