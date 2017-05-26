#include "Object.h"
#include "telegramMessages.h"

Object::Object()
{
}

Object::Object(const glm::vec4 &color)
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
	modelMatrix = getModelMatrix();
}

glm::vec2 Object::GetPosition()
{
	return glm::vec2(posX, posY);
}

void Object::UpdatePos()
{
	modelMatrix = getModelMatrix();
}

void Object::setColor(glm::vec4 color)
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

void Object::draw()
{

	if (!staticModel.getLine())
	{
		if (model->getLine())
			model->draw(modelMatrix, color, true);
		else
			model->draw(modelMatrix, color);
	}
	else
	{
		staticModel.draw(modelMatrix, color, true);
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

bool Object::handleMessage(const telegram& msg)
{
	return true;
}
