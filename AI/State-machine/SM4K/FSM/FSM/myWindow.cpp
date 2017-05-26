#include "myWindow.h"
#include "Text2D.h"
#include "entityManager.h"

//Standard functions ****************************************

int myWindow::initializeGL() {
	//Initialize GLFW 
	if( !glfwInit() ) {
		fprintf( stderr, "Failed to initialize GLFW");
		return -1;
	}

	return 0;
}


int myWindow::createWindow() {
	glfwWindowHint(GLFW_SAMPLES, 4); //Antialiasing 4x
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //OpenGL 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Try to create a window, if failed, abort and return -1
	glfwWindow = glfwCreateWindow(windowWidth, windowHeight, "Sprit motor 4K", NULL, NULL);
	if( glfwWindow == NULL ) {
		fprintf(stderr, "Failed to create window" );
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(glfwWindow);
	glewExperimental = true;
	if ( glewInit() != GLEW_OK ) {
		fprintf(stderr, "GLEW failed to initialise");
		return -1;
	}

	//Capture keys
	glfwSetInputMode(glfwWindow, GLFW_STICKY_KEYS, GL_TRUE);
	//glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);		//SHOW/HIDE CURSOR!

	return 0;
}



void myWindow::initializeScene() {
	//Do everthing in order here instead of calling in main

	//Create projection matrix to be used
	Projection = glm::ortho(0.0f, windowWidth, windowHeight, 0.0f);
	
	//Set's the default speed to standard speed
	speed = 1;

	installShaders();
	//Use installed shader
	glUseProgram(programID);
	//Start running game loop
	gameLoop();
}

void myWindow::installShaders() {
	programID = LoadShaders("vertexShader.gsl", "fragmentShader.gsl");
	textShaderID = LoadShaders("TextVertexShader.gsl", "TextFragmentShader.gsl");
	//debugProgramID = LoadShaders("debugVertexShader.glsl", "debugFragmentShader.glsl");
}

void myWindow::gameLoop() {

	Text2D textPrinter;
	textPrinter.initText2D(textShaderID, "font.DDS", windowWidth, windowHeight);

	//The objects for information
	Model infoObj = shapeGenerator::square(250, 140, programID);
	Object infoObject = Object(glm::vec3(0.5f, 0.5f, 0.5f));
	infoObject.setModel(&infoObj);
	infoObject.setPosition(0, 0);
	objects.push_back(&infoObject);

	Model infoObj2 = shapeGenerator::square(50, 50, programID);
	Object infoObject2 = Object(glm::vec3(0.5f, 0.5f, 0.5f));
	infoObject2.setModel(&infoObj2);
	infoObject2.setPosition(250, 0);
	objects.push_back(&infoObject2);

	Model infoObj3 = shapeGenerator::triangle(50, 50, programID);
	infoObject3 = Object(glm::vec3(0.0f, 0.5f, 0.5f));
	infoObject3.setModel(&infoObj3);
	infoObject3.setPosition(250, 0);
	objects.push_back(&infoObject3);

	//The different buildings
	Model homeObj = shapeGenerator::square(75, 75, programID);
	Object HomeObject = Object(glm::vec3(1.0f, 1.0f, 1.0f));
	HomeObject.setModel(&homeObj);
	HomeObject.setPosition(400, 100);
	objects.push_back(&HomeObject);

	Model work1Obj = shapeGenerator::square(75, 75, programID);
	Object work1Object = Object(glm::vec3(1.0f, 0.3f, 0.3f));
	work1Object.setModel(&work1Obj);
	work1Object.setPosition(500, 500);
	objects.push_back(&work1Object);

	Model work2Obj = shapeGenerator::square(75, 75, programID);
	Object work2Object = Object(glm::vec3(1.0f, 0.5f, 0.5f));
	work2Object.setModel(&work2Obj);
	work2Object.setPosition(650, 550);
	objects.push_back(&work2Object);
	
	Model home2Obj = shapeGenerator::square(75, 75, programID);
	Object homeObject2 = Object(glm::vec3(1.0f, 1.0f, 1.0f));
	homeObject2.setModel(&home2Obj);
	homeObject2.setPosition(650, 100);
	objects.push_back(&homeObject2);

	Model restaurantObj = shapeGenerator::square(75, 75, programID);
	Object restaurantObject = Object(glm::vec3(0.3f, 0.3f, 1.0f));
	restaurantObject.setModel(&restaurantObj);
	restaurantObject.setPosition(250, 340);
	objects.push_back(&restaurantObject);

	Model pubObj = shapeGenerator::square(75, 75, programID);
	Object pubObject = Object(glm::vec3(0.6f, 0.6f, 1.0f));
	pubObject.setModel(&pubObj);
	pubObject.setPosition(280, 550);
	objects.push_back(&pubObject);

	Model shopObj = shapeGenerator::square(75, 75, programID);
	Object shopObject = Object(glm::vec3(0.3f, 1.0f, 0.3f));
	shopObject.setModel(&shopObj);
	shopObject.setPosition(680, 320);
	objects.push_back(&shopObject);

	Model graveyardObj = shapeGenerator::square(75, 75, programID);
	Object graveyardObject = Object(glm::vec3(0.3f, 0.3f, 0.3f));
	graveyardObject.setModel(&graveyardObj);
	graveyardObject.setPosition(30, 670);
	objects.push_back(&graveyardObject);

	//Bob
	Model triangle = shapeGenerator::triangle(30, 35, programID);
	triangleObject = new Unit(glm::vec3(1.0f, 0.0f, 0.0f), 0);
	triangleObject->setModel(&triangle);
	triangleObject->setPosition(500, 100);
	objects.push_back(triangleObject);
	entityMgr->registerUnit(triangleObject);

	//Peter
	Model triangle2 = shapeGenerator::triangle(30, 35, programID);
	triangleObject2 = new Unit(glm::vec3(0.0f, 0.0f, 1.0f), 1);
	triangleObject2->setModel(&triangle2);
	triangleObject2->setPosition(500, 100);
	objects.push_back(triangleObject2);
	entityMgr->registerUnit(triangleObject2);

	//Susan
	Model triangle3 = shapeGenerator::triangle(30, 35, programID);
	triangleObject3 = new Unit(glm::vec3(0.0f, 1.0f, 1.0f), 2);
	triangleObject3->setModel(&triangle3);
	triangleObject3->setPosition(500, 100);
	objects.push_back(triangleObject3);
	entityMgr->registerUnit(triangleObject3);

	//Kim
	Model triangle4 = shapeGenerator::triangle(30, 35, programID);
	triangleObject4 = new Unit(glm::vec3(0.0f, 0.7f, 0.0f), 3);
	triangleObject4->setModel(&triangle4);
	triangleObject4->setPosition(500, 100);
	objects.push_back(triangleObject4);
	entityMgr->registerUnit(triangleObject4);

	//Time variables
	double lastTime = glfwGetTime();
	updateFrequency = 1.0f;
	pause = false;

	while (glfwWindowShouldClose(glfwWindow) == 0)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Calculate deltaTime
		double currentTime = glfwGetTime();
		deltaTime = fabs(currentTime - lastTime);

		//Draw and Update all objects
		for (int i = 0; i < objects.size(); i++)
		{

			objects.at(i)->update(updateFrequency*speed);		
			objects.at(i)->draw(Projection);
			
		}
		
		glUseProgram(textShaderID);

		textPrinter.printText2D("Home 1", 390, 180, 18);
		textPrinter.printText2D("Home 2", 640, 180, 18);

		textPrinter.printText2D("Work 1", 490, 580, 18);
		textPrinter.printText2D("Work 2", 640, 630, 18);

		textPrinter.printText2D("Restaurant", 220, 420, 18);
		textPrinter.printText2D("Pub", 290, 630, 18);
		textPrinter.printText2D("Shop", 680, 400, 18);

		textPrinter.printText2D("Graveyard", 0, 750, 18);


		textPrinter.printText2D("Name: "+unitName, 0, 10, 14);
		textPrinter.printText2D("Fatigue: " + unitFatigue, 0, 30, 14);
		textPrinter.printText2D("Money: " + unitMoney, 0, 50, 14);
		textPrinter.printText2D("Hunger: " + unitHunger, 0, 70, 14);
		textPrinter.printText2D("Thirst: " + unitThirst, 0, 90, 14);
		textPrinter.printText2D("SocialNeed: " + unitSocialNeed, 0, 110, 14);
		
		glUseProgram(programID);
		glfwSwapBuffers(glfwWindow);
		glfwPollEvents();
		
		handleEvents();

		//Set lastTime to currentTime!
		lastTime = currentTime;
	}
}

