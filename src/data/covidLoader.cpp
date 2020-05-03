#include "covidLoader.h"
#include "ofMain.h"
#include "ofxCsv.h"

#include <map>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <tuple>

static string DATE_FORMAT = "%n/%e/%Y";

struct RawCovidData {
	std::vector<CovidData> data;
	std::pair<Poco::Timestamp, Poco::Timestamp> dateRange;
};

RawCovidData _loadRawData(const string &filename);
std::pair<BucketMap, Buckets> _bucketData(const std::vector<CovidData>& data);
Poco::Timestamp parseDate(const string &input);
string countryOnly(const string &input);

LoadedCovidData loadCovidData(const string &filename) {
	auto data = _loadRawData(filename);
	sort(data.data.begin(), data.data.end());

	BucketMap bucketMap;
	Buckets buckets;

	tie(bucketMap, buckets) = _bucketData(data.data);

	return { data.data, data.dateRange, bucketMap, buckets };
}

RawCovidData _loadRawData(const string &filename) {
	ofxCsv csv;
	std::vector<CovidData> covidData;
	Poco::Timestamp earliestDate(INT64_MAX)
							  , latestDate(0);

	map<string, CovidData::CaseType> caseTypeMap{
		pair<string, CovidData::CaseType>("Confirmed",
										  CovidData::CaseType::confirmed),
		pair<string, CovidData::CaseType>("Deaths",
										  CovidData::CaseType::deaths)};

	csv.load(filename);
	int row = 0;
	for (auto x : csv) {
		row++;
		if (row == 1) {
			// Skip the first row since it's a header row
			continue;
		}
		CovidData tmp = CovidData();

		tmp.caseType =
			caseTypeMap[x.getString(CovidData::FieldLabel::Case_Type)];
		;
		tmp.cases = x.getInt(CovidData::FieldLabel::Cases);
		tmp.total_tested = x.getInt(CovidData::FieldLabel::Total_Tested);
		tmp.difference = x.getInt(CovidData::FieldLabel::Difference);
		tmp.date = parseDate(x.getString(CovidData::FieldLabel::Date));
		tmp.countryRegion = x.getString(CovidData::FieldLabel::Country_Region);
		tmp.provinceState = x.getString(CovidData::FieldLabel::Province_State);
		tmp.admin2 = x.getString(CovidData::FieldLabel::Admin2);
		tmp.combinedKey = x.getString(CovidData::FieldLabel::Combined_Key);
		tmp.fips = x.getInt(CovidData::FieldLabel::FIPS);
		tmp.latitude = x.getFloat(CovidData::FieldLabel::Lat);
		tmp.longitude = x.getFloat(CovidData::FieldLabel::Long);

		if (tmp.date < earliestDate) {
			earliestDate = tmp.date;
		}

		if (tmp.date > latestDate) {
			latestDate = tmp.date;
		}

		covidData.push_back(tmp);
	}

	pair<Poco::Timestamp, Poco::Timestamp> dateRange(earliestDate, latestDate);

	return { covidData, dateRange };
}

Poco::Timestamp parseDate(const string &input) {
	int timezone = 0;
	return Poco::DateTimeParser::parse(DATE_FORMAT, input, timezone).timestamp();
}

std::pair<BucketMap, Buckets> _bucketData(const std::vector<CovidData>& data) {
	BucketMap bucketMap;
	unordered_set<BucketKey> bucketSet;

	for(auto &entry : data) {
		int32_t item = entry.difference;
		BucketKey dimension = countryOnly(entry.countryRegion);
		Poco::Timestamp date = entry.date;

		bucketMap[make_pair(date, dimension)] += item;
		bucketSet.insert(dimension);
	}

	Buckets buckets(bucketSet.cbegin(), bucketSet.cend());
	sort(buckets.begin(), buckets.end());

	return make_pair(bucketMap, buckets);
}

string countryOnly(const string& input) {
	auto last_comma = find_if(input.crbegin(), input.crend(), [](char c) { return c == ','; });
	if (last_comma == input.crend()) return input;

	--last_comma; // Skip space, it's -- because it's a reverse iterator...
	string retval(input.crbegin(), last_comma);
	reverse(retval.begin(), retval.end());
	return retval;
}

int LoadedCovidData::getDataFor(const Poco::Timestamp &ts, const BucketKey &key) {
		auto entry = bucketedData.find(make_pair(ts, key));
		if (entry == bucketedData.end()) {
			return 0;
		} else {
			return entry->second;
		}
}
