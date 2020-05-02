#pragma once

#include "covidData.h"
#include "ofxTime.h"

#include <string>
#include <vector>
#include <map>

typedef std::string BucketKey; // To make it easier to change in the future
typedef std::pair<Poco::DateTime, BucketKey> Key; // int  is the FIPS number, not sure how many there are
typedef std::map<Key, int32_t> BucketMap; // Bucket Data is a pointer into the data vector
typedef std::vector<BucketKey> Buckets;

struct LoadedCovidData {
	std::vector<CovidData> data;
	std::pair<Poco::Timestamp, Poco::Timestamp> dateRange;
	BucketMap bucketedData;
	Buckets buckets; // Which buckets exist
};

LoadedCovidData loadCovidData(const string &filename);
