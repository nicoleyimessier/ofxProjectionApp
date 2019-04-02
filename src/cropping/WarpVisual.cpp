//
//  WarpVisual.cpp
//  DataGuiTest
//
//  Created by Nicole Messier on 3/7/19.
//
//

#include "WarpVisual.h"

WarpVisual::WarpVisual()
{
    
}

WarpVisual::~WarpVisual()
{
    
}

void WarpVisual::setup(ofVec2f pos, ofVec2f size,  int _index)
{
    //Set up attributes
    setSize(size.x, size.y);
    setPosition(pos);
    index = _index;
    
    //Set up listeners for ofxInterface
    ofAddListener(eventTouchDown, this, &WarpVisual::onTouchDown);
    ofAddListener(eventTouchUp, this, &WarpVisual::onTouchUp);
    ofAddListener(eventTouchMove, this, &WarpVisual::onTouchMove);
    ofAddListener(eventClick, this, &WarpVisual::onClick);
  
}

void WarpVisual::update(float dt)
{
    
}

void WarpVisual::draw()
{
    if(active)
    {
        //Draw a border when the warp is actively changing the crop data
        
        ofSetColor(ofColor::gray);
        ofNoFill();
        ofDrawRectangle(0.0f, 0.0f, size.x, size.y);
    }
    
    ofSetColor(255.0f);
    
    /// \param x Draw position on the x axis.
    /// \param y Draw position on the y axis.
    /// \param w Draw width.
    /// \param h Draw height.
    /// \param sx Subsection x axis offset within the texture.
    /// \param sy Subsection y axis offset within the texture.
    
    ofVec2f cropPos = Global::one().cropData[index].pos;
    ofVec2f cropSize = Global::one().cropData[index].size;
    
    canvasRef->getTexture().drawSubsection(0.0f, 0.0f, size.x, size.y,
                                            cropPos.x,
                                            cropPos.y,
                                            cropSize.x,
                                            cropSize.y);

    
    
    
    
}

#pragma mark ATTRIBUTES

void WarpVisual::setActive(bool _active)
{
    active = _active;
    
}

#pragma mark CANVAS
void WarpVisual::setCanvasReference(ofFbo * _canvasRef)
{
    canvasRef = _canvasRef;
}


#pragma mark CALLBACKS

void WarpVisual::onTouchDown(ofxInterface::TouchEvent& event)
{
    
}

void WarpVisual::onTouchUp(ofxInterface::TouchEvent& event)
{
    
}

void WarpVisual::onClick(ofxInterface::TouchEvent& event)
{
    //Send event via notification center
    ofxNotificationCenter::Notification mnd;
    mnd.ID = IDManager::one().warpVisual_click_id;
    mnd.data["index"] = index;
    
    ofxNotificationCenter::one().postNotification(IDManager::one().warpVisual_click_id, mnd);
}

void WarpVisual::onTouchMove(ofxInterface::TouchEvent& event)
{
    
}
