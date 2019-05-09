#include "ofApp.h"


void ofApp::setup(){
    
    /*
     Set up app settings
     */
    AppSettings::one().setup();
    ofSetWindowShape(AppSettings::one().appSize.x, AppSettings::one().appSize.y);
    
    //Necessary for ofxProjectionApp
    ofDisableArbTex();
    
    
    /*
     Set up scene for entire application.
     */
    scene = new ofxInterface::Node();
    scene->setSize(AppSettings::one().appSize.x, AppSettings::one().appSize.y);
    scene->setPosition(0, 0);
    scene->setName("scene");
    scene->setEnabled(true);

    //Set up touch manager for scene
    ofxInterface::TouchManager::one().setup(scene, false);
    ofAddListener(ofxInterface::TouchManager::one().eventEveryTouchMove, this, &ofApp::onEveryTouch);
    ofAddListener(ofxInterface::TouchManager::one().eventEveryTouchDown, this, &ofApp::onEveryTouchDown);
    
    /*
     Set up Canvas Manager
     */
    canvasMan = new CanvasManager();
    canvasMan->setup(AppSettings::one().canvasSize);
    
    
    projections = new ofxProjectionApp();
    
    
    //setup projcetion manager
    for(int i = 0; i < AppSettings::one().projectors.size(); i++)
    {
        ProjectorManager::one().addProjector(AppSettings::one().projectors[i].order,
                                             AppSettings::one().projectors[i].numWarps,
                                             AppSettings::one().projectors[i].size,
                                             AppSettings::one().projectors[i].pos);
    }
    
    projections->setup(canvasMan->getCanvas(), AppSettings::one().loadWarpSettingsFromFile, AppSettings::one().appSize, AppSettings::one().scaleDenominator, scene, AppSettings::one().warpDirectoryPath);
    
    /*
     Set up ofxDatGui manager
     */
    //guiMan  = new GUIManager();
    vector<string> appStates;
    
    for(int i = 0; i < App::AppStates::NUM_STATES; i++)
    {
        string temp = getStateName(static_cast<App::AppStates>(i));
        appStates.push_back(temp);
    }
    
    
    
    projections->setupGuiManager(appStates);
    
    projections->setupWarps();
    
    CustomApp::setup();
    
    /*
     Add ofxNotifcation Observers
     */
    ofxNotificationCenter::one().addObserver(this, &ofApp::onNewStateSelection, IDManager::one().appState_id);
    
    // LISTENERS
    ofAddListener(screenSetup.setupChanged, this, &ofApp::setupChanged);
    ofAddListener(RUI_GET_OF_EVENT(), this, &ofApp::remoteUIClientDidSomething);
}


void ofApp::update(){

	float dt = 1./60.;

    //Update Scene.
    ofxInterface::TouchManager::one().update();
    
    //projections->update();
    scene->updateSubtree(1./60.);
    
    projections->update();
}


void ofApp::draw(){
    
    /*
     Draw canvas to the fbo
     */
    canvasMan->draw();
    
    /*
     Draw projections
     */
    
    projections->draw();
    
    /*
     Scene is just for interactive elements
     */
    scene->render();
    
    if (Global::one().debug)
    {
        scene->renderDebug();
    }

}

#pragma mark STATE CONTROL
void ofApp::setAppState(App::AppStates _state)
{
    state = _state;
    
    switch(state)
    {
        case App::AppStates::SETUP:
        {
            ofLogNotice("ofApp::setAppState") << "Application state changed to " << "AppStates::SETUP";
            break;
        }
        case App::AppStates::WARP_CONFIGURATION:
        {
            projections->setMainGuiVisible(true);
            projections->setCropManagerVisible(false);
            
            ofLogNotice("ofApp::setAppState") << "Application state changed to " << "AppStates::CROP_CONFIGURATION";
            break;
        }
        case App::AppStates::CROP_CONFIGURATION:
        {
            projections->setMainGuiVisible(true);
            projections->setCropManagerVisible(true);
            ofLogNotice("ofApp::setAppState") << "Application state changed to " << "AppStates::WARP_CONFIGURATION";
            break;
        }
        case App::AppStates::ACTIVE:
        {
            projections->setMainGuiVisible(true);
            projections->setCropManagerVisible(false);
            ofLogNotice("ofApp::setAppState") << "Application state changed to " << "AppStates::ACTIVE";
            break;
        }
        default: break;
    }
}

