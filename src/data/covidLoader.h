#pragma once

#include "covidData.h"
#include "ofxTime.h"

#include <string>
#include <vector>
#include <map>

typedef std::pair<Poco::DateTime, int> Key; // int  is the FIPS number, not sure how many there are

struct LoadedCovidData {
	std::vector<CovidData> data;
	std::pair<Poco::DateTime, Poco::DateTime> dateRange;
	std::map<Key, CovidData*> bucketedData; // Bucket Data is a pointer into the data vector
	std::vector<int> buckets; // Which buckets exist
};

LoadedCovidData loadCovidData(const string &filename);
