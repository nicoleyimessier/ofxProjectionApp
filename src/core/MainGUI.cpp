//
//  MainGUI.cpp
//  ProjectionApp
//
//  Created by Nicole Messier on 4/2/19.
//
//

#include "MainGUI.h"

MainGUI::MainGUI()
{
    
}

MainGUI::~MainGUI()
{
    
}

void MainGUI::setup(vector<string> &appStates, string _currentDirectory)
{
    
	currentDirectory = _currentDirectory;
    
    // instantiate and position the gui //
    gui = new ofxDatGui( ofxDatGuiAnchor::TOP_RIGHT );
    gui->setAssetPath("");
    gui->setPosition(0.0f, 0.0f);
    
    /*
     Add text component for directory
     */
    gui->addTextInput(dirName_input, "Enter directory");
    gui->onTextInputEvent(this, &MainGUI::onTextInputEvent);
    
    /*
     Add button for saving new mapping directory
     */
    gui->addButton(saveMapping_button);
    gui->onButtonEvent(this, &MainGUI::onButtonEvent);
    
    /*
     Add drop down menu for projection settings
     */
    
    if(!ofDirectory::doesDirectoryExist(projectionDirectory))
    {
        //Create directory
        ofDirectory::createDirectory(projectionDirectory); 
    }

    ofDirectory dir;
    dir.listDir(projectionDirectory);
    int numFiles = dir.size();
    vector<string> paths;
    
    for(int i = 0; i < numFiles; i++)
    {
 
        string path = dir.getPath(i);

		ofLogNotice("MainGUI") << "Setting up " << path << " in drop down.";

        paths.push_back(path);
    }
    
    gui->onDropdownEvent(this, &MainGUI::onDropDownEvent);
    gui->addDropdown(projectionSettings_dropDown, paths);
    
    /*
     Add slider for cropping manager (i.e. cropping interface) scale
     */
    gui->addSlider(croppingManSize_slider, 0.0f, 1.0f, 0.5f);
    gui->addSlider(cropWidth, 0.0f, 1.0f, 1.0f);
    gui->addSlider(cropHeight, 0.0f, 1.0f, 1.0f);
    gui->addSlider(cropXpos, 0.0f, 1.0f, 1.0f);
    gui->addSlider(cropYpos, 0.0f, 1.0f, 1.0f);
    gui->onSliderEvent(this, &MainGUI::onSliderEvent);
    
    
    /*
     Add drop down for each state
     */
    states = appStates;
    gui->addDropdown(configState, appStates);
    
    /*
     Add ofxNotifcation Observers
     */
    ofxNotificationCenter::one().addObserver(this, &MainGUI::onNewCropData, IDManager::one().crop_startInfo_id);
    
}

void MainGUI::update()
{
    
}

void MainGUI::draw()
{
    
}

#pragma mark GUI OBJECT

ofxDatGui * MainGUI::getGui()
{
    return gui;
}

void MainGUI::toggleGuiVisiblity()
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
float MainGUI::getCropSize()
{
    return gui->getSlider(croppingManSize_slider)->getValue();
}

#pragma mark CALLBACKS

void MainGUI::onTextInputEvent(ofxDatGuiTextInputEvent e)
{
    ofLogNotice("MainGUI::onTextInputEvent") << "onTextInputEvent: " << e.target->getLabel() << " " << e.target->getText();
    
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
        
        ofLogNotice("MainGUI::onTextInputEvent") << "Reformated \" " << input << "\" to " << directory;
    }
    
    currentDirectory = directory;
    
}

void MainGUI::onButtonEvent(ofxDatGuiButtonEvent e)
{
    ofLogNotice("MainGUI::onButtonEvent") << "Button " << e.target->getLabel() << " pressed";
    
    //Add to the drop down menu
    string path = ofFilePath::join(projectionDirectory, currentDirectory);
    
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
    mnd.ID = IDManager::one().saveProjSetting_id;
    mnd.data["directory"] = path;
    
    ofxNotificationCenter::one().postNotification(IDManager::one().saveProjSetting_id, mnd);
    
}

