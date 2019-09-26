//
//  ofxProjectionApp.h
//  DataGuiTest
//
//  Created by Nicole Messier on 2/5/19.
//
//

// By default, this addon uses the native Openframeworks json
// parser, ofJson. However, if you would like to use the addon
// ofxJson (for its ability to load commented Json files),
// then you must define the preprocessor macro 
// OFXPROJECTIONAPP_USE_OFXJSON by uncommenting the line below
//#define OFXPROJECTIONAPP_USE_OFXJSON

#pragma once
#include "ofMain.h"
#include "ofxWarp.h"
#include "ofxNotificationCenter.h"
#include "ofxInterface.h"
#include "EdgeBlend.h"
#include "CroppingManager.h"
#include "ProjectorManager.h"
#include "MainGUI.h"
#ifdef OFXPROJECTIONAPP_USE_OFXJSON
#include "ofxJson.h"
#endif

class ofxProjectionApp
{
public:
    ofxProjectionApp();
    ~ofxProjectionApp();
    
    void setup(ofFbo * _canvasRef, bool _loadFromFile, ofVec2f _appSize, float _scaleDenominator, ofxInterface::Node* _sceneRef, string _directoryPath);
    void setupWarps();
    void setupCroppingManager();
    void setupGuiManager(vector<string> &appStates);
    void setupEdges();
    
    
    void update();
    void draw();

    
    /*
     State management of all the different GUIS
     */
    enum GUIStates
    {
        EDGE_BLEND_GUI_OPEN,
        EDGE_BLEND_GUIS_CLOSED,
        NUM_GUI_STATES
    };
    
    /*
     Saving
     */
    void saveCurrentSettings();

    /*
     GUI Visibility
     */
    void setCropManagerVisible(bool visiblity);
    void setMainGuiVisible(bool visibility);
    
	/*
	Warping
	*/
	void toggleEditingWarpsOff();
    
    //Add warp to the warp controller on the fly and cascade change to other classes
    void addWarp(ofxWarp::WarpBase::Type type, ofVec2f size, ofVec2f pos);
    
    //Remove warp to the warp controller on the fly and cascade change to other classes
    void removeWarp(ofxWarp::WarpBase::Type type);

protected:
    /*
     Warping
     */
    ofxWarpController *warpController;
    bool loadFromFile = false;
    string directoryPath = "";
    
    //WARP TYPE
    //NEED TO DO: Make this configurable enum
    bool perspective = true;
    
    enum ControlPoints
    {
        TOP_LEFT,
        TOP_RIGHT,
        BOTTOM_LEFT,
        BOTTOM_RIGHT
    };
    
    //Each warp will have a vector of edges
    vector<EdgeBlend*> edgeGuis;
    
    /*
     Projector Data
     */
    string warpDataFileName = "WARP_SETTINGS.json";
    
    /*
     Canvas Size
     */
    ofFbo *canvasRef;
    ofVec2f canvasSize = ofVec2f(0.0f, 0.0f);
    
    /*
     State management of all the different GUIS
     */
    GUIStates guiState = GUIStates::EDGE_BLEND_GUIS_CLOSED;
    void setGuiState(GUIStates _guiState);
    
    /*
     Notification Center
     */
    void onSaveSettings(ofxNotificationCenter::Notification& n);
    void onLoadSettings(ofxNotificationCenter::Notification& n);
    void onCloseEdgeBlendGui(ofxNotificationCenter::Notification& n);
    
    /*
     Standard callbacks
     */
    void onMousePressed(ofMouseEventArgs & args);
    void onKeyReleased(ofKeyEventArgs &args);
    ofImage debugImg;
    bool debugImage = false;
    
    /*
     Crop Data
     */
#ifdef OFXPROJECTIONAPP_USE_OFXJSON
	ofxJSONElement cropData;
#else 
	ofJson cropData;
#endif
    void setupCropJsonData();
    void clearCropJsonData();
    void saveCropJsonData(string fileName);
    string cropCropFileName = "CROP_DATA.json";
    
    /*
     Saving
     */
    string warpPath = "";
    string cropPath = "";
    
    /*
     Loading
     */

	//!load new settings is to load files on the fly
	void loadWarpSettings(); 
	void loadCropSettings(); 
    void loadNewSettings();
    
    /*
     AppSize & Scale
     */
    ofVec2f appSize;
    
    //!scaleDenominator this is for development purposes if you are developing locally at a smaller scale
    float scaleDenominator = 1.0f;
    
    /*
     Cropping Manager
     */
    CroppingManager *cropMan;
    
    /*
     GUIManager
     */
    
    MainGUI * guiMan;
    ofxInterface::Node* sceneRef;
    

};

