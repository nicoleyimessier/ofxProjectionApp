#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    //ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
    
    ofGLFWWindowSettings winSettings;
    winSettings.numSamples = 8;
    winSettings.setGLVersion(3, 2);
    winSettings.setSize(1920, 1080);

    shared_ptr<ofAppBaseWindow> win = ofCreateWindow(winSettings);	// sets up the opengl context!
    ((ofAppGLFWWindow*)win.get())->setMultiDisplayFullscreen(true);
    
    // 10 x 1 screens
    //ofSetupOpenGL(&win, w, h, OF_WINDOW /*OF_FULLSCREEN*/);	// <-------- setup the GL context
    
    ofRunApp(win, shared_ptr<ofBaseApp>(new ofApp()));
    ofRunMainLoop();

}