void myWindow::handleEvents() {
	if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT) == GLFW_PRESS) {
		speed += 0.05f;
		if (speed > 1)
		{
			speed = 1;
		}

	}if (glfwGetKey(glfwWindow, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		speed -= 0.05f;
		if (speed < 0.01f)
		{
			speed = 0.01f;
		}
	}if (glfwGetKey(glfwWindow, GLFW_KEY_1) == GLFW_PRESS) {
		infoObject3.setColor(triangleObject->getColor());
		unitName = triangleObject->getName();
		unitFatigue = std::to_string(triangleObject->getFatigue());
		unitMoney = std::to_string(triangleObject->moneyCarried());
		unitHunger = std::to_string(triangleObject->getHunger());
		unitThirst = std::to_string(triangleObject->getThirst());
		unitSocialNeed = std::to_string(triangleObject->getSocialNeed());
	}
	if (glfwGetKey(glfwWindow, GLFW_KEY_2) == GLFW_PRESS) {
		infoObject3.setColor(triangleObject2->getColor());
		unitName = triangleObject2->getName();
		unitFatigue = std::to_string(triangleObject2->getFatigue());
		unitMoney = std::to_string(triangleObject2->moneyCarried());
		unitHunger = std::to_string(triangleObject2->getHunger());
		unitThirst = std::to_string(triangleObject2->getThirst());
		unitSocialNeed = std::to_string(triangleObject2->getSocialNeed());
	}
	if (glfwGetKey(glfwWindow, GLFW_KEY_3) == GLFW_PRESS) {
		infoObject3.setColor(triangleObject3->getColor());
		unitName = triangleObject3->getName();
		unitFatigue = std::to_string(triangleObject3->getFatigue());
		unitMoney = std::to_string(triangleObject3->moneyCarried());
		unitHunger = std::to_string(triangleObject3->getHunger());
		unitThirst = std::to_string(triangleObject3->getThirst());
		unitSocialNeed = std::to_string(triangleObject3->getSocialNeed());
	}
	if (glfwGetKey(glfwWindow, GLFW_KEY_4) == GLFW_PRESS) {
		infoObject3.setColor(triangleObject4->getColor());
		unitName = triangleObject4->getName();
		unitFatigue = std::to_string(triangleObject4->getFatigue());
		unitMoney = std::to_string(triangleObject4->moneyCarried());
		unitHunger = std::to_string(triangleObject4->getHunger());
		unitThirst = std::to_string(triangleObject4->getThirst());
		unitSocialNeed = std::to_string(triangleObject4->getSocialNeed());
	}
	
}




