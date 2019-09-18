# ofxProjectionApp Overview

ofxProjectionApp is a basic template for a projection mapping system using a variety of different addons to simplify getting a projection mapping system up quickly. 

The intended process is to draw your app into an FBO that acts as your canvas, and once you are ready to projection, use this setup. Note that the application does not do any scaling; therefore, if you tell the application to draw three warps at 4k and run your application at HD, you will not see everything.

**NOTE** Much of this documentaiton follows the advanced example. 

## Feature List / Addons Needed

* [ofxWarp](https://github.com/local-projects/ofxWarp): create editable linear, bi-linear, and perspective warps. Note this is using the local-projects fork of [prisonerjohn's addon](https://github.com/prisonerjohn/ofxWarp) and the [feature/multipleControls](https://github.com/local-projects/ofxWarp/tree/feature/multipleControls) branch. The feature/multipleControls branch is working for bi-linear warps, but the new features still need to be cascaded to to the linear and perspective warp classes. New features include: clicking control points to toggle active state instead of hovering, controling multiple control points at once
* [ofxDatGui](https://github.com/local-projects/ofxDatGui): In application GUI 
* [ofxJSON](https://github.com/local-projects/ofxJSON): configurable json files
* [ofxNotificationCenter](https://github.com/local-projects/ofxNotificationCenter): addon to send and receive messages to a an application wide
* Saves and loads projection settings dynamically

## Compatibility
* openFrameworks 0.9 and up
* OpenGL 3 and up (programmable pipeline)
* The included shaders only work with normalized textures (GL_TEXTURE_2D) but can be easily modified to work with rectangle textures 

## Class Overview
ofxProjectionApp is more of a template then a singular addon with one function; this results in a lot of moving parts. Below is a WIP diagram of the overall class structure.

![test](https://github.com/nicoleyimessier/ofxProjectionApp/blob/master/Documentation/Diagrams/HighLevelClasses.png)

* `ofxProjectionApp`: overall projection manager 
* `MainGUI`: GUI used to save settings, load settings, change state, and adjust crop settings
* `CanvasRef`: a pointer to the FBO that you should draw your canvas or scene into
* `WarpController`: manager for all warps
* `CroppingManager`: cropping manager that displays scaled versions of the canvas and crops within each warp
* `EdgeBlend`: each warp has it's own edge blend GUI that is divorced from the warpcontroller 

Because many of these classes need the same information, there are a number of singletons in the application:

* `IDManager`: holdes IDs that are used in `ofxNotificationCenter` and are needed across multiple 
* `ProjectorManager`: holds all of the projector specs including order, resolution, number of warps. This information is used to setup information in the `CroppingManager` and other parts of the codebase. 


**NOTE:** There are two ways the ProjectorManager gets set up:

1. If you load in preconfigured projection settings on startup, the ProjectorManager will automatically set up. 
2. If you are starting without any preconfigured projection settings (which will be the case before you set your rig up), you need to set up the ProjectorManager yourself. 

## Intended Workflow

The intended workflow is to have a one to one setup of your intended system. From there the steps should be: 

1. Set up `ProjectorManager` with the appropriate number of warps and projectors.
2. Load projector settings that represent a one to one setup of the actual rig.
3. Adjust the crops that are drawn in each warp.
4. Adjust the control points and edge blend settings of each warp.
5. Save files.
6. Once steps 1-5 are complete, you can simply load in your preconfigured projection settings on app start up.

## Adjust Crops

1. To adjust the crop of the canvas drawn in a specific warp, click on the drop down menu ‘Configuration State’ and click `Crop Configuration`.
2.  A new GUI will pop up that may be too big to see, you can change the size of the GUI by adjusting the `CROPPING INTERFACE` slider. Below is an image of the GUI. 

 ![test](https://github.com/nicoleyimessier/ofxProjectionApp/blob/master/Documentation/SoftwareImages/CroppingState.png)
 
 3. To edit a specific warp, click on the warp visual and it will highlight. 
 4. Once a warp is highlighted, use the GUI variables to edit the size and position of the crop. To adjust the size and position of the crop within the canvas, use the sliders in the main GUI labeled “CROP WIDTH”, “CROP HEIGHT”, “CROP XPOS”, and “CROP YPOS”. Note that the red rectangle represents the crop of the canvas that is drawn into the warp.
 5. To adjust the position of the crop within the warp, change the sliders corresponding to “X POS OF CROP IN WARP” and “Y POS OF CROP IN WARP”


 ![test](https://github.com/nicoleyimessier/ofxProjectionApp/blob/master/Documentation/SoftwareImages/CroppingState2.png)
 

## Editing Warps

1. Click on the configuration state “WARP CONFIGURATION”
2. Press ‘w’ which toggles warp editing on and off. This will come in handy when you have a lot of points to edit in the mesh. 
3. To select a warp, press alt + left-click over the warp. Each warp has a default control point at each corner.
4. Press F1 to reduce the number of horizontal control points
5. Press F2 to increase the number of horizontal control points
6. Press F3 to reduce the number of vertical control points
7. Press F4 to increase the number of vertical control points
8. To move a control point, click on it and it will turn green. Note that you can activate multiple control points to move at once. 
9. To unselect a control point, click on it while holding shift. 

 ![test](https://github.com/nicoleyimessier/ofxProjectionApp/blob/master/Documentation/SoftwareImages/ControlPoint.png)
 
## Edge Blending
 
Each warp has its own edge blending panel. 

To open the panel: 

1. Click on the configuration state “WARP CONFIGURATION”
2. Press ‘w’ which toggles warp editing on and off. 
3. To select a warp, press alt + left-click over the warp. 
4. Right click on the warp and a new GUI will appear with the controls for edge blending.  

 ![test](https://github.com/nicoleyimessier/ofxProjectionApp/blob/master/Documentation/SoftwareImages/EdgeBlend.png)

## Saving Projection Settings

To save new settings: 

1. Enter the filename in the MAIN GUI under “PROJECTION SETTINGS” and click save settings. 


## Loading Projection Settings

To load old settings: 

1. Click the drop down under “PROJECTION SETTINGS” and click on the desired file. 
2. You can also pass in your own file path to load specific settings onstart up. Following the Advavanced example, edit the AppSettings.json file to load a specific file on startup by editing: 

```
"loadFromFile": true,
"directoryPath": "projections/TEST"
```

**NOTE:** you can currently add warps to your initial setup, but you cannot remove any. For example, if you start with one projector and 1 warp, you can then load in a file with 4 warps, this will work. However, you can not start with a setup that has 4 warps and load in a file with 2 warps. "Removing Warps" is in the added features.



## Quick Setup Guide 
	
1. Set open gl version in `int(){}`. This needs to be OpenGL 3 and up.

	```
	winSettings.setGLVersion(3, 2);
	```
2. Call `ofDisableArbTex()` in `ofApp::setup()`
3. Create an `ofxFbo` object to draw in. This will get passed to the projection manager.

	```
	\\ In Header
	ofFbo *canvas;
	
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
	``` 
4. Create your scene using `ofxInterface`.

	```
	 ofxInterface::Node* scene;
	  scene = new ofxInterface::Node();
    scene->setSize(ofGetWidth(), ofGetHeight());
    scene->setPosition(0, 0);
    scene->setName("scene");
    scene->setEnabled(true);
	``` 
	
5. Create an `ofxProjectionApp` object which is essentialy going to be the projection manager.

	```
	// In Header
	ofxProjectionApp * projections;
	
	// In cpp file
	projections = new ofxProjectionApp();
	```

6.  Set up the projector specs.

	```
	// In cpp file
	ProjectorManager::one().addProjector(0, //order
                                         2, //numWarps
                                         ofVec2f(ofGetWidth(), ofGetHeight()), //projector resolution
                                         ofVec2f(0.0f, 0.0f)); // projector position
	```
7. Finish the setup for `ofxProjectionApp`.

	```
    projections->setup(canvas, //ofFbo * _canvasRef
                       false, //bool _loadFromFile
                       ofVec2f(ofGetWidth(), ofGetHeight()), //ofVec2f _appSize
                       1.0f, //float _scaleDenominator
                       scene, //ofxInterface::Node* _sceneRef
                       ""); //string _directoryPath
	```

8. Add any applicaiton states and set up the GUI manager with a vector of those states in string format. State management is flexible, so it's really up to you on how you want to use it. See advanced example for templated version.
	
	```
   vector<string> appStates;
    
    for(int i = 0; i < AppStates::NUM_STATES; i++)
    {
        string temp = getStateName(static_cast<AppStates>(i));
        appStates.push_back(temp);
    }
    
    projections->setupGuiManager(appStates);
    ```
9.  Set up warps for `ofxProjectionApp`
	
	```
	projections->setupWarps();
	```
10. Set up the `TouchManager` for `ofxInterface`
	```
	ofxInterface::TouchManager::one().setup();
	```
11. Update the scene, `ofxProjectionApp` object, and `TouchManager` in `ofApp::update()`
	
	```
	float dt = 1./60.;
    
    //Update Scene.
    ofxInterface::TouchManager::one().update();
    
    //projections->update();
    scene->updateSubtree(1./60.);
    
    projections->update();
	```	 
13.  Draw your content in canvas
	
	```
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
    ```

## Key Controls

* `w`: toggles warp editing mode
* `alt` + `left-click`: selects warp
* `left-click` on selected warp: selects closest control point, note that you can click on multiple control points at once ad shift them
* `shift` + `left-click`: deselects control point
* `right-click` on warp: brings up edge blend gui

Once you are in warp editing mode: 

For Bilinear warps only:

* `m`: to toggle between linear and curved mapping
* `F1`: to reduce the number of horizontal control points
* `F2`: to increase the number of horizontal control points
* `F3`: to reduce the number of vertical control points
* `F4`: to increase the number of vertical control points
* `F5`: to decrease the mesh resolution
* `F6`: to increase the mesh resolution

## Future Features

* Remove warps on the fly
