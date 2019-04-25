//
//  CanvasManager.cpp
//  DataGuiTest
//
//  Created by Nicole Messier on 3/5/19.
//
//

#include "CanvasManager.h"

CanvasManager::CanvasManager()
{
    
}

CanvasManager::~CanvasManager()
{
    
}

void CanvasManager::setup(ofVec2f _canvasSize)
{
    canvasSize = _canvasSize;
    
    //Set up FBO
    ofFbo::Settings fboSettings;
    fboSettings.width = canvasSize.x;
    fboSettings.height = canvasSize.y;
    fboSettings.useDepth = false;
    fboSettings.textureTarget = GL_TEXTURE_RECTANGLE_ARB; //mipmaps!
    fboSettings.numSamples = 8;
    fboSettings.numColorbuffers = 1;
    
    canvas = new ofFbo();
    canvas->allocate(fboSettings);
    canvas->begin();
    ofClear(0,0,0,0);
    canvas->end();
    
    bgImg = new ofImage();
    bgImg->load("assets/BG.png");
    
    //Set up grid values
     numRows = floor(canvasSize.y / boxSize);
     numColumns = floor(canvasSize.x / boxSize);
    
}

void CanvasManager::update(float dt)
{
    
}

void CanvasManager::draw()
{
    canvas->begin();
    {
        ofClear(0.0f, 0.0f, 0.0f);
        
        //Drawing this for testing purposes
        if(showGrid)
        {

            int counter = 1;
            
            for(int i=0; i < numColumns; i++)
            {
                
                for(int j = 0; j < numRows; j++)
                {
                    (counter%2) ? ofSetColor(0) : ofSetColor(255);
                    ofVec2f pos = ofVec2f(i*boxSize, j*boxSize);
                    
                    ofDrawRectangle(pos.x, pos.y, boxSize, boxSize);
                    
                    
                    ofSetColor(ofColor::yellow);
                    ofDrawBitmapString(ofToString(counter), pos.x + boxSize/2, pos.y + boxSize/2);
                    counter++;
                }
            }
        }
        else
        {
            bgImg->draw(0, 0, canvasSize.x, canvasSize.y);
        }
    }
    canvas->end();
}

#pragma mark CANVAS

ofFbo *CanvasManager::getCanvas()
{
    return canvas;
}

ofVec2f CanvasManager::getSize()
{
    return canvasSize; 
}

#pragma mark GRID

void CanvasManager::setShowGrid(bool _showGrid)
{
    showGrid = _showGrid; 
}

void CanvasManager::toggleShowGrid()
{
    showGrid = !showGrid; 
}
