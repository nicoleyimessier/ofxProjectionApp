
//
//  ofxProjectionApp.cpp
//  DataGuiTest
//
//  Created by Nicole Messier on 2/5/19.
//
//

ofxProjectionApp::ofxProjectionApp()
{
    
}

ofxProjectionApp::~ofxProjectionApp()
{
    
}

void ofxProjectionApp::setup(ofFbo * _canvasRef, bool _loadFromFile, string _directoryPath)
{
    warpController = new ofxWarpController();
    
    canvasRef = _canvasRef;
    canvasSize = ofVec2f(canvasRef->getTexture().getHeight(),  canvasRef->getTexture().getWidth());
    
    loadFromFile = _loadFromFile;
    directoryPath = _directoryPath;
    
    /*
     Set up listeners
     */
    ofAddListener(ofEvents().mousePressed, this, &ofxProjectionApp::onMousePressed);
    
}

void ofxProjectionApp::setupWarps()
{
    
    if(!loadFromFile)
    {
        
        //Set up projector origin that can be added to for each projector
        glm::vec2 projectorOrigin = glm::vec2(0.0f, 0.0f);
    std:shared_ptr<ofxWarpBase> warp;
        
        for(int i = 0; i < projectors.size(); i++)
        {
            projectorOrigin.x = projectors[i].size.x * i;
            
            for(int j = 0; j < projectors[i].numWarps; j++)
            {
                warp = warpController->buildWarp<ofxWarpBilinear>();
                
                warp->setSize(projectors[i].size.x/projectors[i].numWarps, projectors[i].size.y);
                
                //Edges are the alphas?
                warp->setEdges(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
                warp->setGamma(glm::vec3(100.0f, 100.0f, 100.0f));
                /*
                 //Set up control points for each warp such that the warps are distributed
                 evently across each projector. Assuming that only x pos needs to change
                 and y positions should span across the entire height of the applicaiton.
                 */
                
                /*
                 Make sure all values are normalized (i.e 0 to 1) and not the actualy pixel height and pixel width.
                 */
                // Top left control point
                
                
                float xPos_l = projectorOrigin.x + (projectors[i].size.x/projectors[i].numWarps)*j;
                xPos_l = xPos_l/AppSettings::one().appSize.x;
                xPos_l = xPos_l/AppSettings::one().scaleDenominator;
                
                float xPos_r = projectorOrigin.x + (projectors[i].size.x/projectors[i].numWarps)*(j + 1);
                xPos_r = xPos_r/AppSettings::one().appSize.x;
                xPos_r = xPos_r/AppSettings::one().scaleDenominator;
                
                
                glm::vec2 tl = glm::vec2(xPos_l, 0.0f);
                glm::vec2 tr = glm::vec2(xPos_l, 1.0f);
                glm::vec2 bl = glm::vec2(xPos_r, 0.0f);
                glm::vec2 br = glm::vec2(xPos_r, 1.0f);
                
                
                warp->setControlPoint(ControlPoints::TOP_LEFT, tl );
                warp->setControlPoint(ControlPoints::TOP_RIGHT, tr );
                warp->setControlPoint(ControlPoints::BOTTOM_LEFT,  bl );
                warp->setControlPoint(ControlPoints::BOTTOM_RIGHT, br );
                
                ofLogNotice("ofxProjectionApp") << warpController->getNumWarps() << "-xPos_l, xPos_r: " << xPos_l << " ," << xPos_r;
                
            }
            
        }
    }
    else
    {
        ofLogNotice("ofxProjectionApp::setupWarps") << "Loading from file: " << directoryPath;
        
        //Set up warpPath
        warpPath = ofFilePath::join(directoryPath, warpDataFileName);
        
        //Set up cropPath
        cropPath = ofFilePath::join(directoryPath, cropCropFileName);
        
        loadNewSettings();
        
        
    }
    
    ofLogNotice("ofxProjectionApp::setupWarps") << "Set up " << warpController->getNumWarps() << " warps.";
    
    setupEdges();
    
    /*
     Set up callbacks for notification center.
     */
    ofxNotificationCenter::one().addObserver(this, &ofxProjectionApp::onSaveSettings, Global::one().saveProjSetting_id);
    
    ofxNotificationCenter::one().addObserver(this, &ofxProjectionApp::onLoadSettings, Global::one().loadProjSetting_id);
    
    ofxNotificationCenter::one().addObserver(this, &ofxProjectionApp::onCloseEdgeBlendGui, Global::one().edgeBlendGui_id);
    
    //Debug purposes
    debugImg.load("testcard.png");
    
    setupCropData();
}

void ofxProjectionApp::setupEdges()
{
    /*
     Set up a set of edges for each warp
     */
    for(int i = 0; i < warpController->getNumWarps(); i++)
    {
        //Set up edge GUIs
        EdgeBlend *temp = new EdgeBlend();
        temp->setup(i);
        edgeGuis.push_back(temp);
        //Cycle through edges based on edge type
        
        
    }
}

void ofxProjectionApp::update()
{
    switch(guiState)
    {
        case GUIStates::EDGE_BLEND_GUI_OPEN: {
            
            
            
            break;
        }
        case GUIStates::EDGE_BLEND_GUIS_CLOSED: {break;}
        default: break;
    }
    
    //Update Edge blending params
    
    for(int i =0; i < warpController->getNumWarps(); i++)
    {
        auto warp = warpController->getWarp(i);
        
        //Edge Blending
        warp->setGamma(edgeGuis[i]->getGamma());
        warp->setEdges(edgeGuis[i]->getEdges());
        warp->setExponent(edgeGuis[i]->getExponent());
        
    }
}

void ofxProjectionApp::draw()
{
    int numWarps = warpController->getNumWarps();
    
    for(int i =0; i < numWarps; i++)
    {
        auto warp = warpController->getWarp(i);
        
        warp->begin();
        {
            
            //Get warp bounds to draw into
            auto bounds = warp->getBounds();
            
            //ofSetColor(ofColor::red);
            //ofDrawRectangle(bounds.x, bounds.y, bounds.width, bounds.height);
            
            if( i < Global::one().cropData.size() )
            {
                //(<#float x#>, <#float y#>, <#float w#>, <#float h#>, <#float sx#>, <#float sy#>, <#float sw#>, <#float sh#>)
                
                if(!debugImage)
                {
                    
                    canvasRef->getTexture().drawSubsection(bounds.x, bounds.y,
                                                           bounds.width, bounds.height,
                                                           Global::one().cropData[i].pos.x,
                                                           Global::one().cropData[i].pos.y,
                                                           Global::one().cropData[i].size.x,
                                                           Global::one().cropData[i].size.y);
                }
                else
                {
                    
                    debugImg.draw(bounds.x, bounds.y, bounds.width, bounds.height);
                    
                }
                
            }
            else
            {
                ofLogError("ofxProjectionApp::draw") << "There are more warps then cropData!";
            }
            
        }
        warp->end();
        
    }
    
}

#pragma mark PROJECTOR DATA
void ofxProjectionApp::addProjector(App::Projector projector)
{
    projectors.push_back(projector);
}

#pragma mark GUI STATES

void ofxProjectionApp::setGuiState(GUIStates _guiState)
{
    guiState = _guiState;
    
    switch(guiState)
    {
        case GUIStates::EDGE_BLEND_GUI_OPEN:
        {
            warpController->setIgnoreMouseInteractions(true);
            break;
        }
        case GUIStates::EDGE_BLEND_GUIS_CLOSED:
        {
            warpController->setIgnoreMouseInteractions(false);
            break;
        }
        default: break;
    }
}

#pragma mark CROP DATA
void ofxProjectionApp::setupCropData()
{
    
    for(int i = 0; i < Global::one().cropData.size(); i++)
    {
        ofxJSONElement data;
        data["warpID"] = ofxJSONElement();
        data["width"] = ofxJSONElement();
        data["height"] = ofxJSONElement();
        data["xPos"] = ofxJSONElement();
        data["yPos"] = ofxJSONElement();
        
        cropData.append(data);
    }
    
}

void ofxProjectionApp::clearCropData()
{
    for(int i = 0; i < Global::one().cropData.size(); i++)
    {
        cropData[i]["warpID"] = "";
        cropData[i]["width"] = "";
        cropData[i]["height"] = "";
        cropData[i]["xPos"] = "";
        cropData[i]["yPos"] = "";
    }
}

void ofxProjectionApp::saveCropData(string fileName)
{
    
    
    for(int i = 0; i < Global::one().cropData.size(); i++)
    {
        cropData[i]["warpID"] = Global::one().cropData[i].index;
        cropData[i]["width"] = Global::one().cropData[i].size.x;
        cropData[i]["height"] = Global::one().cropData[i].size.y;
        cropData[i]["xPos"] = Global::one().cropData[i].pos.x;
        cropData[i]["yPos"] = Global::one().cropData[i].pos.y;
    }
    
    ofLogNotice() << "Saving " << cropData.getRawString();
    
    cropData.save(fileName, true);
    
    
}

#pragma mark NOTIFICATION CENTER
void ofxProjectionApp::onSaveSettings(ofxNotificationCenter::Notification & n)
{
    //! Save warp settings
    string notificationID = n.ID; //the notification ID is available to you
    string directory = n.data["directory"]; //get what you need from the data field
    string fileName = ofFilePath::join(directory, warpDataFileName);
    string path = ofFilePath::join(Global::one().projectionDirectory, fileName);
    warpPath = path;
    warpController->saveSettings(warpPath);
    
    //! Save crop settings
    string cropDataFileName = ofFilePath::join(directory, cropCropFileName);
    string cropPath = ofFilePath::join(Global::one().projectionDirectory, cropDataFileName);
    saveCropData(cropPath);
    cropPath = path;
    ofLogNotice("ofxProjectionApp::onSaveSettings") << "Saving current projection settings at " << path;
}

void ofxProjectionApp::onLoadSettings(ofxNotificationCenter::Notification& n)
{
    string notificationID = n.ID; //the notification ID is available to you
    string directory = n.data["directory"]; //get what you need from the data field
    warpPath = ofFilePath::join(directory, warpDataFileName);
    cropPath = ofFilePath::join(directory, cropCropFileName);
    loadNewSettings();
    
}

#pragma mark STANDARD CALLBACKS
void ofxProjectionApp::onMousePressed(ofMouseEventArgs & args)
{
    if(args.button == OF_MOUSE_BUTTON_RIGHT)
    {
        for(int i=0; i < warpController->getNumWarps(); i++)
        {
            auto warp = warpController->getWarp(i);
            if(warp->isEditing())
            {
                edgeGuis[i]->getGuiObject()->setPosition(args.x, args.y);
                edgeGuis[i]->getGuiObject()->setVisible(true);
                setGuiState(GUIStates::EDGE_BLEND_GUI_OPEN);
            }
        }
    }
}

void ofxProjectionApp::onCloseEdgeBlendGui(ofxNotificationCenter::Notification& n)
{
    string notificationID = n.ID; //the notification ID is available to you
    int index = n.data["index"]; //get what you need from the data field
    
    ofLogNotice("ofxProjectionApp::onCloseEdgeBlendGui") << "Close edgeBlend Gui # " << index;
    
    edgeGuis[index]->getGuiObject()->setVisible(false);
    
    //Check to see if all the GUIs are closed and reset the warp controllers mouse interaction state
    
    for(auto & gui : edgeGuis)
    {
        if(gui->getGuiObject()->getVisible())
        {
            return;
        }
    }
    
    setGuiState(GUIStates::EDGE_BLEND_GUIS_CLOSED);
}

#pragma mark SAVING
void ofxProjectionApp::saveCurrentSettings()
{
    warpController->saveSettings(warpPath);
    saveCropData(cropPath);
}

#pragma mark LOADING
void ofxProjectionApp::loadNewSettings()
{
    ofLogNotice("ofxProjectionApp::loadNewSettings") << "Loading settings: " << warpPath << " and " << cropPath;
    
    warpController->loadSettings(warpPath);
    
    //! Update slider for edge blending values based on these loaded settings
    int numWarps = warpController->getNumWarps();
    
    
    //Set up edgeGuis
    for(int i=edgeGuis.size() - 1; i>=0; i--){
        
        EdgeBlend * deleteGui = edgeGuis[i];
        edgeGuis.erase(edgeGuis.begin() + i);
        delete deleteGui;
    }
    
    edgeGuis.clear();
    
    for(int i =0; i < numWarps; i++)
    {
        auto warp = warpController->getWarp(i);
        
        EdgeBlend *temp = new EdgeBlend();
        temp->setup(i);
        
        //Edge Blending
        glm::vec4 edges = warp->getEdges();
        glm::vec3 gamma = warp->getGamma();
        float exp = warp->getExponent();
        
        temp->setEdges(edges);
        temp->setGamma(gamma);
        temp->setExponent(exp);
        
        edgeGuis.push_back(temp);
        
    }
    
    ofxJSONElement newCropData;
    bool parseNewCropData = newCropData.open(cropPath);
    
    if(parseNewCropData)
    {
        try
        {
            for(int i = 0; i < newCropData.size(); i++)
            {
                if(newCropData.size() == Global::one().cropData.size())
                {
                    
                    Global::one().cropData[i].index = newCropData[i]["warpID"].asInt();
                    Global::one().cropData[i].size.x = newCropData[i]["width"].asFloat() ;
                    Global::one().cropData[i].size.y = newCropData[i]["height"].asFloat();
                    Global::one().cropData[i].pos.x = newCropData[i]["xPos"].asFloat();
                    Global::one().cropData[i].pos.y = newCropData[i]["yPos"].asFloat();
                    
                }
                else
                {
                    ofLogError("ofxProjectionApp::loadNewSettings") << cropPath << " does not have the same amount of objects as Global::one().cropData";
                }
            }
            
            
        }
        catch(exception exc)
        {
            ofLogError("ofxProjectionApp::loadNewSettings") << exc.what() << " While parsing " << newCropData.getRawString();
        }
        
        //! Notify CropManager & GUIManager to update the interfaces
        ofxNotificationCenter::Notification mnd;
        mnd.ID = Global::one().updateCropInterface_id;
        
        ofxNotificationCenter::one().postNotification(Global::one().updateCropInterface_id, mnd);
        
    }
    else
    {
        ofLogError("ofxProjectionApp::loadNewSettings") << "Unable to parse " << cropPath;
    }
    
}