string ofApp::getStateName(App::AppStates state)
{
    switch(state)
    {
        case App::AppStates::SETUP:
        {
            return "SETUP";
            break;
        }
        case App::AppStates::WARP_CONFIGURATION:
        {
            return "WARP_CONFIGURATION";
            break;
        }
        case App::AppStates::CROP_CONFIGURATION:
        {
            return "CROP_CONFIGURATION";
            break;
        }
        case App::AppStates::ACTIVE:
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

#pragma mark ofxNOTIFCATION

#pragma mark NOTIFICATION CENTER
void ofApp::onNewStateSelection(ofxNotificationCenter::Notification& n)
{
    int newState = n.data["appState"];
    setAppState(static_cast<App::AppStates>(newState));
}

#pragma mark OFXINTERFACE CALLBACKS

void ofApp::onEveryTouch(ofxInterface::TouchEvent &event)
{
    
}

void ofApp::onEveryTouchDown(ofxInterface::TouchEvent &event)
{
    
}

#pragma mark MOUSE/KEY INTERACTIONS


void ofApp::keyPressed(int key){

	switch (key)
	{

	case ('d'):
	{
		Global::one().debug = !Global::one().debug;
		break;
	}
	case ('f'):
	{
		ofToggleFullscreen();
		break;
	}
	case ('L'):
	{
		ofxSuperLog::getLogger()->setScreenLoggingEnabled(!ofxSuperLog::getLogger()->isScreenLoggingEnabled());
		break;
	}
	case ('S'):
	{
		screenSetup.cycleToNextScreenMode();
		break;
	}
	default: break;
	}
}


void ofApp::keyReleased(int key){

}


void ofApp::mouseMoved(int x, int y ){

}


void ofApp::mouseDragged(int x, int y, int button){
    ofxInterface::TouchManager::one().touchMove(button, ofVec2f(x, y));
}


void ofApp::mousePressed(int x, int y, int button){
    ofxInterface::TouchManager::one().touchDown(button, ofVec2f(x, y));
}


void ofApp::mouseReleased(int x, int y, int button){
    ofxInterface::TouchManager::one().touchUp(button, ofVec2f(x, y));
}


void ofApp::windowResized(int w, int h){
    ofLogNotice("ofApp::windowResized") << "Resizing window to (w,h): " << w << "," << h;
}

void ofApp::gotMessage(ofMessage msg){

}


void ofApp::dragEvent(ofDragInfo dragInfo){
	
}


#pragma mark CALLBACKS

void ofApp::setupChanged(ofxScreenSetup::ScreenSetupArg &arg){
	ofLogNotice()	<< "ofxScreenSetup setup changed from " << screenSetup.stringForMode(arg.oldMode)
	<< " (" << arg.oldWidth << "x" << arg.oldHeight << ") "
	<< " to " << screenSetup.stringForMode(arg.newMode)
	<< " (" << arg.newWidth << "x" << arg.newHeight << ")";
}


//define a callback method to get notifications of client actions
void ofApp::remoteUIClientDidSomething(RemoteUIServerCallBackArg &arg){
	switch (arg.action) {
		case CLIENT_CONNECTED: cout << "CLIENT_CONNECTED" << endl; break;
		case CLIENT_DISCONNECTED: cout << "CLIENT_DISCONNECTED" << endl; break;
		case CLIENT_UPDATED_PARAM: cout << "CLIENT_UPDATED_PARAM: "<< arg.paramName << " - ";
			arg.param.print();
			break;
		case CLIENT_DID_SET_PRESET: cout << "CLIENT_DID_SET_PRESET" << endl; break;
		case CLIENT_SAVED_PRESET: cout << "CLIENT_SAVED_PRESET" << endl; break;
		case CLIENT_DELETED_PRESET: cout << "CLIENT_DELETED_PRESET" << endl; break;
		case CLIENT_SAVED_STATE: cout << "CLIENT_SAVED_STATE" << endl; break;
		case CLIENT_DID_RESET_TO_XML: cout << "CLIENT_DID_RESET_TO_XML" << endl; break;
		case CLIENT_DID_RESET_TO_DEFAULTS: cout << "CLIENT_DID_RESET_TO_DEFAULTS" << endl; break;
		default:
			break;
	}
}
