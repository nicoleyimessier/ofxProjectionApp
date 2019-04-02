//
//  Crop.hpp
//  DataGuiTest
//
//  Created by Nicole Messier on 3/7/19.
//
//

#pragma once
#include "ofMain.h"
#include "ofxInterface.h"

/*
 This class serves as an interactive element to change crop settings. 
 The position and size of this should be calculted by the crop manager and is 
 scaled down
 */
class Crop : public ofxInterface::Node
{
public :
    Crop();
    ~Crop();
    
    void setup(ofVec2f pos, ofVec2f size, ofVec2f _originOffset, ofVec2f _canvasSize);
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
     Scaled Attributes
     */
    void setPositionNormalized(ofVec2f _positionNormalized);
    void setSizeNormalized(ofVec2f _sizeNormalized);
    
    ofVec2f getPositionNormalized();
    ofVec2f getSizeNormalized();

    
private:
    
    /*
     Scaled Attributes
     */
    ofVec2f positionNormalized = ofVec2f(0.0f, 0.0f);
    ofVec2f sizeNormalized = ofVec2f(0.0f, 0.0f);
    ofVec2f originOffset = ofVec2f(0.0f, 0.0f);
    
    /*
     ofVec2f
     */
    
    ofVec2f canvasSize;
    
};
