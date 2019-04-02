//
//  CroppingManager.hpp
//  DataGuiTest
//
//  Created by Nicole Messier on 3/4/19.
//
//

#pragma once
#include "ofMain.h"
#include "ofxInterface.h"
#include "ofxNotificationCenter.h"
#include "Crop.h"
#include "WarpVisual.h"
#include "IDManager.h"



class CroppingManager : public ofxInterface::Node
{
public:
    CroppingManager();
    ~CroppingManager();
    
    void setup(ofVec2f size, ofVec2f pos, int numCrops, ofFbo *_canvasRef);
    void update(float dt);
    void draw();
    
    /*
     Callbacks
     */
    void onTouchDown(ofxInterface::TouchEvent& event);
    void onTouchUp(ofxInterface::TouchEvent& event);
    void onClick(ofxInterface::TouchEvent& event);
    void onTouchMove(ofxInterface::TouchEvent& event);
    
    
    /*
     Crop Info
     // Notes:
     -Data should still be on to scale (i.e. not scaled down)
     */
    void onWarpVisualClick(ofxNotificationCenter::Notification& n);
    void onUpdateWidth(ofxNotificationCenter::Notification& n);
    void onUpdateHeight(ofxNotificationCenter::Notification& n);
    void onUpdateXpos(ofxNotificationCenter::Notification& n);
    void onUpdateYpos(ofxNotificationCenter::Notification& n);
    void onUpdateInterface(ofxNotificationCenter::Notification& n);
    
    /*
     Crop Data
     */
    struct CropInfo
    {
        ofVec2f pos = ofVec2f(0.0f, 0.0f);
        ofVec2f size = ofVec2f(100.0f, 100.0f);
        int index = 0;
    };
    
    vector<CropInfo> cropData;
    
public:
    
    CropInfo getCropData(int cropIndex);
    void updateCropData(CropInfo data, int CropIndex);
    int getCropDataSize();
    
private:
    /*
    Attributes
     */
    float percent = 1.0f;
    float borderPercent = 0.1f;
    ofVec2f boundingBoxPercent = ofVec2f(0.8, 0.35);
    
    //! Scaled canvas attributes
    ofVec2f canvasPos;
    ofVec2f canvasSizeScaled;
    void calculateCanvasAttributes();
    
    //! Scaled projector attributes
    ofVec2f projectorOrigin;
    vector<App::Projector> scaledProjectors;
    void caclulateProjectorAttributes();
    
    /*
     NEED TO DO: Scaled Data
     */
    
    /*
     Touch
     */
    bool touch = false;
    ofVec2f prevPos;
    
    /*
     Canvas Reference
     */
    ofFbo *canvasRef;
    ofVec2f canvasSize;
    /*
     ofxNotificationCenter Callbacks
     */
    void onResizeCropInterface(ofxNotificationCenter::Notification& n); 
    
    
    /*
     Crop Info
        // Notes: 
        - Data should still be to scale (i.e. not scaled down)
        - Crops are visuals so the attributes (i.e. size and position ARE scaled)
     */
    void calculateWarpAttributes();
    vector<WarpVisual*> warps;
    int activeIndex = 0;
    
    /*
     Crop bounding box -- this is just for visual purposes
     */
    Crop * cropBoundingBox;
    void calculateBoundingBoxAttributes(int warpIndex);
    
    /*
     CropData
     */

};

