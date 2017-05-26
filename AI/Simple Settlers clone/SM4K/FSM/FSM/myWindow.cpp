#include "myWindow.h"
#include "Text2D.h"
#include "Camera2D.h"
#include "graphLoader.h"
#include <algorithm>
#include "GraphAlgorithms.h"
#include "SettingsManager.h"
#include "AISystem.h"
#include "TileHandler.h"
#include "UnitHandler.h"
#include "GameHandler.h"
#include "HiveMind.h"


//Standard functions ****************************************

int myWindow::initializeGL() {
	//Initialise GLFW 
	if( !glfwInit() ) {
		fprintf( stderr, "Failed to initialise GLFW");
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
	//Capture scroll wheel
	glfwSetScrollCallback(glfwWindow, myWindow::StaticScrollCallback);
	//glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);		//SHOW/HIDE CURSOR!

	return 0;
}



void myWindow::initializeScene() {
	//Do everything in order here instead of calling in main

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	installShaders();
	//Use installed shader
	glUseProgram(programID);

	glm::vec2 tileSize = glm::vec2(32, 32);
	
	tileMap = TileHandler::Instance();

	SettingsManager::Instance()->CreateReader("settings/settings.ini", "GlobalSettings");

	//std::cout << SettingsManager::Instance()->GetReader("GlobalSettings").GetString("Default", "World", "name") << std::endl;

	graph = AISystem::Instance()->GetGraph();
	workerGraph = AISystem::Instance()->GetWorkerGraph();

	tileMap->LoadMap("map3.txt", 100, 100, tileSize, programID);

	AISystem::Instance()->SetProgramID(programID);
	UnitHandler::Instance()->SetProgramID(programID);
	UnitHandler::Instance()->SetupUnitModels();
	UnitHandler::Instance()->CreateDummy(UnitHandler::Instance()->GetUnitModel(profession::worker), glm::vec2(500.0f, 500.0f), glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), profession::none);

	std::cout << "Mapsize: " << TileHandler::Instance()->GetMapSize().x << "x" << TileHandler::Instance()->GetMapSize().y << std::endl;
	
	//DrawGraph(*workerGraph, grid, programID);

	//Start running game loop
	gameLoop();
}

float myWindow::DeltaTime()
{
	if (deltaTime < 0.000001f)
	{
		return 0;
	}
	else
	{
		return deltaTime;
	}
}

void myWindow::installShaders() {
	programID = LoadShaders("vertexShader.gsl", "fragmentShader.gsl");
	textShaderID = LoadShaders("TextVertexShader.gsl", "TextFragmentShader.gsl");
	//debugProgramID = LoadShaders("debugVertexShader.glsl", "debugFragmentShader.glsl");
}

void myWindow::gameLoop() {

	//Text2D textPrinter;
	//textPrinter.initText2D(textShaderID, "font.DDS", windowWidth, windowHeight);


	UnitHandler* unit = UnitHandler::Instance();
	glm::vec2 randPosition;
	while (true)
	{
		randPosition = glm::vec2(AISystem::Instance()->randomizer(80, 3100), AISystem::Instance()->randomizer(80, 3100));

		if (TileHandler::Instance()->GetTileAtPos(randPosition)->GetType() == tileType::grass)
		{
			break;
		}
	}
	
	unit->SetHomePosition(randPosition);

	for (int y = 0; y < 25; y+=5)
	{
		for (int x = 0; x < 50; x+=5)
		{

			unit->CreateUnit(unit->GetUnitModel(profession::worker), glm::vec2(randPosition.x+x, randPosition.y+y), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), profession::worker);

			//unit->CreateUnit(unit->GetUnitModel(profession::worker), glm::vec2(2000 + x, 2850 + y), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), profession::worker);
			
			//unit->CreateUnit(&worker, glm::vec2(100 + x, 100 + y), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), profession::worker);
		}
	}
	Camera2D::Instance()->SetCameraPos(randPosition);
	//unit->CreateUnit(&worker, glm::vec2(100, 100), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), profession::explorer);
	AISystem::Instance()->FoW(1);
	AISystem::Instance()->FoW(45);
	
	//Time variables
	double lastTime = glfwGetTime();

	GLuint ViewLocation = glGetUniformLocation(programID, "ViewMatrix");
	GLuint ProjectionLocation = glGetUniformLocation(programID, "ProjectionMatrix");

	AISystem* ais = AISystem::Instance();
	while (glfwWindowShouldClose(glfwWindow) == 0)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		////Updates the projection
		glUniformMatrix4fv(ViewLocation, 1, GL_FALSE, &camera->GetViewMatrix()[0][0]);
		glUniformMatrix4fv(ProjectionLocation, 1, GL_FALSE, &camera->GetProjectionMatrix()[0][0]);

		//Calculate deltaTime
		double currentTime = glfwGetTime();
		deltaTime = fabs(currentTime - lastTime);

		if (HiveMind::Instance()->GetCoalAmount() > 200)
		{
			printf("Congratulations you won the game!\n");
		}

		//Draw and Update the map
		for (int i = 0; i < tileMap->GetMap()->size(); i++)
		{
			tileMap->GetMap()->at(i).draw();
		}

		//Draw and Update the grid
		for (int i = 0; i < ais->GetGrid()->size(); i++)
		{
			ais->GetGrid()->at(i).draw();
		}


		//for (int i = 0; i < ais->GetLinePath()->size(); i++)
		//{
		//	ais->GetLinePath()->at(i).draw();
		//}

		//Draw and Update all objects
		for (int i = 0; i < unit->GetUnits()->size(); i++)
		{
			unit->GetUnits()->at(i)->update(DeltaTime());
			unit->GetUnits()->at(i)->draw();
		}
		for (int i = 0; i < GameHandler::Instance()->GetTreeList()->size(); i++)
		{
			GameHandler::Instance()->GetTreeList()->at(i)->draw();
		}
		HiveMind::Instance()->Update(deltaTime);

		
		/*glUseProgram(textShaderID);

		textPrinter.printText2D("Test av text", 200, 200, 20);*/
		
		
		glUseProgram(programID);
		glfwSwapBuffers(glfwWindow);
		glfwPollEvents();
		
		//Can be used to control a specific object
		//handleEvents(unit->GetUnits()->at(2));

		//Can be used for normal button presses or control a camera
		handleEvents();

		//Set lastTime to currentTime!
		lastTime = currentTime;
	}

	glfwTerminate();
	return;
}

