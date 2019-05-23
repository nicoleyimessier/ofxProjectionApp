//
//  IDManager.h
//  ProjectionApp
//
//  Created by Nicole Messier on 4/2/19.
//

#pragma once
#include "ofMain.h"

/*
 Used to manage IDs for ofxNotificationSystem
 */
class IDManager
{
public:
    
    static IDManager& one()
    {
        static IDManager instance;
        return instance;
    }
    
    IDManager(){};
    ~IDManager(){};
    
    /*
     Notification IDs
     */
    string appState_id = "appState";
    string saveProjSetting_id = "projectionSettings";
    string loadProjSetting_id = "loadNewSetting";
    string updateCropInterface_id = "updateCropData"; // This id will be notified once the new crop data has been updated
    string edgeBlendGui_id = "edgeBlendEvent";
    
    //! Cropping manager IDs
    string croppingInterfaceScale_id = "croppingInterfaceScale";
    string warpVisual_click_id = "warpVisual_click";
    
    // These are the IDs that get sent from the GUI to the cropping manager
    string cropWidth_id = "cropWidth_id";
    string cropHeight_id = "cropHeight_id";
    string cropXpos_id = "cropXpos_id";
    string cropYpos_id = "cropYpos_id";
    string posOfCropInWarp_X_id = "crop_xPos_inWarp_id";
    string posOfCropInWarp_Y_id = "crop_yPos_inWarp_id";
    
    //These are the IDs that get sent from the Cropping Manager when a new warp visual is clicked and the gui needs to be updated.
    string crop_startInfo_id = "crop_startInfo_id";
    
private:
};
