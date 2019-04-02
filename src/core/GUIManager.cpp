//
//  GUIManager.cpp
//  DataGuiTest
//
//  Created by Nicole Messier on 2/5/19.
//
//

#include "GUIManager.h"

GUIManager::GUIManager()
{
    
}

GUIManager::~GUIManager()
{
    
}

void GUIManager::setup(vector<string> &appStates, string _projectionDirectory)
{
    
    projectionDirectory = _projectionDirectory;
    
    // instantiate and position the gui //
    gui = new ofxDatGui( ofxDatGuiAnchor::TOP_RIGHT );
    gui->setAssetPath("");
	gui->setPosition(0.0f, 0.0f); 
    
    /*
     Add text component for directory
     */
    gui->addTextInput(dirName_input, "Enter directory");
    gui->onTextInputEvent(this, &GUIManager::onTextInputEvent);
    
    /*
     Add button for saving new mapping directory
     */
    gui->addButton(saveMapping_button);
    gui->onButtonEvent(this, &GUIManager::onButtonEvent);
    
    /*
     Add drop down menu for projection settings
     */
    ofDirectory dir;
    dir.listDir(projectionDirectory);
    int numFiles = dir.size();
    vector<string> paths;
    
    for(int i = 0; i < numFiles; i++)
    {
        string path = dir.getPath(i);
        paths.push_back(path);
    }
    
    gui->onDropdownEvent(this, &GUIManager::onDropDownEvent);
    gui->addDropdown(projectionSettings_dropDown, paths);
    
    /*
     Add slider for cropping manager (i.e. cropping interface) scale
     */
    gui->addSlider(croppingManSize_slider, 0.0f, 1.0f, 0.5f);
    gui->addSlider(cropWidth, 0.0f, 1.0f, 1.0f);
    gui->addSlider(cropHeight, 0.0f, 1.0f, 1.0f);
    gui->addSlider(cropXpos, 0.0f, 1.0f, 1.0f);
    gui->addSlider(cropYpos, 0.0f, 1.0f, 1.0f);
    gui->onSliderEvent(this, &GUIManager::onSliderEvent);
    
    
    /*
    Add drop down for each state
     */
    states = appStates;
    gui->addDropdown(configState, appStates);
    
    /*
     Add ofxNotifcation Observers
     */
    ofxNotificationCenter::one().addObserver(this, &GUIManager::onNewCropData, Global::one().crop_startInfo_id);
    
}

void GUIManager::update()
{
    
}

void GUIManager::draw()
{
    
}

#pragma mark GUI OBJECT

ofxDatGui * GUIManager::getGui()
{
    return gui;
}

void GUIManager::toggleGuiVisiblity()
{
    if(gui->getVisible())
    {
        gui->setVisible(false);
    }
    else
    {
        gui->setVisible(true);
    }
}


#pragma mark CROPPING INTERFACE
float GUIManager::getCropSize()
{
    return gui->getSlider(croppingManSize_slider)->getValue(); 
}

#pragma mark CALLBACKS

void GUIManager::onTextInputEvent(ofxDatGuiTextInputEvent e)
{
    ofLogNotice("GUIManager::onTextInputEvent") << "onTextInputEvent: " << e.target->getLabel() << " " << e.target->getText();
    
    string directory = e.target->getText();
    
    /*
     Clean up text.
     */
    
    if(ofIsStringInString(e.target->getText(), " "))
    {
        string input = e.target->getText();
        vector<string> stringParts;
        stringParts = ofSplitString(input, " ");
        directory = "";
        
        for(auto part : stringParts)
        {
            directory += part;
        }
        
        ofLogNotice("GUIManager::onTextInputEvent") << "Reformated \" " << input << "\" to " << directory;
    }
    
    Global::one().currentDirectory = directory;
    
}

void GUIManager::onButtonEvent(ofxDatGuiButtonEvent e)
{
    ofLogNotice("GUIManager::onButtonEvent") << "Button " << e.target->getLabel() << " pressed";
    
    //Add to the drop down menu
    string path = ofFilePath::join(Global::one().projectionDirectory, Global::one().currentDirectory);
    
    bool dropDownExist = false;
    for(int i = 0; i < gui->getDropdown(projectionSettings_dropDown)->size(); i++)
    {
        string label = gui->getDropdown(projectionSettings_dropDown)->getChildAt(i)->getLabel();
        if(ofToUpper(path) == ofToUpper(label))
           {
               dropDownExist = true;
           }
    }
    
    if(!dropDownExist)
    {
        gui->getDropdown(projectionSettings_dropDown)->addToDropDown(path);
    }
    
    //Send event via notification center
    ofxNotificationCenter::Notification mnd;
    mnd.ID = Global::one().saveProjSetting_id;
    mnd.data["directory"] = Global::one().currentDirectory;
    
    ofxNotificationCenter::one().postNotification(Global::one().saveProjSetting_id, mnd);

}

