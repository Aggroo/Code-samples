#include "myWindow.h"
#include "Text2D.h"
#include "graphLoader.h"
#include <algorithm>
#include "GraphAlgorithms.h"

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
	//glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);		//SHOW/HIDE CURSOR!

	return 0;
}



void myWindow::initializeScene() {
	//Do everything in order here instead of calling in main

	//Create projection matrix to be used 

	offsetX = windowWidth * 0.5f;
	offsetY = windowHeight * 0.5f;

	installShaders();
	//Use installed shader
	glUseProgram(programID);

	const char* filename = "Map1.txt";

	tiles = shapeGenerator::square(32, 32, programID);

	loadMap(filename, 16, 16, &tiles);

	graph = new NavGraph(true);

	createGraph(*graph, filename, tiles.getWidth(), tiles.getHeight(), 16, 16);

	//DrawGraph(*graph, grid, programID);

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

	
	//Time variables
	double lastTime = glfwGetTime();

	while (glfwWindowShouldClose(glfwWindow) == 0)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Projection = glm::ortho(offsetX - windowWidth*0.5f*zoomScale, offsetX + windowWidth*0.5f*zoomScale, offsetY + windowHeight*0.5f*zoomScale, offsetY - windowHeight*0.5f*zoomScale);

		//Calculate deltaTime
		double currentTime = glfwGetTime();
		deltaTime = fabs(currentTime - lastTime);

		if (showGraph)
		{
			showGraph = false;
			DrawGraph(*graph, grid, programID);
		}

		for (int i = 0; i < map.size(); i++)
		{

			map.at(i).draw(Projection);
		}
		
		for (int i = 0; i < grid.size(); i++)
		{
			grid.at(i).draw(Projection*zoomScale);
		}

		for (int i = 0; i < path.size(); i++)
		{

			path.at(i).draw(Projection*zoomScale);
		}

		glUseProgram(textShaderID);

		textPrinter.printText2D("1: Map 1", 840, 100, 20);
		textPrinter.printText2D("2: Map 2", 840, 120, 20);
		textPrinter.printText2D("3: Map 3", 840, 140, 20);

		textPrinter.printText2D("F1: DFS", 840, 180, 20);
		textPrinter.printText2D("F2: BFS", 840, 200, 20);
		textPrinter.printText2D("F3: A*", 840, 220, 20);
		textPrinter.printText2D("F4: Custom", 840, 240, 20);

		textPrinter.printText2D("G: Show G", 840, 280, 20);
		textPrinter.printText2D("H: Hide G", 840, 300, 20);

		glUseProgram(programID);
		glfwSwapBuffers(glfwWindow);
		glfwPollEvents();
		
		handleEvents();
		
		
		//Set lastTime to currentTime!
		lastTime = currentTime;
	}
}

