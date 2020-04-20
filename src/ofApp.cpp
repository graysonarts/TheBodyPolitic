#include "ofApp.h"
#include "covidData.h"

void ofDisplayApp::setup() {
	ofxGuiEnableHiResDisplay();
	showGui = false;
	speed = speed.set("speed", 1., 1., 10.);
	drawLine = drawLine.set("clear screen per frame", false);

	gui.setup("panel");
	gui.setPosition(1000.0f, 0.f);
	gui.add(speed);
	gui.add(drawLine);

	speed.addListener(&covid19, &Covid19::onSpeedChange);
	drawLine.addListener(&covid19, &Covid19::onDrawChange);

	piMapper.registerFboSource(covid19);
	piMapper.setup();

	loadCovidCsv();

	ofLog() << "Loaded country . " << covidData[0].countryRegion;
	ofLog() << "Loaded country . " << covidData[1].countryRegion;
}

void ofDisplayApp::loadCovidCsv() {
	map<string, CovidData::CaseType> caseTypeMap{
		pair<string, CovidData::CaseType>("Confirmed",
										  CovidData::CaseType::confirmed),
		pair<string, CovidData::CaseType>("Deaths",
										  CovidData::CaseType::deaths)};

	csv.load("covid_data.csv");
	for (auto x : csv) {
		CovidData::CaseType caseType;

		if (auto it =
				caseTypeMap.find(x.getString(CovidData::FieldLabel::Case_Type));
			it != caseTypeMap.end()) {
			caseType = it->second;
		} else {
			ofLog() << "Couldn't find the thing: "
					<< x.getString(CovidData::FieldLabel::Case_Type);
			continue;
		}

		CovidData tmp = CovidData();

		tmp.caseType = caseType;
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
}

void ofDisplayApp::update() { piMapper.update(); }

void ofDisplayApp::draw() {
	ofBackground(0.0f);
	piMapper.draw();
	if (showGui)
		gui.draw();
}

void ofDisplayApp::keyPressed(int key) {
	if (key == 'i') {
		showGui = !showGui;
	}
	if (key == 'f') {
		ofToggleFullscreen();
	}
	if (key == ' ') {
		drawLine.set(!drawLine.get());
	}
	piMapper.keyPressed(key);
}

void ofDisplayApp::keyReleased(int key) { piMapper.keyReleased(key); }

void ofDisplayApp::mousePressed(int x, int y, int button) {
	piMapper.mousePressed(x, y, button);
}

void ofDisplayApp::mouseReleased(int x, int y, int button) {
	piMapper.mouseReleased(x, y, button);
}

void ofDisplayApp::mouseDragged(int x, int y, int button) {
	piMapper.mouseDragged(x, y, button);
}