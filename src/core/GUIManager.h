//
//  GUIManager.hpp
//  DataGuiTest
//
//  Created by Nicole Messier on 2/5/19.
//
//

#pragma once
#include "ofMain.h"
#include "ofxDatGui.h"
#include "ofxNotificationCenter.h"
#include "IDManager.h"

class GUIManager{
public:
    GUIManager();
    ~GUIManager();
    
    void setup(vector<string> &appStates, string _projectionDirectory);
    void update();
    void draw();
    
    /*
     Cropping Interface
     */
    float getCropSize();
    
    /*
     Gui Object
     */
    ofxDatGui *getGui();
    
    void toggleGuiVisiblity(); 
    
private:
    
    /*
     Gui Object
     */
     ofxDatGui *gui;
    
    
    /*
     Callbacks
     */
    void onTextInputEvent(ofxDatGuiTextInputEvent e);
    void onButtonEvent(ofxDatGuiButtonEvent e);
    void onDropDownEvent(ofxDatGuiDropdownEvent e);
    void onSliderEvent(ofxDatGuiSliderEvent e);
    
    /*
     Naming Convention
     */
    string dirName_input = "Projection Settings Directory";
    string saveMapping_button = "Save mapping settings.";
    string projectionSettings_dropDown = "Projection Settings";
    string croppingManSize_slider = "Cropping Interface Scale";
    string cropWidth = "Crop Width";
    string cropHeight = "Crop Height";
    string cropXpos = "Crop XPos";
    string cropYpos = "Crop YPos";
    string configState = "Configuration State";
    
    
    //! Config state management
    vector<string> states;
    
    /*
     ofxNotifcationSystem Callbacks
     */
    void onNewCropData(ofxNotificationCenter::Notification& n);
    
    /*
     Projection Directory
     */
    
    string projectionDirectory = "";
};