void GUIManager::onDropDownEvent(ofxDatGuiDropdownEvent e)
{
   //ofLogNotice("GUIManager::onDropDownEvent") << gui->getChild(e.parent);
    
    
    if(e.child < gui->getDropdown(projectionSettings_dropDown)->size())
    {
        if(e.target->getLabel() == gui->getDropdown(projectionSettings_dropDown)->getChildAt(e.child)->getLabel())
        {
            string file = gui->getDropdown(projectionSettings_dropDown)->getChildAt(e.child)->getLabel();
            ofLogNotice("GUIManager::onDropDownEvent") << "Drop down " << file << " selected.";
            
            //Send event via notification center
            ofxNotificationCenter::Notification mnd;
            mnd.ID = Global::one().loadProjSetting_id;
            mnd.data["directory"] = file;
            ofxNotificationCenter::one().postNotification(Global::one().loadProjSetting_id, mnd);
            
             return;
        }
        
    }
    
    
    if(e.child < gui->getDropdown(configState)->size())
    {
        if(e.target->getLabel() == gui->getDropdown(configState)->getChildAt(e.child)->getLabel())
        {
            string state = gui->getDropdown(configState)->getChildAt(e.child)->getLabel();
            
            for(int i = 0; i < states.size(); i++)
            {
                if(states[i] == state)
                {
                    //Send event via notification center
                    ofxNotificationCenter::Notification mnd;
                    mnd.ID = Global::one().appState_id;
                    mnd.data["appState"] = i;
                    ofxNotificationCenter::one().postNotification(Global::one().appState_id, mnd);
                }
            }
            
            ofLogNotice("GUIManager::onDropDownEvent") << "Config state changed to: " << state;
            return;
        }
        
    }

}

void GUIManager::onSliderEvent(ofxDatGuiSliderEvent e)
{
    ofLogNotice("GUIManager::onSliderEvent") << "New event from " << e.target->getLabel();
    
    if(e.target->getLabel() == ofToUpper(croppingManSize_slider))
    {
        //Send event via notification center
        ofxNotificationCenter::Notification mnd;
        mnd.ID = Global::one().croppingInterfaceScale_id;
        mnd.data["percent"] = e.value;
        
        ofxNotificationCenter::one().postNotification(Global::one().croppingInterfaceScale_id, mnd);
        
        ofLogNotice("GUIManager::onSliderEvent") << "Updated cropping interface scale to " << e.value;
    }
    else if(e.target->getLabel() == ofToUpper(cropWidth))
    {
        //Send event via notification center
        ofxNotificationCenter::Notification mnd;
        mnd.ID = Global::one().cropWidth_id;
        mnd.data["width"] = e.value;
        
        ofLogNotice("GUIManager::onSliderEvent") << "Updated crop width scale to: " << e.value;
        
        ofxNotificationCenter::one().postNotification(Global::one().cropWidth_id, mnd);
        
    }
    else if(e.target->getLabel() == ofToUpper(cropHeight))
    {
        //Send event via notification center
        ofxNotificationCenter::Notification mnd;
        mnd.ID = Global::one().cropHeight_id;
        mnd.data["height"] = e.value;
        
        ofLogNotice("GUIManager::onSliderEvent") << "Updated crop height scale to: " << e.value;
        
        ofxNotificationCenter::one().postNotification(Global::one().cropHeight_id, mnd);
        
    }
    else if(e.target->getLabel() == ofToUpper(cropXpos))
    {
        //Send event via notification center
        ofxNotificationCenter::Notification mnd;
        mnd.ID = Global::one().cropXpos_id;
        mnd.data["cropXpos"] = e.value;
    
        ofLogNotice("GUIManager::onSliderEvent") << "Updated crop cropXpos scale to: " << e.value;
        
        ofxNotificationCenter::one().postNotification(Global::one().cropXpos_id, mnd);
    
    }
    else if(e.target->getLabel() == ofToUpper(cropYpos))
    {
        //Send event via notification center
        ofxNotificationCenter::Notification mnd;
        mnd.ID = Global::one().cropYpos_id;
        mnd.data["cropYpos"] = e.value;
        
        ofLogNotice("GUIManager::onSliderEvent") << "Updated crop cropYpos scale to: " << e.value;
        
        ofxNotificationCenter::one().postNotification(Global::one().cropYpos_id, mnd);
        
    }


}

#pragma mark OFXNOTIFCATION CALLBACKS
void GUIManager::onNewCropData(ofxNotificationCenter::Notification& n)
{

    float width = n.data["width"];
    float height = n.data["height"];
    float xPos = n.data["xPos"];
    float yPos = n.data["yPos"];
    
    gui->getSlider(cropWidth)->setValue(width);
    gui->getSlider(cropHeight)->setValue(height);
    gui->getSlider(cropXpos)->setValue(xPos);
    gui->getSlider(cropYpos)->setValue(yPos);
}
