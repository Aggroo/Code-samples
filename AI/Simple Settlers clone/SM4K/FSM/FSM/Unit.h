#pragma once
#include "Object.h"
#include <iostream>
#include "StateMachine.h"

class AISystem;
class UnitHandler;
class telegram;

enum profession
{
	none,
	worker,
	workerTree,
	explorer,
	builder,
	coalBurner
};

class Unit : public Object
{
public:
	Unit(const glm::vec4 &color, profession prof, int id);
	~Unit();
	void update(float deltaTime);
	//Handles the incoming messages
	bool handleMessage(const telegram& msg);

	//Returns a stateMachine
	StateMachine<Unit>* GetFSM();

	void moveToPosition();

	glm::vec2 Seek(glm::vec2 targetPos);
	glm::vec2 Arrive(glm::vec2 targetPos);

	glm::vec2 ObstacleAvoidance(std::vector<Object>* obstacles);

	std::vector<glm::vec2> GetPath();

	void SetTarget(glm::vec2 pos, int ID);

	profession GetProfession();
	void SetProfession(profession prof);

	void searchTrees();

	bool closeToEqual(glm::vec2 pos1, glm::vec2 pos2, float radius);

	int GetID();

	glm::vec2 GetGoalPosition() { return goalPosition; }
	void SetGoalPosition(glm::vec2 val) {  goalPosition = val; }

	float GetRadius() { return radius; }

	bool GetHarvesting(){ return harvesting; }
	void SetHarvesting() { harvesting = !harvesting; }

	bool GetCarryingTrees() { return carryingTrees; }
	void SetCarryingTrees() { carryingTrees = !carryingTrees; }


	bool Building(){ return building; }
	void SetBuilding(bool val) { building = val; }

	bool GetProducingCoal() { return producingCoal; }
	void SetProducingCoal() { producingCoal = !producingCoal; }

	bool GetWalkingPath() const { return walkingPath; }
	void SetWalkingPath() { walkingPath = !walkingPath; }

	glm::vec2 GetStartPosition() const { return startPosition; }
	void SetStartPosition(glm::vec2 val) { startPosition = val; }
private:
	//The units walking speed
	float speed;

	profession prof;

	int ID;

	//The startPosition and goalPosition of the unit
	glm::vec2 goalPosition;
	glm::vec2 startPosition;

	glm::vec2 velocity;
	glm::vec2 heading;
	glm::vec2 side;

	std::vector<glm::vec2> path;

	float maxSpeed;
	float mass;
	float radius;
	float deceleration;
	float minDetectionLength;
	float detectionLength;

	float deltaTime;

	bool firstRun;
	bool harvesting;
	bool carryingTrees;
	bool building;
	bool producingCoal;
	bool walkingPath;

	int radiusSearch;

	AISystem* aiSystem;
	UnitHandler* unitHandler;
	StateMachine<Unit> stateMachine;

};

