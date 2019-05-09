#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    //Necessary for ofxProjectionApp
    ofDisableArbTex();
    ofSetBackgroundColor(0.0f, 0.0f, 0.0f);
    
    
    /*
     SET UP CANVAS FBO
     */
    
    //Set up FBO
    ofFbo::Settings fboSettings;
    fboSettings.width = ofGetWidth();
    fboSettings.height = ofGetHeight();
    fboSettings.useDepth = false;
    fboSettings.textureTarget = GL_TEXTURE_RECTANGLE_ARB; //mipmaps!
    fboSettings.numSamples = 8;
    fboSettings.numColorbuffers = 1;
    
    canvas = new ofFbo();
    canvas->allocate(fboSettings);
    canvas->begin();
    ofClear(0,0,0,0);
    canvas->end();
    
    /*
     SET UP SCENE FOR ENTIRE APPLICATION
     */
    scene = new ofxInterface::Node();
    scene->setSize(ofGetWidth(), ofGetHeight());
    scene->setPosition(0, 0);
    scene->setName("scene");
    scene->setEnabled(true);
    
    //Set up touch manager for scene
    ofxInterface::TouchManager::one().setup(scene, false);
    ofAddListener(ofxInterface::TouchManager::one().eventEveryTouchMove, this, &ofApp::onEveryTouch);
    ofAddListener(ofxInterface::TouchManager::one().eventEveryTouchDown, this, &ofApp::onEveryTouchDown);
    
    /*
     SET UP PROJECTION MANAGER
     */
    projections = new ofxProjectionApp();
    
    //Add projectors to projector manager
    ProjectorManager::one().addProjector(0, //order
                                         2, //numWarps
                                         ofVec2f(ofGetWidth(), ofGetHeight()), //projector resolution
                                         ofVec2f(0.0f, 0.0f)); // projector position
    
    projections->setup(canvas, //ofFbo * _canvasRef
                       false, //bool _loadFromFile
                       ofVec2f(ofGetWidth(), ofGetHeight()), //ofVec2f _appSize
                       1.0f, //float _scaleDenominator
                       scene, //ofxInterface::Node* _sceneRef
                       ""); //string _directoryPath
    

    /*
     SET UP APP STATES
     */
    vector<string> appStates;
    
    for(int i = 0; i < AppStates::NUM_STATES; i++)
    {
        string temp = getStateName(static_cast<AppStates>(i));
        appStates.push_back(temp);
    }
    
    projections->setupGuiManager(appStates);
    
    projections->setupWarps();
    
    /*
     Add ofxNotifcation Observers
     */
    ofxNotificationCenter::one().addObserver(this, &ofApp::onNewStateSelection, IDManager::one().appState_id);

}

//--------------------------------------------------------------
void ofApp::update(){
    float dt = 1./60.;
    
    //Update Scene.
    ofxInterface::TouchManager::one().update();
    
    //projections->update();
    scene->updateSubtree(1./60.);
    
    projections->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    /*
     Draw canvas to the fbo
     */
    canvas->begin();
    {
        //Draw whatever you want here!
        ofClear(0,0,0,0);
        
        ofSetColor(0.0f);
        ofDrawRectangle(0.0f, 0.0f, canvas->getWidth(), canvas->getWidth()); 
        
        ofSetColor(ofColor::pink);
        float size = 100.0f;
        ofDrawCircle(ofGetWidth()/2 - size/2, ofGetHeight()/2 - size/2, size, size);
        
    }
    canvas->end();
    
    /*
     Draw projections
     */
    
    projections->draw();
    
    /*
     Scene is just for interactive elements
     */
    scene->render();
    
    if (debug)
    {
        scene->renderDebug();
    }
}


#pragma mark CALLBACKS

#pragma mark STATE CONTROL

void ofApp::setAppState(AppStates _state)
{
    state = _state;
    
    switch(state)
    {
        case AppStates::SETUP:
        {
            ofLogNotice("ofApp::setAppState") << "Application state changed to " << "AppStates::SETUP";
            break;
        }
        case AppStates::WARP_CONFIGURATION:
        {
            projections->setMainGuiVisible(true);
            projections->setCropManagerVisible(false);
            
            ofLogNotice("ofApp::setAppState") << "Application state changed to " << "AppStates::CROP_CONFIGURATION";
            break;
        }
        case AppStates::CROP_CONFIGURATION:
        {
            projections->setMainGuiVisible(true);
            projections->setCropManagerVisible(true);
            ofLogNotice("ofApp::setAppState") << "Application state changed to " << "AppStates::WARP_CONFIGURATION";
            break;
        }
        case AppStates::ACTIVE:
        {
            projections->setMainGuiVisible(true);
            projections->setCropManagerVisible(false);
            ofLogNotice("ofApp::setAppState") << "Application state changed to " << "AppStates::ACTIVE";
            break;
        }
        default: break;
    }
}

string ofApp::getStateName(AppStates state)
{
    switch(state)
    {
        case AppStates::SETUP:
        {
            return "SETUP";
            break;
        }
        case AppStates::WARP_CONFIGURATION:
        {
            return "WARP_CONFIGURATION";
            break;
        }
        case AppStates::CROP_CONFIGURATION:
        {
            return "CROP_CONFIGURATION";
            break;
        }
        case AppStates::ACTIVE:
        {
            return "ACTIVE";
            break;
        }
        default:
        {
            return "";
            break;
        }
    }
}

#pragma mark NOTIFICATION CENTER
void ofApp::onNewStateSelection(ofxNotificationCenter::Notification& n)
{
    int newState = n.data["appState"];
    setAppState(static_cast<AppStates>(newState));
}


#pragma mark OFXINTERFACE CALLBACKS

void ofApp::onEveryTouch(ofxInterface::TouchEvent &event)
{
    
}

void ofApp::onEveryTouchDown(ofxInterface::TouchEvent &event)
{
    
}

#pragma mark OF INTERACTIONS


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    ofxInterface::TouchManager::one().touchMove(button, ofVec2f(x, y));
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    ofxInterface::TouchManager::one().touchDown(button, ofVec2f(x, y));
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    ofxInterface::TouchManager::one().touchUp(button, ofVec2f(x, y));
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
