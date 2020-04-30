#pragma once

#include "covidData.h"
#include "ofxTime.h"

#include <string>
#include <vector>

struct LoadedCovidData {
	std::vector<CovidData> data;
	std::pair<Poco::DateTime, Poco::DateTime> dateRange;
};

LoadedCovidData loadCovidData(const string &filename);
