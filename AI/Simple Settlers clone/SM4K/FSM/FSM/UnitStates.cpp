#include "UnitStates.h"
#include "Unit.h"
#include "messageDispatcher.h"
#include "telegramMessages.h"

#include <iostream>
#include "UnitHandler.h"
#include "TileHandler.h"
#include "HiveMind.h"
#include "messageTimers.h"

using std::cout;


///////////////////////
//------IDLE--------//
idle* idle::Instance()
{
	static idle instance;
	return &instance;
}

void idle::enter(Unit* unit)
{

}

void idle::execute(Unit* unit)
{
	//unit->GetFSM()->changeState(goToWork::Instance());
}

void idle::exit(Unit* unit)
{
	//printf("Left State\n");
}

bool idle::onMessage(Unit* unit, const telegram& msg)
{
	if (msg.msg == messageType::msg_SearchWood)
	{
		//printf("Unit %i received msg_SearchWood\n", unit->GetID());
		unit->GetFSM()->changeState(GetWood::Instance());
		return true;
	}
	if (msg.msg == messageType::msg_UpgradeExplorer)
	{
		//printf("Unit %i received message msg_UpgradeExplorer\n", unit->GetID());
		unit->GetFSM()->changeState(Upgrade::Instance());
		dispatcher->sendMessage(messageTimers::Instance()->GetCreateExplorer(), unit->GetID(), unit->GetID(), msg_UpgradeExplorer, 0);
		return true;
	}
	return false;
}

/////////////////////////
//------Upgrade--------//
Upgrade* Upgrade::Instance()
{
	static Upgrade instance;
	return &instance;
}

void Upgrade::enter(Unit* unit)
{
	
}

void Upgrade::execute(Unit* unit)
{
	//unit->GetFSM()->changeState(goToWork::Instance());
}

void Upgrade::exit(Unit* unit)
{
	//printf("Left State\n");
}

bool Upgrade::onMessage(Unit* unit, const telegram& msg)
{
	if (msg.msg == messageType::msg_UpgradeExplorer)
	{
		printf("Unit %i received message msg_UpgradeExplorer\n", unit->GetID());
		unit->SetProfession(explorer);
		unit->SetModel(UnitHandler::Instance()->GetUnitModel(profession::explorer));
		return true;
	}
	else if (msg.msg == messageType::msg_UpgradeBuilder)
	{
		printf("Unit %i received message msg_UpgradeBuilder\n", unit->GetID());
		unit->SetProfession(builder);
		unit->SetModel(UnitHandler::Instance()->GetUnitModel(profession::builder));
		unit->GetFSM()->changeState(BuildBuilding::Instance());
		return true;
	}
	else if (msg.msg == messageType::msg_UpgradeCoalBurner)
	{
		printf("Unit %i received message msg_UpgradeCoalBurner\n", unit->GetID());
		unit->SetProfession(coalBurner);
		unit->SetModel(UnitHandler::Instance()->GetUnitModel(profession::coalBurner));
		unit->SetGoalPosition(UnitHandler::Instance()->GetHomePosition());
		unit->GetFSM()->changeState(ProduceCoal::Instance());
		return true;
	}
	return false;
}

///////////////////////
//------ProduceCoal--------//
ProduceCoal* ProduceCoal::Instance()
{
	static ProduceCoal instance;
	return &instance;
}

void ProduceCoal::enter(Unit* unit)
{
	unit->SetWalkingPath();
	unit->SetTarget(UnitHandler::Instance()->GetHomePosition(), unit->GetID());
}

void ProduceCoal::execute(Unit* unit)
{

	if (unit->GetPosition() != unit->GetGoalPosition())
	{
		unit->moveToPosition();
	}
	else if (unit->closeToEqual(unit->GetPosition(), unit->GetGoalPosition(),unit->GetRadius()+5))
	{
		if (!unit->GetProducingCoal() && TileHandler::Instance()->GetTileAtPos(unit->GetPosition())->GetTreeAmount() > 2)
		{
			unit->SetProducingCoal();
			dispatcher->sendMessage(messageTimers::Instance()->GetProduceCoal(), unit->GetID(), unit->GetID(), msg_ProduceCoal, 0);
		}
	}

}

void ProduceCoal::exit(Unit* unit)
{
	//printf("Left State\n");
}

bool ProduceCoal::onMessage(Unit* unit, const telegram& msg)
{
	if (msg.msg == messageType::msg_ProduceCoal)
	{
		//printf("Unit %i received msg_ProduceCoal\n", unit->GetID());
		//printf("Unit %i produced coal\n", unit->GetID());
		TileHandler::Instance()->GetTileAtPos(UnitHandler::Instance()->GetHomePosition())->DecreaseTreeAmountByTwo();
		HiveMind::Instance()->IncreaseCoalAmount();
		unit->SetProducingCoal();
		return true;
	}
	return false;
}

///////////////////////////////
//------BuildBuilding--------//
BuildBuilding* BuildBuilding::Instance()
{
	static BuildBuilding instance;
	return &instance;
}

void BuildBuilding::enter(Unit* unit)
{
	unit->SetWalkingPath();
	unit->SetTarget(UnitHandler::Instance()->GetHomePosition(), unit->GetID());
}

