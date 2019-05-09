//
//  AppSettings.cpp
//  DataGuiTest
//
//  Created by Nicole Messier on 2/19/19.
//
//

#include "AppSettings.h"


AppSettings::AppSettings()
{
    
}

AppSettings::~AppSettings()
{
    
}

void AppSettings::setup()
{
    /*
     Parse Json
     */
    
    bool parsingJson = jsonRef.open(configPath);
    
    if(parsingJson)
    {
        ofLogNotice("App_Settings::setup") << "Succesfully initial parse of " << configPath << " with data: " << jsonRef.getRawString();
        
        try
        {
            const ofxJSONElement & appSettings = jsonRef["Configurable"];
            
            /*
             App Section
             */
            const ofxJSONElement & app = appSettings["app"];
            const ofxJSONElement & _testing = app["testing"];
            const ofxJSONElement & _logToFile = app["logToFile"];
            
            testing = _testing.asBool();
            logToFile = _logToFile.asBool();
            
            /*
             Window section
             */
            const ofxJSONElement & window = appSettings["window"];
            const ofxJSONElement & _fullscreen = window["fullscreen"];
            const ofxJSONElement & _windowMode = window["windowMode"];
            const ofxJSONElement & _appWidth = window["appWidth"];
            const ofxJSONElement & _appHeight = window["appHeight"];
            const ofxJSONElement & _scaleDenominator = window["scale-denominator"];
            
            fullScreen = _fullscreen.asBool();
            windowMode = _windowMode.asInt();
            appSize.set(_appWidth.asFloat(), _appHeight.asFloat());
            scaleDenominator = _scaleDenominator.asFloat();
            
            /*
             Canvas
             */
            const ofxJSONElement & _canvasSize = appSettings["canvasSize"];
            const ofxJSONElement & _canvasWidth = _canvasSize["width"];
            const ofxJSONElement & _canvasHeight = _canvasSize["height"];
            
            canvasSize.set(_canvasWidth.asFloat(), _canvasHeight.asFloat());
            
            /*
             Warp Settings
             */
            const ofxJSONElement & warpSettings = appSettings["warpSettings"];
            const ofxJSONElement & _warpDirectoryPath = warpSettings["directoryPath"];
            const ofxJSONElement & _loadWarpSettingsFromFile = warpSettings["loadFromFile"];
            
            warpDirectoryPath = _warpDirectoryPath.asString();
            loadWarpSettingsFromFile = _loadWarpSettingsFromFile.asBool();
            
            
            /*
             Projectors
             */
            const ofxJSONElement & projectorsList = appSettings["projectors"];
            
            for(int i=0; i < projectorsList.size(); i++)
            {
                App::Projector temp;
                projectors.push_back(temp);
            }
            
            for(int i=0; i < projectors.size(); i++)
            {
                const ofxJSONElement & proj = projectorsList[i];
                const ofxJSONElement & _order= proj["order"];
                const ofxJSONElement & width = proj["width"];
                const ofxJSONElement & height = proj["height"];
                const ofxJSONElement & numWarps = proj["warps"];
                
                int order = _order.asInt();
                
                if(order < projectors.size())
                {
                    
                    projectors[order].order = order;
                    projectors[order].size.set(width.asFloat(), height.asFloat());
                    projectors[order].numWarps = numWarps.asInt();
                    numCrops += numWarps.asInt();
                    
                    ofLogNotice("AppSettings::setup") << "Adding projector # " << order << " with (width, height): " << projectors[order].size << " & " << numWarps << " warps";
                    
                    /*
                     Need to do:
                     -- make sure that all projectors are ordered correctly
                     -- make sure canvas size accounts for the entire width of each projector 
                     */
                }
            }
            
            
            
        }catch(exception exc){
            ofLogError("App_Settings::setup") << exc.what() << " While parsing " << configPath;
        }
    }
    else
    {
        ofLogError("App_Settings::setup") << "Failed to parse " << configPath;
    }
}
