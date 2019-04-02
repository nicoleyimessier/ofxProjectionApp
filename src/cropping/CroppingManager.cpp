//
//  CroppingManager.cpp
//  DataGuiTest
//
//  Created by Nicole Messier on 3/4/19.
//
//

#include "CroppingManager.h"

CroppingManager::CroppingManager()
{
    
}

CroppingManager::~CroppingManager()
{
    
}

void CroppingManager::setup(ofVec2f size, ofVec2f pos, int numCrops, ofFbo *_canvasRef)
{
    //Set up node
    setSize(size.x, size.y);
    setPosition(pos);
    setEnabled(true);
    
    canvasRef = _canvasRef;
    canvasSize.set(canvasRef->getWidth(), canvasRef->getHeight());
    
    //Set up crops
    //The number of crops is equal to the number of warps in the system
    calculateCanvasAttributes();
    
    //Set up Scaled projectors
    for(int i=0; i < AppSettings::one().projectors.size() ; i++)
    {
        App::Projector data;
        data.order = AppSettings::one().projectors[i].order;
        data.numWarps = AppSettings::one().projectors[i].numWarps;
        scaledProjectors.push_back(data);
    }
    
    caclulateProjectorAttributes();
    
    //Set up warps
    int counter = 0;
    for(int i=0; i < numCrops; i++)
    {
        WarpVisual *temp  = new WarpVisual();
        temp->setup(ofVec2f(0.0f, 0.0f), ofVec2f(0.0f, 0.0f), counter);
        temp->setCanvasReference(canvasRef);
        addChild(temp);
        warps.push_back(temp);
        
        //Create a global crop object
        App::CropInfo data;
        data.size.set(canvasSize.x/numCrops, canvasSize.y);
        data.pos.set(0.0f, 0.0f);
        data.index = i;
        Global::one().cropData.push_back(data);
        
        counter++;
    }
    
    calculateWarpAttributes();

    
    // Set up crop bounding box
    cropBoundingBox = new Crop();
    cropBoundingBox->setup(canvasPos, ofVec2f(canvasSizeScaled.x/numCrops, canvasSizeScaled.y), canvasPos);
    addChild(cropBoundingBox);
    cropBoundingBox->setVisible(false);
    cropBoundingBox->setPlane(10.0f); 
    
    //Set up listeners for ofxInterface
    ofAddListener(eventTouchDown, this, &CroppingManager::onTouchDown);
    ofAddListener(eventTouchUp, this, &CroppingManager::onTouchUp);
    ofAddListener(eventTouchMove, this, &CroppingManager::onTouchMove);
    ofAddListener(eventClick, this, &CroppingManager::onClick);

    //Set up notification center listeners
    ofxNotificationCenter::one().addObserver(this, &CroppingManager::onResizeCropInterface, IDManager::one().croppingInterfaceScale_id);
    ofxNotificationCenter::one().addObserver(this, &CroppingManager::onWarpVisualClick, IDManager::one().warpVisual_click_id);
    ofxNotificationCenter::one().addObserver(this, &CroppingManager::onUpdateWidth, IDManager::one().cropWidth_id);
    ofxNotificationCenter::one().addObserver(this, &CroppingManager::onUpdateHeight, IDManager::one().cropHeight_id);
    ofxNotificationCenter::one().addObserver(this, &CroppingManager::onUpdateXpos, IDManager::one().cropXpos_id);
    
    ofxNotificationCenter::one().addObserver(this, &CroppingManager::onUpdateYpos, IDManager::one().cropYpos_id);

    ofxNotificationCenter::one().addObserver(this, &CroppingManager::onUpdateInterface, IDManager::one().updateCropInterface_id);
    
}

void CroppingManager::update(float dt)
{
    
}

void CroppingManager::draw()
{
    ofSetColor(ofColor::blue, 100.0f);
    ofDrawRectangle(0, 0, size.x, size.y);
    ofSetColor(ofColor::white);
    
    
    /*
     Draw canvas on top half of interface
    */

    canvasRef->draw(canvasPos.x, canvasPos.y, canvasSizeScaled.x, canvasSizeScaled.y);
    
    /*
     Draw projectors on top of interface
     */
    for(int i = 0; i < scaledProjectors.size(); i++)
    {

        ofSetColor(ofColor::green);
        ofSetLineWidth(10.0f);
        ofNoFill();
        ofDrawRectangle(scaledProjectors[i].pos.x, scaledProjectors[i].pos.y,
                        scaledProjectors[i].size.x, scaledProjectors[i].size.y);
        
        ofSetColor(ofColor::black);
        ofFill();
        ofDrawRectangle(scaledProjectors[i].pos.x, scaledProjectors[i].pos.y,
                        scaledProjectors[i].size.x, scaledProjectors[i].size.y);
    }
    
    // Update visual warp data
    for(int i = 0; i < warps.size(); i++){
        
    }
    
    
    ofSetColor(255.0f);
    
    
}

