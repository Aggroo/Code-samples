#include "Unit.h"
#include <random>
#include <iostream>
#include "telegramMessages.h"
#include "messageDispatcher.h"
using std::cout;


Unit::Unit(const glm::vec3 &color, int id) : Object(color)
{
	stateMachine = new StateMachine<Unit>(this);

	fatigue = randomizer(80,100);
	hunger = randomizer(80, 100);
	thirst = randomizer(80, 100);
	money = 0;
	socialNeed = randomizer(80, 100);;

	speed = 7;

	offsetX = 0, offsetY = 0;

	lastTime = glfwGetTime();

	meetUp = false;

	chance = 0.20f;
	setName(id);
	stateMachine->setCurrentState(idle::Instance());
	stateMachine->setGlobalState(globalStates::Instance());
}


Unit::~Unit()
{
	delete stateMachine;
}
/********************************
*		ALL LOGIC GOES HERE		*
*		EXAMPLE YOUR AI!!!!		*
*********************************/
void Unit::update(float deltaTime)
{
	if (timeSinceLastUpdate() >= deltaTime)
	{
		lastTime = clock();		
		stateMachine->update(deltaTime);
		decreaseFatigue(0.1f);
		decreaseHunger(0.5f);
		decreaseThirst(0.7f);
		decreaseSocialNeed(0.5f);

		dispatcher->sendDelayedMessage();

		if (randomizer(0, 1) < 0.01f)
			setNewClothes(true);
	}

	moveToPosition(loc, fabs(1 / deltaTime));	
}


bool Unit::handleMessage(const telegram& msg)
{
	return stateMachine->handleMessage(msg);
}

StateMachine<Unit>* Unit::GetFSM() const
{
	return stateMachine;
}

locations Unit::Location() const
{
	return loc;
}

void Unit::changeLocation(locations loc)
{
	this->loc = loc;

	if (loc == home1)
		setGoalPosition(400 + getOffsetX(), 100 + getOffsetY());
	else if (loc == home2)
		setGoalPosition(650 + getOffsetX(), 100 + getOffsetY());
	else if (loc == work1)
		setGoalPosition(500 + getOffsetX(), 500 + getOffsetY());
	else if (loc == work2)
		setGoalPosition(650 + getOffsetX(), 550 + getOffsetY());
	else if (loc == pub)
		setGoalPosition(280 + getOffsetX(), 550 + getOffsetY());
	else if (loc == restaurant)
		setGoalPosition(250 + getOffsetX(), 340 + getOffsetY());
	else if (loc == shop)
		setGoalPosition(680 + getOffsetX(), 320 + getOffsetY());
	else if (loc == graveyard)
		setGoalPosition(30 + getOffsetX(), 670 + getOffsetY());
}

int Unit::moneyCarried() const
{
	return money;
}

void Unit::setMoneyCarried(int val)
{
	money = val;
}

void Unit::increaseMoneyCarried(float val)
{
	money += val;

	if (money < 0) 
		money = 0;
}

void Unit::decreaseMoneyCarried(float val)
{
	money -= val;

	if (money < 0)
		money = 0;
}

float Unit::getThirst() const
{
	return thirst;
}

void Unit::increaseThirst(float val)
{
	
	thirst += val;

	if (thirst > 100)
		thirst = 100;
}

void Unit::decreaseThirst(float val)
{
	thirst -= val;

	if (thirst < 0)
		thirst = 0;
}

float Unit::getHunger() const
{
	return hunger;
}

void Unit::increaseHunger(float val)
{
	hunger += val;

	if (hunger > 100)
		hunger = 100;
}

void Unit::decreaseHunger(float val)
{
	hunger -= val;

	if (hunger < 0)
		hunger = 0;
}

float Unit::getFatigue() const
{
	return fatigue;
}

void Unit::increaseFatigue(float val)
{
	fatigue += val;
	if (fatigue > 100)
		fatigue = 100;
}

void Unit::decreaseFatigue(float val)
{
	fatigue -= val;

	if (fatigue < 0)
		fatigue = 0;
}

float Unit::getSocialNeed() const
{
	return socialNeed;
}

void Unit::increaseSocialNeed(float val)
{
	socialNeed += val;

	if (socialNeed > 100)
		socialNeed = 100;
}

void Unit::decreaseSocialNeed(float val)
{
	socialNeed -= val;

	if (socialNeed < 0)
		socialNeed = 0;
}

bool Unit::getNewClothes() const
{
	return newClothes;
}

void Unit::setNewClothes(bool val)
{
	newClothes = val;
}

bool Unit::getMeetUp() const
{
	return meetUp;
}

void Unit::setMeetUp(bool val)
{
	meetUp = val;
}

int Unit::getOffsetX()
{
	switch (id)
	{
	case 0:
		return offsetX;
	case 1:
		return (offsetX + 40);
	case 2:
		return offsetX;
	case 3:
		return (offsetX + 40);
	}
}

int Unit::getOffsetY()
{
	switch (id)
	{
	case 0:
		return offsetY;
	case 1:
		return offsetY;
	case 2:
		return (offsetY + 40);
	case 3:
		return (offsetY + 40);
	}
}

float Unit::getChance()
{
	return chance;
}

float Unit::randomizer(float minVal, float maxVal)
{
	float random;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(minVal, maxVal);

	random = dis(gen);
	return random;
}

void Unit::moveToPosition(locations loc, float deltaTime)
{
	startPosition = glm::vec2(getX(), getY());
	glm::vec2 locVec = listOfLoc.getListOfLocations(loc) + glm::vec2(getOffsetX(), getOffsetY());
	glm::vec2 vecC = (locVec - startPosition);

	if (glm::length(vecC) <= speed*deltaTime)
	{
		setPosition(locVec.x, locVec.y);
	}
	else
	{
		vecC = glm::normalize(vecC);
		glm::vec2 vec = vecC*speed*deltaTime;
		posX += vec.x;
		posY += vec.y;
	}
	
}

void Unit::setGoalPosition(float x, float y)
{
	goalPosition = glm::vec2(x, y);
}

void Unit::executeMessage(locations loc)
{
	if (loc == home1 || loc == home2)
		cout << "\n" << getName() << ": " << "zZZzz..";
	else if (loc == work1 || loc == work2)
		cout << "\n" << getName() << ": " << "Working hard or hardly working..";
	else if (loc == restaurant)
		cout << "\n" << getName() << ": " << "*Munch munch* this food tastes amazing!";
	else if (loc == pub)
		cout << "\n" << getName() << ": " << "*Blubb blubb* aaah refreshing!";
	else if (loc == shop)
		cout << "\n" << getName() << ": " << "This shirt looks amazing!";
}

float Unit::timeSinceLastUpdate()
{
	return (((float)clock() - lastTime) / CLOCKS_PER_SEC);
}

locations Unit::getHome(int id)
{
	if (id == 1 || id == 3)
	{
		return home1;
	}
	else
	{
		return home2;
	}
}

locations Unit::getWork(int id)
{
	if (id == 2 || id == 3)
	{
		return work1;
	}
	else
	{
		return work2;
	}
}

