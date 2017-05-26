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

//Shader loader
#include "loadShaders.h"

#include "Model.h"

#include "shapeGenerators.hpp"
#include "Object.h"
#include "Unit.h"
#include "GraphNodeTypes.h"
#include "GraphEdgeTypes.h"
#include "SparseGraph.h"


class myWindow
{
//Private pointers and constants
private:
	//Projection matrix
	glm::mat4 Projection;

	//Window height and width
	float windowHeight;
	float windowWidth;

	GLFWwindow* glfwWindow;

	//Ids and shit here
	GLuint programID;
	GLuint textShaderID;
	GLuint debugProgramID;

	int width;
	int height;

	int squareSizeX;
	int squareSizeY;

	//Time variables
	float deltaTime;

	std::vector<Object*> objects;
	std::vector<Object> map;
	std::vector<Object> grid;
	std::vector<Object> path;

	typedef SparseGraph<NavGraphNode<void*>, GraphEdge> NavGraph;

	NavGraph* graph;

	Model tiles;

	bool showGraph;

	float zoomScale = 1; 
	float offsetX, offsetY;
	float moveSpeed = 5;
	float zoomStep = 0.02f;

public:
	//Standard functions**
	myWindow(int windowWidth, int windowHeight) : windowWidth(windowWidth), windowHeight(windowHeight) {};

	//Initialized GL
	int initializeGL();

	//Create window
	int createWindow();

	//Initalize scene
	void initializeScene();

private:

	//install shaders
	void installShaders();

	//NON STANDARD FUNCTIONS
	void gameLoop();

	void handleEvents();

	void loadMap(const char* filename, int h, int w, Model* tile);


	
};

