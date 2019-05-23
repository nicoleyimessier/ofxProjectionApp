//
//  MainGUI.hpp
//  ProjectionApp
//
//  Created by Nicole Messier on 4/2/19.
//
//

#pragma once
#include "ofMain.h"
#include "ofxDatGui.h"
#include "ofxNotificationCenter.h"
#include "IDManager.h"

class MainGUI{
public:
    MainGUI();
    ~MainGUI();
    
    void setup(vector<string> &appStates, string _currentDirectory);
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
    
    /*
     Directory Management
     */
    string getCurrentDirectory();
    
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
    
    string posOfCropInWarp_X = "X Pos of Crop in Warp";
    string posOfCropInWarp_Y = "Y Pos of Crop in Warp";
    
    //! Config state management
    vector<string> states;
    
    /*
     ofxNotifcationSystem Callbacks
     */
    void onNewCropData(ofxNotificationCenter::Notification& n);
    
    /*
     Directory Management
     */
    
    /*
      projectionDirectory is the directory that all projection settings will be saved to.
      currentDirectory is the directory of the current or last saved projections
     
     For example, 
     currentDirectory = TESTA;
     projecitonDirectory = data/projections/
     
     fullPath = projectionDirectory + currentDirectory;
     */
    string projectionDirectory = "projections";
    string currentDirectory = "";

};
