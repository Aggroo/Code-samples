#include "HiveMind.h"
#include "messageDispatcher.h"
#include "AISystem.h"
#include "Unit.h"
#include "UnitHandler.h"
#include "UnitStates.h"
#include "TileHandler.h"


HiveMind* HiveMind::Instance()
{
	static HiveMind instance;
	return &instance;
}

void HiveMind::Update(float deltaTime)
{
	if (TileHandler::Instance()->GetTileAtPos(UnitHandler::Instance()->GetHomePosition())->GetTreeAmount() == 10 && !TileHandler::Instance()->GetTileAtPos(UnitHandler::Instance()->GetHomePosition())->GetBuilding())
	{
		prioQueue.push_back(taskList[BuildingNeedsBuilder]);
		TileHandler::Instance()->GetTileAtPos(UnitHandler::Instance()->GetHomePosition())->SetModel(TileHandler::Instance()->GetTileModel(7));
		TileHandler::Instance()->GetTileAtPos(UnitHandler::Instance()->GetHomePosition())->SetTreeAmount(0);
		TileHandler::Instance()->GetTileAtPos(UnitHandler::Instance()->GetHomePosition())->SetBuilding(true);
	}
	if (prioQueue.size() != 0)
	{
		if (prioQueue.back() == 7)
		{
			Unit* unit = GetIdleWorker();
			if (unit != nullptr)
			{
				dispatcher->sendMessage(0.0f, GetID(), unit->GetID(), msg_UpgradeExplorer, 0);
				prioQueue.pop_back();
			}

		}
		else if (prioQueue.back() == 6)
		{
			
			Unit* unit = GetIdleWorker();
			if (unit != nullptr)
			{
				dispatcher->sendMessage(0.0f, GetID(), unit->GetID(), msg_SearchWood, 0);
				mCheck++;
			}
			if (mCheck == 5)
			{
				prioQueue.pop_back();
				mCheck = 0;
			}	
		}
		else if (prioQueue.back() == 5)
		{
			if (AISystem::Instance()->GetTreeFound())
			{
				TileHandler::Instance()->GetTileAtPos(UnitHandler::Instance()->GetHomePosition())->SetModel(TileHandler::Instance()->GetTileModel(6));
				prioQueue.pop_back();
			}	
		}
		else if (prioQueue.back() == 4)
		{
			Unit* unit = GetBuilder();
			if (unit == nullptr)
			{
				unit = GetWorker();
				if (unit != nullptr)
				{
					dispatcher->sendMessage(0.0f, GetID(), unit->GetID(), msg_UpgradeBuilder, 0);
					prioQueue.pop_back();
				}
			}
			else
			{
				dispatcher->sendMessage(0.0f, GetID(), unit->GetID(), msg_NeedBuilder, 0);
				prioQueue.pop_back();
			}
			
		}
		else if (prioQueue.back() == 3)
		{

		}
		else if (prioQueue.back() == 2)
		{
			Unit* unit = GetWorker();
			if (unit != nullptr)
			{
				dispatcher->sendMessage(0.0f, GetID(), unit->GetID(), msg_UpgradeCoalBurner, 0);
				prioQueue.pop_back();
			}
		}
		else if (prioQueue.back() == 1)
		{

		}
	}	
}

int HiveMind::GetID()
{
	return ID;
}

Unit* HiveMind::GetIdleWorker()
{
	for (int i = 1; i < UnitHandler::Instance()->GetUnits()->size(); i++)
	{
		if (UnitHandler::Instance()->GetUnit(i)->GetFSM()->getCurrentState() == idle::Instance())
		{
			return UnitHandler::Instance()->GetUnit(i);
		}
	}
	return nullptr;
}

Unit* HiveMind::GetWorker()
{
	for (int i = 1; i < UnitHandler::Instance()->GetUnits()->size(); i++)
	{
		if (UnitHandler::Instance()->GetUnit(i)->GetFSM()->getCurrentState() != idle::Instance() && UnitHandler::Instance()->GetUnit(i)->GetProfession() == profession::worker)
		{
			return UnitHandler::Instance()->GetUnit(i);
		}
	}
	return nullptr;
}

Unit* HiveMind::GetBuilder()
{
	for (int i = 1; i < UnitHandler::Instance()->GetUnits()->size(); i++)
	{
		if (UnitHandler::Instance()->GetUnit(i)->GetFSM()->getCurrentState() != idle::Instance() && UnitHandler::Instance()->GetUnit(i)->GetProfession() == profession::builder)
		{
			return UnitHandler::Instance()->GetUnit(i);
		}
	}
	return nullptr;
}
int HiveMind::GetTaskListValue(tasks task)
{
	return taskList[task];
}

void HiveMind::IncreaseCoalAmount()
{
	coalAmount++;
	printf("<Hivemind> Coal amount: %i \n", coalAmount);
}

HiveMind::HiveMind() : mCheck(0)
{

	taskList[CreateExplorer] = 7;
	taskList[HarvestWood] = 6;
	taskList[BuildCoalBurner] = 5;
	taskList[BuildingNeedsBuilder] = 4;
	taskList[BurnerNeedsCoalBurner] = 1;
	taskList[CreateBuilder] = 3;
	taskList[CreateCoalBurner] = 2;


	prioQueue.push_back(taskList[BuildCoalBurner]);

	prioQueue.push_back(taskList[CreateExplorer]);
	prioQueue.push_back(taskList[CreateExplorer]);
	prioQueue.push_back(taskList[CreateExplorer]);
	prioQueue.push_back(taskList[CreateExplorer]);
}

