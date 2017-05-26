#pragma once
#include <GL\glew.h>
#include <glm.hpp>
#include <gtx\transform.hpp>
#include <iostream>
#include "Model.h"
#include "telegramMessages.h"
#include <map>

class Object
{
private:
	bool physics;
	Model* model;
	GLuint MVPLocation;
protected:
	GLfloat posX;
	GLfloat posY;
	glm::vec3 color;

	std::string name;
	int id;
	int nextID;


public:
	Object();
	Object(const glm::vec3 &color);
	~Object();
	void setModel(Model* model);

	glm::mat4 getModelMatrix();
	//Using models draw function
	void draw(const glm::mat4 &projection);
	//Update function, all logic goes here!
	virtual void update(float deltaTime);

	//Handles the communication of the units
	virtual bool handleMessage(const telegram& msg);

	//Dimension functions
	float getWidth() { return model->getWidth(); }
	float getHeight() { return model->getHeight(); }

	//Getters and setters for color
	glm::vec3 getColor() const;
	void setColor(glm::vec3 color);

	//Position functions
	void setPosition(GLfloat posX, GLfloat posY);
	//Increment position
	void incrementX(GLfloat x) { posX += x; }
	void incrementY(GLfloat y) { posY += y; }
	//Setters and getters
	void setX(GLfloat x) { posX = x; }
	void setY(GLfloat y) { posY = y; }
	GLfloat getX() { return posX; }
	GLfloat getY() { return posY; }
	
	//Get and set Name;
	std::string getName() const;
	void setName(int val);

	//Gets the unit ID
	int getID() const;

	//Physics functions
	void enablePhysics(bool boolean) { physics = boolean; }
	
};