#pragma mark ATTRIBUTES
// CroppingManager::calculateCanvasAttributes() calculates the scaled attributes for the canvas
void CroppingManager::calculateCanvasAttributes()
{
    
    canvasPos = ofVec2f(size.x*borderPercent, size.y*borderPercent);
    float canvasRatio = canvasRef->getHeight() / canvasRef->getWidth();
    
    if(canvasRef->getWidth() > canvasRef->getHeight())
    {
        canvasSizeScaled.y = (size.y * boundingBoxPercent.y);
        canvasSizeScaled.x = (canvasSizeScaled.y)/canvasRatio;
        
    }
    else
    {
        // Need to do scaling for portrait applicaiton
    }
    
}

// CroppingManager::caclulateProjectorAttributes() calculates the scaled attributes for the projectors
void CroppingManager::caclulateProjectorAttributes()
{
    projectorOrigin = ofVec2f(canvasPos.x, canvasPos.y + canvasSizeScaled.y + size.y*borderPercent);

    float projector_xPos = projectorOrigin.x;

    for(int i = 0; i < scaledProjectors.size(); i++)
    {
        ofVec2f projectorSize = AppSettings::one().projectors[i].size;
        float projectorRatio = projectorSize.y / projectorSize.x;
        scaledProjectors[i].size.y = size.y*boundingBoxPercent.y;
        scaledProjectors[i].size.x = scaledProjectors[i].size.y/projectorRatio;
        scaledProjectors[i].pos.y = projectorOrigin.y;
        scaledProjectors[i].pos.x = projector_xPos;
        
        projector_xPos += scaledProjectors[i].size.x;
        
        
    }
}

void CroppingManager::calculateWarpAttributes()
{
    int counter = 0;
    float warpOriginX = projectorOrigin.x;
    
    for(int i=0; i < scaledProjectors.size() ; i++)
    {
        
        int numWarps = AppSettings::one().projectors[i].numWarps;
        float warpWidth = scaledProjectors[i].size.x/numWarps;
        
        for(int j = 0; j < numWarps; j++)
        {
            //Create a visual warp object for cropping interface
            warps[counter]->setPosition(ofVec2f(warpOriginX, projectorOrigin.y));
            warps[counter]->setSize(ofVec2f(warpWidth, scaledProjectors[i].size.y));
            warpOriginX += warpWidth;
            counter++;
    
        }
        
    }
    
}

void CroppingManager::calculateBoundingBoxAttributes(int warpIndex)
{
    cropBoundingBox->setVisible(true);
    
    //! Update GUI values
    ofVec2f newCropSize = Global::one().cropData[warpIndex].size / canvasSize;
    ofVec2f newCropPos = Global::one().cropData[warpIndex].pos / canvasSize;
    
    
    ofxNotificationCenter::Notification mnd;
    mnd.ID = IDManager::one().crop_startInfo_id;
    mnd.data["width"] = newCropSize.x;
    mnd.data["height"] = newCropSize.y;
    mnd.data["xPos"] = newCropPos.x;
    mnd.data["yPos"] = newCropPos.y;
    
    ofxNotificationCenter::one().postNotification(Global::one().IDManager, mnd);
    
    
    //! Update crop visual bounding box
    cropBoundingBox->setSize(newCropSize*canvasSizeScaled);
    
    //! Update position of visual bounding box
    cropBoundingBox->setPosition(canvasPos + newCropPos*canvasSizeScaled);
    
}


#pragma mark NOTIFICATION CENTER CALLBACKS

void CroppingManager::onResizeCropInterface(ofxNotificationCenter::Notification& n)
{
    percent = n.data["percent"]; //get what you need from the data field
    ofVec2f appSize = ofVec2f(ofGetWidth(), ofGetHeight()); ;
    setSize(appSize.x*percent, appSize.y*percent);
    
    calculateCanvasAttributes();
    caclulateProjectorAttributes();
    calculateWarpAttributes();
    calculateBoundingBoxAttributes(activeIndex);
}

#pragma mark CROP CALLBACKS

