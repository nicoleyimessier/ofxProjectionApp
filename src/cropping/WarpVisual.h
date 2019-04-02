//
//  WarpVisual.hpp
//  DataGuiTest
//
//  Created by Nicole Messier on 3/7/19.
//
//

#pragma once
#include "ofMain.h"
#include "ofxInterface.h"
#include "IDManager.h"
#include "ofxNotificationCenter.h"

class WarpVisual : public ofxInterface::Node
{
public:
    WarpVisual();
    ~WarpVisual();
    
    void setup(ofVec2f pos, ofVec2f size, int _index);
    void update(float dt);
    void draw();
    
    /*
     ofxInterface Callbacks
     */
    void onTouchDown(ofxInterface::TouchEvent& event);
    void onTouchUp(ofxInterface::TouchEvent& event);
    void onClick(ofxInterface::TouchEvent& event);
    void onTouchMove(ofxInterface::TouchEvent& event);
    
    /*
     Canvas Reference
     */
    void setCanvasReference(ofFbo *_canvasRef);
    
    /*
     Attributes
     */
    void setActive(bool _active);
    
    /*
     Canvas Reference / crop info
     */
    void updateCropData(ofVec2f _cropPos, ofVec2f _cropSize);
    
private:
    
    /*
     Attributes
     */
    bool active = false; 
    int index = 0;
    
    
    /*
     Canvas Reference / crop info
     */
    ofFbo *canvasRef;
    ofVec2f cropPos = ofVec2f(0.0f, 0.0f);
    ofVec2f cropSize = ofVec2f(0.0f, 0.0f);
    
};