void BuildBuilding::execute(Unit* unit)
{

	if (unit->GetPosition() != unit->GetGoalPosition())
	{
		unit->moveToPosition();
	}
	else if (unit->GetPosition() == unit->GetGoalPosition())
	{
		if (!unit->Building())
		{
			unit->SetBuilding(true);
			dispatcher->sendMessage(messageTimers::Instance()->GetBuildingDone(), unit->GetID(), unit->GetID(), msg_BuildingDone, 0);
		}
	}
}

void BuildBuilding::exit(Unit* unit)
{
	//printf("Left State\n");
}

bool BuildBuilding::onMessage(Unit* unit, const telegram& msg)
{
	if (msg.msg == messageType::msg_BuildingDone)
	{
		//printf("Unit %i received msg_BuildingDone\n", unit->GetID());
		TileHandler::Instance()->GetTileAtPos(UnitHandler::Instance()->GetHomePosition())->SetModel(TileHandler::Instance()->GetTileModel(8));
		HiveMind::Instance()->GetPrioQueue()->push_back(HiveMind::Instance()->GetTaskListValue(CreateCoalBurner));
		return true;
	}
	return false;
}

///////////////////////
//------GetWood--------//
GetWood* GetWood::Instance()
{
	static GetWood instance;
	return &instance;
}

void GetWood::enter(Unit* unit)
{
	//printf("Entered State\n");
	unit->searchTrees();
}

void GetWood::execute(Unit* unit)
{
	if (unit->GetPosition() != unit->GetGoalPosition())
	{
		unit->moveToPosition();
	}
	else if (unit->GetPosition() == unit->GetGoalPosition() )
	{
		if (!unit->GetHarvesting())
		{
			unit->SetHarvesting();
			dispatcher->sendMessage(messageTimers::Instance()->GetHarvestWood(), unit->GetID(), unit->GetID(), msg_HarvestWood, 0);
		}
	}
}

void GetWood::exit(Unit* unit)
{

}

bool GetWood::onMessage(Unit* unit, const telegram& msg)
{
	if (msg.msg == messageType::msg_HarvestWood)
	{
		//printf("Unit %i received message\n", UnitHandler::Instance()->GetUnit(msg.receiver)->GetID());
		Tile* tile = TileHandler::Instance()->GetTileAtPos(unit->GetPosition());
		for (int i = 0; i < tile->GetTreeList()->size(); i++)
		{
			if (unit->closeToEqual(unit->GetPosition(), tile->GetTreeList()->at(i)->GetPosition(), unit->GetRadius()-3))
			{
				tile->GetTreeList()->at(i)->Harvested();
				unit->SetCarryingTrees();
			}
				
		}
		unit->GetFSM()->changeState(LeaveWood::Instance());
		return true;
	}
	return false;
}

///////////////////////
//------LeaveWood--------//
LeaveWood* LeaveWood::Instance()
{
	static LeaveWood instance;
	return &instance;
}

void LeaveWood::enter(Unit* unit)
{	
	//printf("Entered State\n");
	unit->SetModel(UnitHandler::Instance()->GetUnitModel(workerTree));
	unit->SetTarget(UnitHandler::Instance()->GetHomePosition(),unit->GetID());
}

void LeaveWood::execute(Unit* unit)
{
	if (unit->GetPosition() != unit->GetGoalPosition())
	{
		unit->moveToPosition();
	}
	else if (unit->GetPosition() == unit->GetGoalPosition())
	{
		TileHandler::Instance()->GetTileAtPos(unit->GetPosition())->IncreaseTreeAmount();
		printf("Tree Amount: %i\n",TileHandler::Instance()->GetTileAtPos(unit->GetPosition())->GetTreeAmount());
		unit->SetCarryingTrees();
		unit->SetHarvesting();
		unit->SetModel(UnitHandler::Instance()->GetUnitModel(worker));
		unit->GetFSM()->changeState(idle::Instance());
	}
}

void LeaveWood::exit(Unit* unit)
{
	
}

bool LeaveWood::onMessage(Unit* unit, const telegram& msg)
{

	return false;
}

/////////////////////////////////
//-------GlobalStates----------//
globalStates* globalStates::Instance()
{
	static globalStates instance;
	return &instance;
}

void globalStates::enter(Unit* unit)
{

}

void globalStates::execute(Unit* unit)
{
	


}

void globalStates::exit(Unit* unit)
{

}

bool globalStates::onMessage(Unit* unit, const telegram& msg)
{
	if (msg.msg == messageType::msg_UpgradeBuilder)
	{
		//printf("Unit %i received message msg_UpgradeBuilder\n", unit->GetID());
		unit->GetFSM()->changeState(Upgrade::Instance());
		dispatcher->sendMessage(messageTimers::Instance()->GetCreateBuilder(), unit->GetID(), unit->GetID(), msg_UpgradeBuilder, 0);
		return true;
	}
	else if (msg.msg == messageType::msg_NeedBuilder)
	{
		//printf("Unit %i received message msg_NeedBuilder\n", unit->GetID());
		unit->GetFSM()->changeState(BuildBuilding::Instance());
		return true;
	}
	else if (msg.msg == messageType::msg_UpgradeCoalBurner)
	{
		//printf("Unit %i received message msg_UpgradeCoalBurner\n", unit->GetID());
		unit->GetFSM()->changeState(Upgrade::Instance());
		dispatcher->sendMessage(messageTimers::Instance()->GetCreateCoalBurner(), unit->GetID(), unit->GetID(), msg_UpgradeCoalBurner, 0);
		return true;
	}
	return false;
}

