#include "Object.h"

Object::Object()
{
}

Object::Object(const glm::vec3 &color)
{
	this->color = color;
	this->physics = false;

	nextID = 0;
}

Object::~Object()
{
}

void Object::setPosition(GLfloat posX, GLfloat posY)
{
	this->posX = posX;
	this->posY = posY;
}

glm::vec3 Object::getColor() const
{
	return color;
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

void Object::draw(const glm::mat4 &projection)
{
	glm::mat4 MVP = projection * getModelMatrix();
	model->draw(MVP, color);
}

std::string Object::getName() const
{
	switch (id)
	{
	case 0:
		return "Bob";
	case 1:
		return "Peter";
	case 2:
		return "Susan";
	case 3:
		return "Kim";
	default:
		break;
	}
}

void Object::setName(int val)
{
	assert((val >= nextID) && "<Object::setName>: invalid ID");

	id = val;
	nextID = id + 1;
}

int Object::getID() const
{
	return id;
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
