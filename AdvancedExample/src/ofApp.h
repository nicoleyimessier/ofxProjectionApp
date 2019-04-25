#pragma once

#include "ofMain.h"
#include "CustomApp.h"
#include "ofxProjectionApp.h"
#include "AppSettings.h"
#include "CanvasManager.h"
#include "ofxInterface.h"
#include "Global.h"

class ofApp : public CustomApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);


	// APP CALLBACKS ////////////////////////////////////////

	void setupChanged(ofxScreenSetup::ScreenSetupArg &arg);
	void remoteUIClientDidSomething(RemoteUIServerCallBackArg & arg);
    void onEveryTouch(ofxInterface::TouchEvent &event);
    void onEveryTouchDown(ofxInterface::TouchEvent &event);
    
	// APP SETUP ////////////////////////////////////////////
	float p1;
    
    /*
     ofxNotification Callbacks
     */
    
    void onNewStateSelection(ofxNotificationCenter::Notification& n);
    

    /*
     Application States
     */
    void setAppState(App::AppStates _state);
    App::AppStates state = App::AppStates::SETUP;
    string getStateName(App::AppStates state);
    
    /*
     Canvas -- this is where you draw
     */
    CanvasManager * canvasMan;
    
    /*
     Projections
     */
    ofxProjectionApp * projections;
    
    /*
     ofxInteface scene
     */
    ofxInterface::Node* scene;
    
};
