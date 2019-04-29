#pragma once

#include "ofMain.h"
#include "ofxProjectionApp.h"
#include "ofxInterface.h"
#include "ofxNotificationCenter.h"

class ofApp : public ofBaseApp{

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
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);


    /*
     App States
     */
    
    // APP CALLBACKS ////////////////////////////////////////
    void onEveryTouch(ofxInterface::TouchEvent &event);
    void onEveryTouchDown(ofxInterface::TouchEvent &event);
    
    //These are the basic app states to use -- although it is up to determine how many states you need
    enum AppStates
    {
        SETUP,
        WARP_CONFIGURATION,
        CROP_CONFIGURATION,
        ACTIVE,
        NUM_STATES
    };
    
    AppStates state = AppStates::SETUP;
    void setAppState(AppStates _state);
    string getStateName(AppStates state);
    
    /*
     Projection manager
     */
    ofxProjectionApp * projections;
    
    /*
     Canvas
     */
    ofFbo *canvas;
    
    /*
     ofxInteface scene
     */
    ofxInterface::Node* scene;
    bool debug = false;

    /*
     ofxNotification Callbacks
     */
    
    void onNewStateSelection(ofxNotificationCenter::Notification& n);
    

};
