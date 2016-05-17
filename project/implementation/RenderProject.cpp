#include "RenderProject.h"

/* Initialize the Project */
void RenderProject::init()
{
	bRenderer::loadConfigFile("config.json");	// load custom configurations replacing the default values in Configuration.cpp

	// let the renderer create an OpenGL context and the main window
	if(Input::isTouchDevice())
		bRenderer().initRenderer(true);										// full screen on iOS
	else
		bRenderer().initRenderer(1000, 700, false, "Underwater-3");		// windowed mode on desktop
		//bRenderer().initRenderer(View::getScreenWidth(), View::getScreenHeight(), true);		// full screen using full width and height of the screen

	// start main loop 
	bRenderer().runRenderer();
}

/* This function is executed when initializing the renderer */
void RenderProject::initFunction()
{
	// get OpenGL and shading language version
	bRenderer::log("OpenGL Version: ", glGetString(GL_VERSION));
	bRenderer::log("Shading Language Version: ", glGetString(GL_SHADING_LANGUAGE_VERSION));

	// initialize variables
	_offset = 0.0f;
	_randomOffset = 0.0f;
	_cameraSpeed = 40.0f;
	_running = false; _lastStateSpaceKey = bRenderer::INPUT_UNDEFINED;
	_viewMatrixHUD = Camera::lookAt(vmml::Vector3f(0.0f, 0.0f, 0.25f), vmml::Vector3f::ZERO, vmml::Vector3f::UP);

	// set shader versions (optional)
	bRenderer().getObjects()->setShaderVersionDesktop("#version 120");
	bRenderer().getObjects()->setShaderVersionES("#version 100");

	// load materials and shaders before loading the model
	//ShaderPtr boatShader = bRenderer().getObjects()->loadShaderFile("boat", 1, false, true, true, false, false);

	// create additional properties for a model
	PropertiesPtr cubeProperties = bRenderer().getObjects()->createProperties("cubeProperties");
	PropertiesPtr boatProperties = bRenderer().getObjects()->createProperties("boatProperties");
	PropertiesPtr groundProperties = bRenderer().getObjects()->createProperties("groundProperties");


	// load models
	bRenderer().getObjects()->loadObjModel("cube.obj", true, true, true, 4, false, false, cubeProperties);
	//bRenderer().getObjects()->loadObjModel("groundModel.obj", true, true, true, 4, false, false, groundProperties);
	//bRenderer().getObjects()->loadObjModel("boat.obj", true, true, true, 4, false, false, boatProperties);


	// create sprites
	ShaderPtr waterSurfaceShader = bRenderer().getObjects()->loadShaderFile_o("waterSurfaceShader", 0);
	MaterialPtr waterSurfaceMaterial = bRenderer().getObjects()->createMaterial("waterSurfaceMaterial", waterSurfaceShader);								// create an empty material to assign either texture1 or texture2 to
	bRenderer().getObjects()->createSprite("waterSurfaceSprite", waterSurfaceMaterial);
	
	ShaderPtr fogShader = bRenderer().getObjects()->loadShaderFile("fog", 0);
	fogShader


	//ShaderPtr groundShader = bRenderer().getObjects()->loadShaderFile_o("groundShader", 1);
	//MaterialPtr groundMaterial = bRenderer().getObjects()->createMaterial("groundMaterial", groundShader);								// create an empty material to assign either texture1 or texture2 to
	//Texture sand = "groundShader-sand.png";

	//groundMaterial->setTexture("sand", sand);
	//bRenderer().getObjects()->createSprite("groundSprite", groundMaterial);
	//PropertiesPtr groundSpriteProperties = bRenderer().getObjects()->createProperties("groundSpriteProperties");
	bRenderer().getObjects()->createSprite("groundSprite", "sand.png");
	//bRenderer().getObjects().get("groundSprite")
	





	bRenderer().getObjects()->createLight("testLight", vmml::Vector3f(0.0f, 10.f, 0.0f), vmml::Vector3f(1.f, 1.f, 1.f), 100.0f, 0.1f, 1000.0f);
	bRenderer().getObjects()->loadObjModel("Rock1.obj", false, true, false, 0, true, false);
	//bRenderer().getObjects()->loadObjModel("plane.obj", false, true, false, 0, true, false);
	bRenderer().getObjects()->loadObjModel("groundModel.obj", true, true, false, 0, true, false);

	// create camera
	bRenderer().getObjects()->createCamera("camera", vmml::Vector3f(0.0f, -2.0f, 0.0f), vmml::Vector3f(0.f, 0.f, 0.f));

	// Update render queue
	updateRenderQueue("camera", 0.0f);
}

/* Draw your scene here */
void RenderProject::loopFunction(const double &deltaTime, const double &elapsedTime)
{
//	bRenderer::log("FPS: " + std::to_string(1 / deltaTime));	// write number of frames per second to the console every frame

	//// Draw Scene and do post processing ////

	/// Draw scene ///	
	
	bRenderer().getModelRenderer()->drawQueue(/*GL_LINES*/);
	bRenderer().getModelRenderer()->clearQueue();
	

	//// Camera Movement ////
	updateCamera("camera", deltaTime);
	

	/// Update render queue ///
	updateRenderQueue("camera", deltaTime);

	// Quit renderer when escape is pressed
	if (bRenderer().getInput()->getKeyState(bRenderer::KEY_ESCAPE) == bRenderer::INPUT_PRESS)
		bRenderer().terminateRenderer();
}

