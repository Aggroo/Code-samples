#pragma once

//Glew
#include <GL\glew.h>

//GLFW
#include <GLFW\glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>

//Streams
#include <fstream>

//vector
#include <vector>

//Lambda function
#include <functional>

//Shader loader
#include "loadShaders.h"

#include "Model.h"

#include "shapeGenerators.hpp"
#include "Object.h"
#include "Unit.h"
#include "Camera2D.h"
#include "GraphNodeTypes.h"
#include "GraphEdgeTypes.h"
#include "SparseGraph.h"
#include "TileHandler.h"

class myWindow
{
//Private pointers and constants
private:

	//Window height and width
	float windowHeight;
	float windowWidth;

	GLFWwindow* glfwWindow;

	//Ids and shit here
	GLuint programID;
	GLuint textShaderID;
	GLuint lineShaderID;

	//Time variables
	float deltaTime;
	TileHandler* tileMap;
	std::vector<Unit> objects;
	std::vector<Object> map;
	std::vector<Object> grid;
	std::vector<Object> path;

	typedef SparseGraph<NavGraphNode<void*>, GraphEdge> NavGraph;

	NavGraph* graph;
	NavGraph* workerGraph;

	std::vector<Model> tiles;

	Camera2D* camera;
	
	bool pathFinding;

public:
	//Standard functions**
	myWindow(int windowWidth, int windowHeight) : windowWidth(windowWidth), windowHeight(windowHeight) 
	{
		camera = Camera;
		camera->SetWindowWidth(windowWidth);
		camera->SetWindowHeight(windowHeight);
		camera->SetPos(glm::vec2(-windowWidth*0.5f, -windowHeight*0.5f));
		camera->SetViewMatrix(glm::translate(glm::vec3(-windowWidth*0.5f, -windowHeight*0.5f, 0.0f)));
		camera->SetProjectionMatrix(glm::ortho(-windowWidth*0.5f, windowWidth*0.5f, windowHeight*0.5f, -windowHeight*0.5f));
	};

	//Initialized GL
	int initializeGL();

	//Create window
	int createWindow();

	//Initalize scene
	void initializeScene();


	float GetWindowHeight() const { return windowHeight; }
	float GetWindowWidth() const { return windowWidth; }

	float DeltaTime();

private:

	//install shaders
	void installShaders();

	//NON STANDARD FUNCTIONS
	void gameLoop();

	void handleEvents(Unit &player);

	void handleEvents();

	static void StaticScrollCallback(GLFWwindow* window, double x, double y);

	void loadMap(const char* filename, int h, int w, glm::vec2 tileSize);

	bool PointToIndex(glm::vec2 p, int& NodeIndex);
};

