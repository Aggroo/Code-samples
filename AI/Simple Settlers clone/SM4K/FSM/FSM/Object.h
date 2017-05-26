#pragma once
#include <GL\glew.h>
#include <glm.hpp>
#include <gtx\transform.hpp>
#include <iostream>
#include "Model.h"
#include "telegramMessages.h"
#include <vector>

//class telegram;


class Object
{
private:
	bool physics;
	Model* model;
	Model staticModel;


protected:
	GLfloat posX;
	GLfloat posY;
	glm::vec4 color;
	glm::mat4 modelMatrix;
public:
	Object();
	Object(const glm::vec4 &color);
	~Object();
	void setModel(Model* model);

	void setModel(Model model);

	//Model GetModel(){ return staticModel; }
	Model* GetModel(){ return model; }
	void SetModel(Model* model){ this->model = model; }

	glm::mat4 getModelMatrix();

	Model GetStaticModel() const { return staticModel; }
	
	//Using models draw function
	void draw();
	//Update function, all logic goes here!
	virtual void update(float deltaTime);

	//Handles the communication of the units
	virtual bool handleMessage(const telegram& msg);

	//Dimension functions
	float getWidth() { return model->getWidth(); }
	float getHeight() { return model->getHeight(); }

	void setColor(glm::vec4 color);

	//Position functions
	void setPosition(GLfloat posX, GLfloat posY);
	glm::vec2 GetPosition();

	void UpdatePos();
	//Increment position
	void incrementX(GLfloat x) { posX += x; modelMatrix = getModelMatrix(); }
	void incrementY(GLfloat y) { posY += y; modelMatrix = getModelMatrix(); }
	//Setters and getters
	void setX(GLfloat x) { posX = x; }
	void setY(GLfloat y) { posY = y; }
	GLfloat getX() { return posX; }
	GLfloat getY() { return posY; }

	//Physics functions
	void enablePhysics(bool boolean) { physics = boolean; }
	
};

