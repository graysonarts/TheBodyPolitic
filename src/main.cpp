#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	  float scale = 2.0f;
    ofGLWindowSettings settings;

    settings.setPosition({0.0f, 0.0f});
		settings.setSize(1024*scale, 768*scale);
    settings.windowMode = OF_WINDOW;

    auto projectorWin = ofCreateWindow(settings);

    ofRunApp(projectorWin, std::make_shared<ofDisplayApp>());

    ofRunMainLoop();

}
