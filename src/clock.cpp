#include "clock.h"

#include <cmath>

void Clock::setup(Poco::Timestamp startDate, Poco::Timestamp endDate) {
	this->startDate = startDate;
	this->endDate = endDate;
	numDays = ceil((endDate - startDate)/1000./1000./60./60./24.); // Microseconds;
	index = 0;
	speed = 1000.f;
	time = 0.f;
}

bool Clock::update() {
	int lastIndex = index;
	time = ofGetElapsedTimeMillis();
	index = ceil(fmod(time / speed, numDays));
	transitionPercentage = fmod(time, speed) / speed;

	return lastIndex > index;
}

Poco::Timestamp Clock::currentDate() const {
	return startDate + Poco::Timespan(index, 0, 0, 0, 0);
}