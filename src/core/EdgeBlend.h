//
//  EdgeBlend.hpp
//  DataGuiTest
//
//  Created by Nicole Messier on 2/26/19.
//
//

/*
 This class will act as an interface for each edge of our system
 */

#pragma once
#include "ofMain.h"
#include "ofxDatGui.h"
#include "ofxNotificationCenter.h"
#include "IDManager.h"

class EdgeBlend
{
public:
    EdgeBlend();
    ~EdgeBlend();
    
    void setup(int _index);
    void update(float dt);
    void draw();
    
    
    /*
     Gui Object
     */
    ofxDatGui* getGuiObject();
    
    /*
     Get edge blending values
     */
    glm::vec4 getEdges();
    glm::vec3 getGamma();
    float getExponent();
    
    /*
     Set edge blending values
     */
    void setEdges(glm::vec4 edges);
    void setGamma(glm::vec3 gamma);
    void setExponent(float _exponent);
    
    /*
     Attributes
     */
    int getIndex();
    
private:
    
    /*
     Gui object
     */
    ofxDatGui *gui;
    
    /*
     Attributes
     */
    int index = 0;
    
    
    //Labels
    string gammaR_lab = "GAMMAR";
    string gammaB_lab = "GAMMAB";
    string gammaG_lab = "GAMMAG";
    
    string edgeX_lab = "LEFT_EDGE";
    string edgeY_lab = "TOP_EDGE";
    string edgeZ_lab = "RIGHT_EDGE";
    string edgeW_lab = "BTM_EDGE";
    
    string exponent_lab = "EXPONENT";
    
    string close_lab = "CLOSE";
    /*
     Toggle for button
     */
    bool active = false;
    
    
    /*
     Callbacks
     */
    void onButtonEvent(ofxDatGuiButtonEvent e);
};
