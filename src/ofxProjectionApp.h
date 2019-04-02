//
//  ofxProjectionApp.h
//  DataGuiTest
//
//  Created by Nicole Messier on 2/5/19.
//
//

#pragma once
#include "ofMain.h"
#include "ofxWarp.h"
#include "ofxNotificationCenter.h"
#include "ofxJson.h"
#include "ofxInterface.h"
#include "EdgeBlend.h"
#include "ofxJson.h"
#include "CroppingManager.h"

class ofxProjectionApp
{
public:
    ofxProjectionApp();
    ~ofxProjectionApp();
    
    void setup(ofFbo * _canvasRef, bool _loadFromFile, string _directoryPath = "", ofVec2f _appSize, float _scaleDenominator = 1.0f);
    void setupWarps();
    void setupCroppingManager();
    void setupEdges();
    void update();
    void draw();
    
    /*
     Projector Data
     */
    void addProjector(App::Projector projector);
    
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
     Cropping Manager
     */
    CroppingManager *cropMan;
    
protected:
    /*
     Warping
     */
    ofxWarpController *warpController;
    bool loadFromFile = false;
    string directoryPath = "";
    
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
    vector<App::Projector> projectors;
    string warpDataFileName = "PROJECTION_SETTINGS";
    
    /*
     Canvas Size
     */
    ofFbo *canvasRef;
    ofVec2f canvasSize = ofVec2f(0.0f, 0.0f);
    
    /*
     State management of all the different GUIS
     */
    GUIStates guiState;
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
    ofImage debugImg;
    bool debugImage = false;
    
    /*
     Crop Data
     */
    ofxJSONElement cropData;
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
    void loadNewSettings();
    
    /*
     AppSize & Scale
     */
    ofVec2f appSize;
    
    //!scaleDenominator this is for development purposes if you are developing locally at a smaller scale
    float scaleDenominator = 1.0f;
};

