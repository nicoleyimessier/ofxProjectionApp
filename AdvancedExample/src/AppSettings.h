//
//  AppSettings.hpp
//  DataGuiTest
//
//  Created by Nicole Messier on 2/19/19.
//
//

#pragma once
#include "ofMain.h"
#include "ofxJson.h"
#include "AppStructs.h"

class AppSettings
{
public:
    
    static AppSettings& one()
    {
        static AppSettings instance;
        return instance;
    }
    
    AppSettings();
    ~AppSettings();
    
    void setup();
    
    /*
     Application
     */
    bool testing = false;
    bool logToFile = false;
    
    /*
     Window section
     */
    bool fullScreen = false;
    int windowMode = 0;
    ofVec2f appSize = ofVec2f(1920, 1080);
    float scaleDenominator = 1.0f;
    
    /*
     Warp & Crop Settings
     */
    bool loadWarpSettingsFromFile = false;
    string warpDirectoryPath = ""; 
    
    /*
     Projectors
     */
    vector<App::Projector> projectors;
    int numCrops = 0 ; 
    
    /*
     Canvas Size
     */
    ofVec2f canvasSize = ofVec2f(0.0f, 0.0f);

    
private:
    /*
     Json Element
     */
    ofxJSONElement jsonRef;
    string configPath = "AppSettings.json";
    
};
