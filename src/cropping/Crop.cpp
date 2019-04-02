//
//  Crop.cpp
//  DataGuiTest
//
//  Created by Nicole Messier on 3/7/19.
//
//

#include "Crop.h"

Crop::Crop()
{
    
}

Crop::~Crop()
{
    
}

void Crop::setup(ofVec2f pos, ofVec2f size, ofVec2f _originOffset, ofVec2f _canvasSize)
{
    setPosition(pos);
    setSize(size.x, size.y);
    
    canvasSize = _canvasSize;
    
    //Calculate normalized values
    originOffset = _originOffset;
    positionNormalized = (pos - originOffset) / canvasSize;
    sizeNormalized = size/canvasSize;
    
}

void Crop::update(float dt)
{
    
}

void Crop::draw()
{

    ofSetColor(255.0f, 0.0f, 0.0f, 100.0f);
    ofDrawRectangle(0.0f, 0.0f, size.x, size.y);
    
}

#pragma mark SCALED ATTRIBUTES

void Crop::setPositionNormalized(ofVec2f _positionNormalized)
{
    positionNormalized = _positionNormalized;
}

void Crop::setSizeNormalized(ofVec2f _sizeNormalized)
{
    sizeNormalized = _sizeNormalized;
}

ofVec2f Crop::getPositionNormalized()
{
    return positionNormalized;
}

ofVec2f Crop::getSizeNormalized()
{
    return sizeNormalized;
}


#pragma mark OFXINTERFACE CALLBACKS

void Crop::onTouchDown(ofxInterface::TouchEvent& event)
{
    
}

void Crop::onTouchUp(ofxInterface::TouchEvent& event)
{
    
}

void Crop::onClick(ofxInterface::TouchEvent& event)
{
    
}

void Crop::onTouchMove(ofxInterface::TouchEvent& event)
{
    
}