void myWindow::handleEvents() {
	if (glfwGetKey(glfwWindow, GLFW_KEY_1) == GLFW_PRESS) {
		
		map.clear();
		grid.clear();
		path.clear();

		const char* filename = "Map1.txt";

		loadMap(filename, 16, 16, &tiles);

		graph = new NavGraph(true);

		createGraph(*graph, filename, tiles.getWidth(), tiles.getHeight(), 16, 16);

		//DrawGraph(*graph, grid, programID);


	}if (glfwGetKey(glfwWindow, GLFW_KEY_2) == GLFW_PRESS) {

		map.clear();
		grid.clear();
		path.clear();

		const char* filename = "Map2.txt";

		loadMap(filename, 16, 16, &tiles);

		graph = new NavGraph(true);

		createGraph(*graph, filename, tiles.getWidth(), tiles.getHeight(), 16, 16);

		//


	}if (glfwGetKey(glfwWindow, GLFW_KEY_3) == GLFW_PRESS) {
		
		map.clear();
		grid.clear();
		path.clear();

		const char* filename = "Map3.txt";

		loadMap(filename, 26, 26, &tiles);

		graph = new NavGraph(true);

		createGraph(*graph, filename, tiles.getWidth(), tiles.getHeight(), 26, 25);

		//DrawGraph(*graph, grid, programID);


	}
	if (glfwGetKey(glfwWindow, GLFW_KEY_G) == GLFW_PRESS) {
		
		showGraph = true;
	}
	if (glfwGetKey(glfwWindow, GLFW_KEY_H) == GLFW_PRESS) {

		grid.clear();
	}
	if (glfwGetKey(glfwWindow, GLFW_KEY_F1) == GLFW_PRESS) {

		path.clear();

		DepthFirstSearch<NavGraph> DFS(*graph, graph->GetStartIndex(), graph->GetGoalIndex());

		if (DFS.Found())
		{
			DrawPath(*graph, DFS.GetPathToTarget(), path, programID);
		}
	}
	if (glfwGetKey(glfwWindow, GLFW_KEY_F2) == GLFW_PRESS) {

		path.clear();

		BreadthFirstSearch<NavGraph> BFS(*graph, graph->GetStartIndex(), graph->GetGoalIndex());

		if (BFS.Found())
		{
			DrawPath(*graph, BFS.GetPathToTarget(), path, programID);
		}
	}
	if (glfwGetKey(glfwWindow, GLFW_KEY_F3) == GLFW_PRESS) {

		path.clear();

		AStarSearch<NavGraph> AST(graph, graph->GetStartIndex(), graph->GetGoalIndex());

		DrawPath(*graph, AST.GetPathToTarget(), path, programID);
	}
	if (glfwGetKey(glfwWindow, GLFW_KEY_F4) == GLFW_PRESS) {

		path.clear();

		CustomSearch<NavGraph> CST(*graph, graph->GetStartIndex(), graph->GetGoalIndex());

		DrawPath(*graph, CST.GetPathToTarget(), path, programID);
	}
	if (glfwGetKey(glfwWindow, GLFW_KEY_HOME) == GLFW_PRESS) {
		zoomScale += zoomStep;
		if (zoomScale > 10)
		{
			zoomScale = 10;
		}
	}
	if (glfwGetKey(glfwWindow, GLFW_KEY_END) == GLFW_PRESS) {
		zoomScale -= zoomStep;
		if (zoomScale < 0.1f)
		{
			zoomScale = 0.1f;
		}
	}
	if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS) {
		offsetY -= moveSpeed;
	}
	if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS) {
		offsetY += moveSpeed;
	}
	if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS) {
		offsetX -= moveSpeed;
	}
	if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS) {
		offsetX += moveSpeed;
	}

}

void myWindow::loadMap(const char* filename, int h, int w, Model* tile)
{
	std::string unWalkable = "X";
	std::string start = "S";
	std::string goal = "G";

	int height = h;
	int width = w;

	std::string* mapArray = new std::string[height*width];

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			mapArray[y*width + x] = "";
		}
	}
	
	squareSizeX = tile->getHeight();
	squareSizeY = tile->getWidth();
	
	FILE* file;
	file = fopen(filename, "r");
	char c;
	int i = 0;
	while ((c = fgetc(file)) != EOF) {

		if (c != '\n')
		{
			mapArray[i] = c;
			i++;
		}	

	}
	fclose(file);

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			Unit square = Unit(glm::vec3(1.0f, 1.0f, 1.0f));
			if (mapArray[y*width + x] == unWalkable)
			{
				square.setColor(glm::vec3(0.2f, 0.2f, 0.2f));
			}
			else if (mapArray[y*width + x] == start)
			{
				square.setColor(glm::vec3(0.0f, 1.0f, 0.0f));
			}
			else if (mapArray[y*width + x] == goal)
			{
				square.setColor(glm::vec3(1.0f, 0.0f, 0.0f));
			}

			square.setModel(tile);
			square.setPosition(squareSizeX*x, squareSizeY*y);
			map.push_back(square);
		}
	}
}

