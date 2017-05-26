#pragma once
#include <map>
#include <queue>
#include "StateMachine.h"

class Unit;
enum tasks
{
	CreateExplorer,
	CreateBuilder,
	CreateCoalBurner,
	HarvestWood,
	BuildCoalBurner,
	BuildingNeedsBuilder,
	BurnerNeedsCoalBurner

};

class telegram;
/************************************************************************/
/*						HiveMind - Singleton							*/
/*					Controls the units AI. Main AI						*/
/************************************************************************/
class HiveMind
{
public:
	static HiveMind* Instance();

	//Updates the HiveMinds AI
	void Update(float deltaTime);
	//Handles the incoming messages
	bool handleMessage(const telegram& msg);

	//Get's HiveMinds ID
	int GetID();

	//Finds a worker in Idle state
	Unit* GetIdleWorker();

	//Finds a worker that's not in a idle state and has a worker profession
	Unit* GetWorker();
	//Finds a idle Builder
	Unit* GetBuilder();
	
	//Get the HiveMinds queue
	std::vector<int>* GetPrioQueue() { return &prioQueue; }

	//Get the value of a Task
	int GetTaskListValue(tasks task);

	//Get total amount of coal
	int GetCoalAmount() const { return coalAmount; }
	//Sets an amount of coal
	void SetCoalAmount(int val) { coalAmount = val; }
	//Increase coal by one
	void IncreaseCoalAmount();
private:
	HiveMind();
	HiveMind(const HiveMind&) = delete;
	HiveMind& operator=(const HiveMind&) = delete;

	//Holds the values of all tasks
	std::map<tasks, int> taskList;
	//The priority queue of the HiveMind
	std::vector<int> prioQueue;
	//ID of the HiveMind
	int ID = 0;

	//Message check. Used when five workers got a SearchWood message
	int mCheck;

	//Total Coal amount
	int coalAmount;
};