void MainGUI::onDropDownEvent(ofxDatGuiDropdownEvent e)
{
    //ofLogNotice("MainGUI::onDropDownEvent") << gui->getChild(e.parent);
    
    
    if(e.child < gui->getDropdown(projectionSettings_dropDown)->size())
    {
        if(e.target->getLabel() == gui->getDropdown(projectionSettings_dropDown)->getChildAt(e.child)->getLabel())
        {
            string file = gui->getDropdown(projectionSettings_dropDown)->getChildAt(e.child)->getLabel();
            ofLogNotice("MainGUI::onDropDownEvent") << "Drop down " << file << " selected.";
            
            //Send event via notification center
            ofxNotificationCenter::Notification mnd;
            mnd.ID = IDManager::one().loadProjSetting_id;
            mnd.data["directory"] = file;
            ofxNotificationCenter::one().postNotification(IDManager::one().loadProjSetting_id, mnd);
            
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
                    mnd.ID = IDManager::one().appState_id;
                    mnd.data["appState"] = i;
                    ofxNotificationCenter::one().postNotification(IDManager::one().appState_id, mnd);
                }
            }
            
            ofLogNotice("MainGUI::onDropDownEvent") << "Config state changed to: " << state;
            return;
        }
        
    }
    
}

void MainGUI::onSliderEvent(ofxDatGuiSliderEvent e)
{
    ofLogNotice("MainGUI::onSliderEvent") << "New event from " << e.target->getLabel();
    
    if(e.target->getLabel() == ofToUpper(croppingManSize_slider))
    {
        //Send event via notification center
        ofxNotificationCenter::Notification mnd;
        mnd.ID = IDManager::one().croppingInterfaceScale_id;
        mnd.data["percent"] = e.value;
        
        ofxNotificationCenter::one().postNotification(IDManager ::one().croppingInterfaceScale_id, mnd);
        
        ofLogNotice("MainGUI::onSliderEvent") << "Updated cropping interface scale to " << e.value;
    }
    else if(e.target->getLabel() == ofToUpper(cropWidth))
    {
        //Send event via notification center
        ofxNotificationCenter::Notification mnd;
        mnd.ID = IDManager::one().cropWidth_id;
        mnd.data["width"] = e.value;
        
        ofLogNotice("MainGUI::onSliderEvent") << "Updated crop width scale to: " << e.value;
        
        ofxNotificationCenter::one().postNotification(IDManager::one().cropWidth_id, mnd);
        
    }
    else if(e.target->getLabel() == ofToUpper(cropHeight))
    {
        //Send event via notification center
        ofxNotificationCenter::Notification mnd;
        mnd.ID = IDManager::one().cropHeight_id;
        mnd.data["height"] = e.value;
        
        ofLogNotice("MainGUI::onSliderEvent") << "Updated crop height scale to: " << e.value;
        
        ofxNotificationCenter::one().postNotification(IDManager::one().cropHeight_id, mnd);
        
    }
    else if(e.target->getLabel() == ofToUpper(cropXpos))
    {
        //Send event via notification center
        ofxNotificationCenter::Notification mnd;
        mnd.ID = IDManager::one().cropXpos_id;
        mnd.data["cropXpos"] = e.value;
        
        ofLogNotice("MainGUI::onSliderEvent") << "Updated crop cropXpos scale to: " << e.value;
        
        ofxNotificationCenter::one().postNotification(IDManager::one().cropXpos_id, mnd);
        
    }
    else if(e.target->getLabel() == ofToUpper(cropYpos))
    {
        //Send event via notification center
        ofxNotificationCenter::Notification mnd;
        mnd.ID = IDManager::one().cropYpos_id;
        mnd.data["cropYpos"] = e.value;
        
        ofLogNotice("MainGUI::onSliderEvent") << "Updated crop cropYpos scale to: " << e.value;
        
        ofxNotificationCenter::one().postNotification(IDManager::one().cropYpos_id, mnd);
        
    }
    
    
}

#pragma mark OFXNOTIFCATION CALLBACKS

void MainGUI::onNewCropData(ofxNotificationCenter::Notification& n)
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

#pragma mark DIRECTORY 

string MainGUI::getCurrentDirectory()
{
    return currentDirectory;
}
