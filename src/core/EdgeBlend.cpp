//
//  EdgeBlende.cpp
//  DataGuiTest
//
//  Created by Nicole Messier on 2/26/19.
//
//

#include "EdgeBlend.h"

EdgeBlend::EdgeBlend()
{
    
}

EdgeBlend::~EdgeBlend()
{
    
}

void EdgeBlend::setup(int _index)
{
    
    //The index serves of the UID
    index = _index;
    
    // instantiate and position the gui //
    gui = new ofxDatGui( ofxDatGuiAnchor::TOP_RIGHT );
    gui->setAssetPath("");
    gui->setVisible(false);
    
    //Add gamma sliders
    gui->addSlider(gammaR_lab, 0.0f, 100.0f, 100.0f);
    gui->addSlider(gammaB_lab, 0.0f, 100.0f, 100.0f);
    gui->addSlider(gammaG_lab, 0.0f, 100.0f, 100.0f);
    
    //Add edge sliders
    gui->addSlider(edgeX_lab, 0.0f, 1.0f, 1.0f);
    gui->addSlider(edgeY_lab, 0.0f, 1.0f, 1.0f);
    gui->addSlider(edgeZ_lab, 0.0f, 1.0f, 1.0f);
    gui->addSlider(edgeW_lab, 0.0f, 1.0f, 1.0f);
    
    //Add exponenet slider
    gui->addSlider(exponent_lab, 1.0f, 5.0f, 1.0f);
    
    //Add close button
    gui->addButton(close_lab);
    gui->onButtonEvent(this, &EdgeBlend::onButtonEvent);
    
    //Add listeners
    
    
}

void EdgeBlend::update(float dt)
{
    
}

void EdgeBlend::draw()
{
    
    
}

#pragma mark GET EDGE BLENDING VALUES

glm::vec4 EdgeBlend::getEdges()
{
    glm::vec4 temp;
    temp = glm::vec4(gui->getSlider(edgeX_lab)->getValue(),
                     gui->getSlider(edgeY_lab)->getValue(),
                     gui->getSlider(edgeZ_lab)->getValue(),
                     gui->getSlider(edgeW_lab)->getValue());
    return temp;
}

glm::vec3 EdgeBlend::getGamma()
{
    glm::vec3 temp;
    temp = glm::vec3(gui->getSlider(gammaR_lab)->getValue(),
                     gui->getSlider(gammaG_lab)->getValue(),
                     gui->getSlider(gammaB_lab)->getValue());
    return temp;
}

float EdgeBlend::getExponent()
{
    return gui->getSlider(exponent_lab)->getValue();
}

#pragma mark SET EDGE BLENDING VALUES

void EdgeBlend::setEdges(glm::vec4 edges)
{
    gui->getSlider(edgeX_lab)->setValue(edges.x);
    gui->getSlider(edgeY_lab)->setValue(edges.y);
    gui->getSlider(edgeZ_lab)->setValue(edges.z);
    gui->getSlider(edgeW_lab)->setValue(edges.x);
    
}

void EdgeBlend::setGamma(glm::vec3 gamma)
{
    gui->getSlider(gammaR_lab)->setValue(gamma.x);
    gui->getSlider(gammaB_lab)->setValue(gamma.y);
    gui->getSlider(gammaG_lab)->setValue(gamma.z);
    
}

void EdgeBlend::setExponent(float _exponent)
{
    gui->getSlider(exponent_lab)->setValue(_exponent);
}

#pragma mark ATTRIBUTES
int EdgeBlend::getIndex()
{
    return index;
}

#pragma mark GUI
ofxDatGui * EdgeBlend::getGuiObject()
{
    return gui;
}

#pragma mark EVENTS


void EdgeBlend::onButtonEvent(ofxDatGuiButtonEvent e)
{
    //Send event via notification center
    ofxNotificationCenter::Notification mnd;
    mnd.ID = IDManager::one().edgeBlendGui_id;
    mnd.data["index"] = index;
    
    ofxNotificationCenter::one().postNotification(IDManager::one().edgeBlendGui_id, mnd);
}

