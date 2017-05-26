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
#include <string>

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

	//Time variables
	float deltaTime;
	float speed;
	float lastSpeed;
	float updateFrequency;
	bool pause;

	//Character traits
	std::string unitName;
	std::string unitFatigue;
	std::string unitMoney;
	std::string unitHunger;
	std::string unitThirst;
	std::string unitSocialNeed;

	//The Units
	Object infoObject3;
	Unit* triangleObject;
	Unit* triangleObject2;
	Unit* triangleObject3;
	Unit* triangleObject4;


	std::vector<Object*> objects;
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

};

