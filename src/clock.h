#pragma once

#include "ofMain.h"
#include "ofxTime.h"

#include <cmath>

class Clock {
	public:
		void setup(Poco::Timestamp startDate, Poco::Timestamp endDate);
		bool update();
		Poco::Timestamp currentDate() const;

	Poco::Timestamp startDate, endDate;
	int index, numDays;
	float transitionPercentage, speed;

	private:
		float time;
};