//
//  AppStructs.hpp
//  DataGuiTest
//
//  Created by Nicole Messier on 2/20/19.
//
//

#pragma once
#include "ofMain.h"
#include "ofxNotificationCenter.h"

namespace App
{
    /*
     App States
     */
    enum AppStates{
        SETUP,
        WARP_CONFIGURATION,
        CROP_CONFIGURATION,
        ACTIVE,
        NUM_STATES
    };
    
    /*
     Projector Data
     */
    struct Projector
    {
        int order = 0;
        int numWarps = 0; 
        ofVec2f size = ofVec2f(0.0f, 0.0f);
        ofVec2f pos = ofVec2f(0.0f, 0.0f); 
    };
    
    
    /*
     Generaric notification
     */
    struct GeneralData : public ofxNotificationCenter::Notification {
        
        string stringData = "";
        
        GeneralData(){
            ofLogNotice("GeneralData") << "GeneralData()";
        }
        
        ~GeneralData(){
            ofLogNotice("GeneralData") << "~GeneralData()";
        }
    };
    
}