/* This function is executed when terminating the renderer */
void RenderProject::terminateFunction()
{
	bRenderer::log("I totally terminated this Renderer :-)");
}

/* Update render queue */
void RenderProject::updateRenderQueue(const std::string &camera, const double &deltaTime)
{

	TouchMap touchMap = bRenderer().getInput()->getTouches();
	int i = 0;
	float rotation = 0.0f;
	for (auto t = touchMap.begin(); t != touchMap.end(); t++)
	{
		Touch touch = t->second;
		rotation = (touch.currentPositionX - touch.startPositionX) / 100;
		if (i++ > 1)
			break;
	}

	//translate and scale
	vmml::Matrix4f modelMatrix = vmml::create_translation(vmml::Vector3f(0.0f, 0.0f, 5.5f)) * vmml::create_scaling(vmml::Vector3f(0.8));
	vmml::Matrix4f rotationMatrix = vmml::create_rotation(rotation, vmml::Vector3f::UNIT_Y);
	modelMatrix *= rotationMatrix;

	bRenderer().getModelRenderer()->queueModelInstance("cube", "cube_instance", camera, modelMatrix, std::vector<std::string>({}));


	vmml::Matrix4f modelMatrix2 = vmml::create_translation(vmml::Vector3f(0.0f, 0.0f, 5971.5f)) * vmml::create_scaling(vmml::Vector3f(0.05));
	//bRenderer().getModelRenderer()->queueModelInstance("boat", "boat_instance", camera, modelMatrix2, std::vector<std::string>({}));


	vmml::Matrix4f modelMatrix3 = vmml::create_translation(vmml::Vector3f(0.0f, 100.0f, -10.0f)) * vmml::create_scaling(vmml::Vector3f(10000.0f));
	modelMatrix3 *= vmml::create_rotation(3*M_PI_2_F, vmml::Vector3f(1.0f, 0.f, 0.f));
	bRenderer().getModelRenderer()->drawModel("waterSurfaceSprite", camera, modelMatrix3, std::vector<std::string>({}), true, false);

	vmml::Matrix4f modelMatrix4 = vmml::create_translation(vmml::Vector3f(0.0f, 20.0f, -10.0f)) * vmml::create_scaling(vmml::Vector3f(1.0f));
	bRenderer().getModelRenderer()->drawModel("Rock1", camera, modelMatrix4, std::vector<std::string>({}), true, false);


	vmml::Matrix4f modelMatrix5 = vmml::create_translation(vmml::Vector3f(0.0f, -100.0f, -10.0f)) * vmml::create_scaling(vmml::Vector3f(1.0f));
	modelMatrix5 *= vmml::create_rotation(M_PI_2_F, vmml::Vector3f(1.0f, 0.f, 0.f));
	bRenderer().getModelRenderer()->drawModel("groundSprite", camera, modelMatrix5, std::vector<std::string>({}), true, false);

	vmml::Matrix4f modelMatrix6 = vmml::create_translation(vmml::Vector3f(0.0f, -100.0f, -10.0f)) * vmml::create_scaling(vmml::Vector3f(1.0f));
	//bRenderer().getModelRenderer()->drawModel("plane", camera, modelMatrix6, std::vector<std::string>({ "testLight"}), true, false);
	modelMatrix6 *= vmml::create_rotation(M_PI_2_F, vmml::Vector3f(0.0f, 1.f, 0.f));
	//bRenderer().getModelRenderer()->drawModel("groundModel", camera, modelMatrix6, std::vector<std::string>({ "testLight"}), true, false);
	bRenderer().getModelRenderer()->queueModelInstance("groundModel", "groundModel_instance", camera, modelMatrix6, std::vector<std::string>({}));
}