void myWindow::handleEvents(Unit &player) {
	int speed = 150;
	if (glfwGetKey(glfwWindow, GLFW_KEY_UP) == GLFW_PRESS) {
		player.Object::incrementY(speed*deltaTime *-1);
	}if (glfwGetKey(glfwWindow, GLFW_KEY_DOWN) == GLFW_PRESS) {
		player.Object::incrementY(speed*deltaTime);
	}if (glfwGetKey(glfwWindow, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		player.Object::incrementX(speed*deltaTime);
	}if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT) == GLFW_PRESS) {
		player.Object::incrementX(speed*deltaTime*-1);
	}
}

void myWindow::handleEvents() {
	if (glfwGetKey(glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(glfwWindow, GL_TRUE);
	}
	if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS) {
		camera->MoveCamera(0, 1);
	}
	if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS) {
		camera->MoveCamera(0, -1);
	}
	if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS) {
		camera->MoveCamera(-1, 0);
	}
	if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS) {
		camera->MoveCamera(1, 0);
	}
	if (glfwGetKey(glfwWindow, GLFW_KEY_HOME) == GLFW_PRESS) {
		Camera->SetZoomScale(Camera->GetZoomScale() + Camera->GetZoomStep());
	}
	if (glfwGetKey(glfwWindow, GLFW_KEY_END) == GLFW_PRESS) {
		Camera->SetZoomScale(Camera->GetZoomScale() - Camera->GetZoomStep());
	}
	if (glfwGetKey(glfwWindow, GLFW_KEY_F1) == GLFW_PRESS) {

		UnitHandler::Instance()->GetUnits()->at(1)->SetProfession(profession::explorer);
		UnitHandler::Instance()->GetUnits()->at(1)->SetModel(UnitHandler::Instance()->GetUnitModel(profession::explorer));
		UnitHandler::Instance()->GetUnits()->at(6)->SetProfession(profession::explorer);
		UnitHandler::Instance()->GetUnits()->at(6)->SetModel(UnitHandler::Instance()->GetUnitModel(profession::explorer));
		UnitHandler::Instance()->GetUnits()->at(2)->SetProfession(profession::explorer);
		UnitHandler::Instance()->GetUnits()->at(2)->SetModel(UnitHandler::Instance()->GetUnitModel(profession::explorer));
		//UnitHandler::Instance()->GetUnits()->at(2).SetProfession(profession::explorer);
		//UnitHandler::Instance()->GetUnits()->at(3).SetProfession(profession::explorer);
		//UnitHandler::Instance()->GetUnits()->at(4).SetProfession(profession::explorer);
		//UnitHandler::Instance()->GetUnits()->at(5).SetProfession(profession::explorer);
	}

}

void myWindow::StaticScrollCallback(GLFWwindow* window, double x, double y)
{

	Camera->ZoomCamera((float) y);
	
}

