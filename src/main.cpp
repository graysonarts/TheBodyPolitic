#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    ofGLWindowSettings settings;
    settings.setPosition({0.f,0.f});
    settings.windowMode = OF_WINDOW;
    auto controlWin = ofCreateWindow(settings);

    settings.setPosition({(float)controlWin->getWidth(),0.0f});
    settings.windowMode = OF_WINDOW;
    auto projectorWin = ofCreateWindow(settings);

		auto controlApp = std::make_shared<ofControlApp>();

    ofRunApp(controlWin, controlApp);
    ofRunApp(projectorWin, std::make_shared<ofDisplayApp>(controlApp));

    ofRunMainLoop();

}