void CroppingManager::onWarpVisualClick(ofxNotificationCenter::Notification& n)
{
    int newActiveIndex = n.data["index"]; //get what you need from the data field
    
    ofLogNotice("CroppingManager::onWarpVisualClick") << "Activiating warp #" << activeIndex;
    
    for(int i = 0; i < warps.size(); i++)
    {
        (i == newActiveIndex) ? warps[i]->setActive(true) : warps[i]->setActive(false);
    }
    
    activeIndex = newActiveIndex;
    calculateBoundingBoxAttributes(activeIndex);
    
    
}

void CroppingManager::onUpdateWidth(ofxNotificationCenter::Notification& n)
{
    float width_normalized = n.data["width"]; //get what you need from the data field
    
    //Update global crop data
    float newCropData = width_normalized * canvasSize.x;
    Global::one().cropData[activeIndex].size.x = newCropData;
    
    ofLogNotice("CroppingManager::onUpdateWidth") << "Update warp width to " << newCropData;
    
    //Update crop visual
    cropBoundingBox->setSize(ofVec2f(width_normalized*canvasSizeScaled.x, cropBoundingBox->getSize().y));
    
    
    
}

void CroppingManager::onUpdateHeight(ofxNotificationCenter::Notification& n)
{
    float height_normalized = n.data["height"]; //get what you need from the data field
    
    //Update global crop data
    float newCropData = height_normalized * canvasSize.y;
    Global::one().cropData[activeIndex].size.y = newCropData;
    
    ofLogNotice("CroppingManager::onUpdateHeight") << "Update warp width to " << newCropData;
    
    //Update crop visual
    cropBoundingBox->setSize(ofVec2f(cropBoundingBox->getSize().x, height_normalized*canvasSizeScaled.y));
}

void CroppingManager::onUpdateXpos(ofxNotificationCenter::Notification&n)
{
    float xPos_normalized = n.data["cropXpos"];
    float newXpos = xPos_normalized * canvasSize.x;
    Global::one().cropData[activeIndex].pos.x = newXpos;
    
    ofLogNotice("CroppingManager::onUpdateXpos") << "Update warp xPos to " << newXpos;
    
    // Update crop visual position
    cropBoundingBox->setPosition(ofVec2f(xPos_normalized*canvasSizeScaled.x + canvasPos.x,
                                         cropBoundingBox->getPosition().y));
                                 
}

void CroppingManager::onUpdateYpos(ofxNotificationCenter::Notification&n)
{
    float yPos_normalized = n.data["cropYpos"];
    float newYpos = yPos_normalized * canvasSize.x;
    Global::one().cropData[activeIndex].pos.y = newYpos;
    
    ofLogNotice("CroppingManager::onUpdateYpos") << "Update warp yPos to " << newYpos;
    // Update crop visual position
    cropBoundingBox->setPosition(ofVec2f(cropBoundingBox->getPosition().x,
                                         yPos_normalized*canvasSizeScaled.y + canvasPos.y));
}

void CroppingManager::onUpdateInterface(ofxNotificationCenter::Notification&n)
{
    ofVec2f posNoramlized = Global::one().cropData[activeIndex].pos / canvasSize;
    ofVec2f sizeNormalized = Global::one().cropData[activeIndex].size / canvasSize;
    
    ofLogNotice("CroppingManager::onUpdateInterface") << "Update crop visual data to posNoramlized: " << posNoramlized << " and sizeNormalized: " << sizeNormalized;
    // Update crop visual position
    cropBoundingBox->setPosition(canvasPos + posNoramlized*canvasSizeScaled);
    cropBoundingBox->setSize(sizeNormalized*canvasSizeScaled);
}


#pragma mark CALLBACKS

void CroppingManager::onTouchDown(ofxInterface::TouchEvent& event)
{
    touch = true;
    prevPos = event.position;
}

void CroppingManager::onTouchUp(ofxInterface::TouchEvent& event)
{
    touch = false;
    prevPos = event.position;
}

void CroppingManager::onClick(ofxInterface::TouchEvent& event)
{
    
}

void CroppingManager::onTouchMove(ofxInterface::TouchEvent& event)
{
    //ofLogNotice("GUI::onTouchMove") << "Moving!";
    
    if(touch)
    {
        ofVec2f eventOrign = event.position - getPosition();
        
        
        float diffX = fabs(prevPos.x - event.position.x);
        float diffY = fabs(prevPos.y - event.position.y);
        ofVec2f newPos;
        
        if(prevPos.x < event.position.x)
        {
            newPos.x = getPosition().x + diffX;
        } else {
            newPos.x = getPosition().x - diffX;
        }
        
        if(prevPos.y < event.position.y)
        {
            newPos.y = getPosition().y + diffY;
        } else {
            newPos.y = getPosition().y - diffY;
        }
        
        
        setPosition(newPos);
        prevPos = event.position;
    }
}
