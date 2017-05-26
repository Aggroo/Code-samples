#include "Object.h"

Object::Object()
{
}

Object::Object(const glm::vec3 &color)
{
	this->color = color;
	this->physics = false;
}

Object::~Object()
{
}

void Object::setPosition(GLfloat posX, GLfloat posY)
{
	this->posX = posX;
	this->posY = posY;
}

void Object::setColor(glm::vec3 color)
{
	this->color = color;
}

glm::mat4 Object::getModelMatrix()
{
	return glm::translate(glm::vec3(posX, posY, 0.0f));
}

void Object::setModel(Model* model)
{
	this->model = model;
}

void Object::setModel(Model model)
{
	staticModel = model;
}

//void Object::setStaticModel(Model model)
//{
//	this->staticModel = model;
//}

void Object::draw(const glm::mat4 &projection)
{
	if (!staticModel.getLine())
	{
		glm::mat4 MVP = projection * getModelMatrix();
		if (model->getLine())
			model->draw(MVP, color, true);
		else
			model->draw(MVP, color);
	}
	else
	{
		glm::mat4 MVP = projection * getModelMatrix();
		staticModel.draw(MVP, color, true);
	}
	
}


/********************************
*	LOGIC THAT AFFECTS ALL		*
*	GEOMETRY ON THE SCENE		*
*	GOES HERE!					*
*********************************/

void Object::update(float deltaTime)
{
}