#pragma once
#include "Object.h"
#include "StateMachine.h"
#include "UnitStates.h"
#include "locations.h"
#include <iostream>
#include <string>
#include <ctime>

struct telegram;

class Unit : public Object
{
public:
	//Constructor and deconstructor
	Unit(const glm::vec3 &color, int id);
	~Unit();

	//Update function. Updates the entire unit
	void update(float deltaTime);
	//Handles the incoming messages
	bool handleMessage(const telegram& msg);

	//Returns a stateMachine
	StateMachine<Unit>* GetFSM()const;

	//Returns the location
	locations Location()const;
	//Sets the new location. Changes the goalPosition
	void changeLocation(locations loc);
	
	//Returns money
	int moneyCarried() const;
	//Sets the amount of money carried
	void setMoneyCarried(int val);
	//Increase the money with a chosen value
	void increaseMoneyCarried(float val);
	//Decrease the money with a chosen value
	void decreaseMoneyCarried(float val);

	//Returns thirst
	float getThirst() const;
	//Increase the thirst with a chosen value
	void increaseThirst(float val);
	//Decrease the thirst with a chosen value
	void decreaseThirst(float val);

	//Returns hunger
	float getHunger() const;
	//Increase the hunger with a chosen value
	void increaseHunger(float val);
	//Decrease the hunger with a chosen value
	void decreaseHunger(float val);

	//Returns fatigue
	float getFatigue() const;
	//Increase the fatigue with a chosen value
	void increaseFatigue(float val);
	//Decrease the fatigue with a chosen value
	void decreaseFatigue(float val);

	//Returns socialNeed
	float getSocialNeed() const;
	//Increase the socialNeed with a chosen value
	void increaseSocialNeed(float val);
	//Decrease the socialNeed with a chosen value
	void decreaseSocialNeed(float val);

	//Returns true or false. Depending of newClothes
	bool getNewClothes() const;
	//Sets newClothes to true or false
	void setNewClothes(bool val);

	//Returns true or false. Depending of meetUp
	bool getMeetUp() const;
	//Sets meetUp to true or false
	void setMeetUp(bool val);

	//Returns offsetX
	int getOffsetX();
	//Returns offsetY
	int getOffsetY();

	//Returns chance
	float getChance();
	//Randomizes between two float values and returns the randomized digit
	float randomizer(float minVal, float maxVal);

	//Moves the unit towards goalPosition at a certain speed
	void moveToPosition(locations loc, float deltaTime);
	//Changes the goal position
	void setGoalPosition(float x, float y);

	//Writes a message depending on the location. Used in the execute states
	void executeMessage(locations loc);

	//Returns the time since the last time check
	float timeSinceLastUpdate();
	
	//Returns home1 or home2 depending on the ID
	locations getHome(int id);
	//Returns work1 or work2 depending on the ID
	locations getWork(int id);
private:
	//The units stateMachine
	StateMachine<Unit>* stateMachine;

	//Class that can send the position of a certain location
	locationList listOfLoc;
	//Enum of locations
	locations loc;
	//Units thirst value
	float thirst;
	//Units hunger value
	float hunger;
	//Units socialNeed value
	float socialNeed;
	//Units fatigue value
	float fatigue;
	//Units money value
	float money;

	//Booleans that is used to check if the unit should meetUp, buy new clothes or if it's dead
	bool newClothes;
	bool dead;
	bool meetUp;

	//The unit offsets. Used to not have the units overlap when in the same building
	int offsetX;
	int offsetY;

	//The percental chance for the unit to change state
	float chance;

	//The units walking speed
	float speed;

	//The startPosition and goalPosition of the unit
	glm::vec2 goalPosition;
	glm::vec2 startPosition;
	
	//The last updated time
	double lastTime;
	
};

