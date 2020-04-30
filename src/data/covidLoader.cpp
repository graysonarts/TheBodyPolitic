#include "covidLoader.h"
#include "ofMain.h"
#include "ofxCsv.h"

#include <map>
#include <string>

static string DATE_FORMAT = "%n/%e/%Y";

Poco::DateTime parseDate(string input) {
	int timezone = 0;
	return Poco::DateTimeParser::parse(DATE_FORMAT, input, timezone);
}

LoadedCovidData loadCovidData(const string &filename) {
	ofxCsv csv;
	std::vector<CovidData> covidData;
	Poco::DateTime earliestDate(3000, 12, 31)
							 , latestDate(1990, 1, 1);

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

	pair<Poco::DateTime, Poco::DateTime> dateRange(earliestDate, latestDate);

	return { covidData, dateRange };
}