/* Camera movement */
void RenderProject::updateCamera(const std::string &camera, const double &deltaTime)
{
	//// Adjust aspect ratio ////
	bRenderer().getObjects()->getCamera(camera)->setAspectRatio(bRenderer().getView()->getAspectRatio());

	double deltaCameraY = 0.0;
	double deltaCameraX = 0.0;
	double cameraForward = 0.0;
	double cameraSideward = 0.0;

	/* iOS: control movement using touch screen */
	if (Input::isTouchDevice()){

		// pause using double tap
		if (bRenderer().getInput()->doubleTapRecognized()){
			_running = !_running;
		}

		if (_running){
			// control using touch
			TouchMap touchMap = bRenderer().getInput()->getTouches();
			int i = 0;
			for (auto t = touchMap.begin(); t != touchMap.end(); ++t)
			{
				Touch touch = t->second;
				// If touch is in left half of the view: move around
				if (touch.startPositionX < bRenderer().getView()->getWidth() / 2){
					cameraForward = -(touch.currentPositionY - touch.startPositionY) / 100;
					cameraSideward = (touch.currentPositionX - touch.startPositionX) / 100;

				}
				// if touch is in right half of the view: look around
				else
				{
					deltaCameraY = (touch.currentPositionX - touch.startPositionX) / 2000;
					deltaCameraX = (touch.currentPositionY - touch.startPositionY) / 2000;
				}
				if (++i > 2)
					break;
			}
		}

	}
	/* Windows: control movement using mouse and keyboard */
	else{
		// use space to pause and unpause
		GLint currentStateSpaceKey = bRenderer().getInput()->getKeyState(bRenderer::KEY_SPACE);
		if (currentStateSpaceKey != _lastStateSpaceKey)
		{
			_lastStateSpaceKey = currentStateSpaceKey;
			if (currentStateSpaceKey == bRenderer::INPUT_PRESS){
				_running = !_running;
				bRenderer().getInput()->setCursorEnabled(!_running);
			}
		}

		// mouse look
		double xpos, ypos; bool hasCursor = false;
		bRenderer().getInput()->getCursorPosition(&xpos, &ypos, &hasCursor);

		deltaCameraY = (xpos - _mouseX) / 1000;
		_mouseX = xpos;
		deltaCameraX = (ypos - _mouseY) / 1000;
		_mouseY = ypos;

		if (_running){
			// movement using wasd keys
			if (bRenderer().getInput()->getKeyState(bRenderer::KEY_W) == bRenderer::INPUT_PRESS)
				if (bRenderer().getInput()->getKeyState(bRenderer::KEY_LEFT_SHIFT) == bRenderer::INPUT_PRESS) 			cameraForward = 2.0;
				else			cameraForward = 1.0;
			else if (bRenderer().getInput()->getKeyState(bRenderer::KEY_S) == bRenderer::INPUT_PRESS)
				if (bRenderer().getInput()->getKeyState(bRenderer::KEY_LEFT_SHIFT) == bRenderer::INPUT_PRESS) 			cameraForward = -2.0;
				else			cameraForward = -1.0;
			else
				cameraForward = 0.0;

			if (bRenderer().getInput()->getKeyState(bRenderer::KEY_A) == bRenderer::INPUT_PRESS)
				cameraSideward = -1.0;
			else if (bRenderer().getInput()->getKeyState(bRenderer::KEY_D) == bRenderer::INPUT_PRESS)
				cameraSideward = 1.0;
			if (bRenderer().getInput()->getKeyState(bRenderer::KEY_UP) == bRenderer::INPUT_PRESS)
				bRenderer().getObjects()->getCamera(camera)->moveCameraUpward(_cameraSpeed*deltaTime);
			else if (bRenderer().getInput()->getKeyState(bRenderer::KEY_DOWN) == bRenderer::INPUT_PRESS)
				bRenderer().getObjects()->getCamera(camera)->moveCameraUpward(-_cameraSpeed*deltaTime);
			if (bRenderer().getInput()->getKeyState(bRenderer::KEY_LEFT) == bRenderer::INPUT_PRESS)
				bRenderer().getObjects()->getCamera(camera)->rotateCamera(0.0f, 0.0f, 0.03f*_cameraSpeed*deltaTime);
			else if (bRenderer().getInput()->getKeyState(bRenderer::KEY_RIGHT) == bRenderer::INPUT_PRESS)
				bRenderer().getObjects()->getCamera(camera)->rotateCamera(0.0f, 0.0f, -0.03f*_cameraSpeed*deltaTime);
		}
	}

	//// Update camera ////
	if (_running){
		bRenderer().getObjects()->getCamera(camera)->moveCameraForward(cameraForward*_cameraSpeed*deltaTime);
		bRenderer().getObjects()->getCamera(camera)->rotateCamera(deltaCameraX, deltaCameraY, 0.0f);
		bRenderer().getObjects()->getCamera(camera)->moveCameraSideward(cameraSideward*_cameraSpeed*deltaTime);
	}	
}

/* For iOS only: Handle device rotation */
void RenderProject::deviceRotated()
{
	if (bRenderer().isInitialized()){
		// set view to full screen after device rotation
		bRenderer().getView()->setFullscreen(true);
		bRenderer::log("Device rotated");
	}
}

/* For iOS only: Handle app going into background */
void RenderProject::appWillResignActive()
{
	if (bRenderer().isInitialized()){
		// stop the renderer when the app isn't active
		bRenderer().stopRenderer();
	}
}

/* For iOS only: Handle app coming back from background */
void RenderProject::appDidBecomeActive()
{
	if (bRenderer().isInitialized()){
		// run the renderer as soon as the app is active
		bRenderer().runRenderer();
	}
}

/* For iOS only: Handle app being terminated */
void RenderProject::appWillTerminate()
{
	if (bRenderer().isInitialized()){
		// terminate renderer before the app is closed
		bRenderer().terminateRenderer();
	}
}

/* Helper functions */
GLfloat RenderProject::randomNumber(GLfloat min, GLfloat max){
	return min + static_cast <GLfloat> (rand()) / (static_cast <GLfloat> (RAND_MAX / (max - min)));
}