#include "covidLoader.h"
#include "ofMain.h"
#include "ofxCsv.h"

#include <map>
#include <string>

std::vector<CovidData> loadCovidData(const string &filename) {
	ofxCsv csv;
	std::vector<CovidData> covidData;

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

		tmp.caseType = caseTypeMap[x.getString(CovidData::FieldLabel::Case_Type)];;
		tmp.cases = x.getInt(CovidData::FieldLabel::Cases);
		tmp.difference = x.getInt(CovidData::FieldLabel::Difference);
		tmp.date = x.getString(CovidData::FieldLabel::Date);
		tmp.countryRegion = x.getString(CovidData::FieldLabel::Country_Region);
		tmp.provinceState = x.getString(CovidData::FieldLabel::Province_State);
		tmp.admin2 = x.getString(CovidData::FieldLabel::Admin2);
		tmp.combinedKey = x.getString(CovidData::FieldLabel::Combined_Key);
		tmp.fips = x.getInt(CovidData::FieldLabel::FIPS);
		tmp.latitude = x.getFloat(CovidData::FieldLabel::Lat);
		tmp.longitude = x.getFloat(CovidData::FieldLabel::Long);

		covidData.push_back(tmp);
	}

	return covidData;
}