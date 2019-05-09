//
//  CanvasManager.hpp
//  DataGuiTest
//
//  Created by Nicole Messier on 3/5/19.
//
//

#pragma once
#include "ofMain.h"

class CanvasManager
{
public:
    
    CanvasManager();
    ~CanvasManager();
    
    void setup(ofVec2f _canvasSize);
    void update(float dt);
    void draw();
    
    //! Returns canvas fbo
    ofFbo *getCanvas();
    ofVec2f getSize();
    
    void setShowGrid(bool _showGrid); 
    void toggleShowGrid(); 
private:
    
    /*
     Scene FBO to draw into
     */
    ofFbo *canvas;
    
    /*
     Background Image
     */
    ofImage *bgImg; 
    
    /*
     Canvas
     */
    ofVec2f canvasSize;
    
    /*
     Grid
     */
    bool showGrid = false;
    float boxSize = 100.0f;
    int numRows = 10;
    int numColumns = 10;

};
