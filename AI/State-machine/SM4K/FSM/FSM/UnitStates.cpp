#include "UnitStates.h"
#include "Unit.h"
#include "locations.h"
#include "messageDispatcher.h"
#include "telegramMessages.h"

#include <iostream>
using std::cout;

class locationsList;

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
	unit->GetFSM()->changeState(goToWork::Instance());
}

void idle::exit(Unit* unit)
{

}

bool idle::onMessage(Unit* unit, const telegram& msg)
{
	return false;
}

////////////////////////////
//--------goToWork-------//
goToWork* goToWork::Instance()
{
	static goToWork instance;
	return &instance;
}

void goToWork::enter(Unit* unit)
{
	if (unit->Location() != unit->getWork(unit->getID()))
	{
		cout << "\n" << unit->getName() << ": " << "Walking to work";
		
		unit->changeLocation(unit->getWork(unit->getID()));
	}
}

void goToWork::execute(Unit* unit)
{
	unit->decreaseFatigue(0.5f);
	
	if (unit->getWork(unit->getID()) == work1)
		unit->increaseMoneyCarried(3);
	else
		unit->increaseMoneyCarried(4);

	unit->executeMessage(unit->Location());
}

void goToWork::exit(Unit* unit)
{
	cout << "\n" << unit->getName() << ": " << "Leaving work with some cash in my pocket";
}

bool goToWork::onMessage(Unit* unit, const telegram& msg)
{
	return false;
}

////////////////////////////////////////
//------EnterSHopToBuyClothes--------//
enterShopToBuyClothes* enterShopToBuyClothes::Instance()
{
	static enterShopToBuyClothes instance;
	return &instance;
}

void enterShopToBuyClothes::enter(Unit* unit)
{
	if (unit->Location() != shop)
	{
		cout << "\n" << unit->getName() << ": " << "Going to the shop to buy some new clothing";

		unit->changeLocation(shop);
	}
}

void enterShopToBuyClothes::execute(Unit* unit)
{
	if (unit->randomizer(0, 1) < 0.5f)
	{
		unit->setColor(glm::vec3(unit->randomizer(0.3f, 1.0f), unit->randomizer(0.3f, 1.0f), unit->randomizer(0.3f, 1.0f)));
		unit->decreaseMoneyCarried(30);
		unit->setNewClothes(false);

		unit->GetFSM()->changeState(goToWork::Instance());
	}
}

void enterShopToBuyClothes::exit(Unit* unit)
{
	cout << "\n" << unit->getName() << ": " << "Leave the shop with new pretty clothes!";
}

bool enterShopToBuyClothes::onMessage(Unit* unit, const telegram& msg)
{
	return false;
}

///////////////////////////////////////
//-------GoToTheRestaurant----------//
goToTheRestaurant* goToTheRestaurant::Instance()
{
	static goToTheRestaurant instance;
	return &instance;
}

void goToTheRestaurant::enter(Unit* unit)
{
	if (unit->Location() != restaurant)
	{
		cout << "\n" << unit->getName() << ": " << "Going to the restaurant to get some food!";

		unit->changeLocation(restaurant);
	}
}

void goToTheRestaurant::execute(Unit* unit)
{
	if (unit->moneyCarried() > 20 && unit->getHunger() < 95)
	{
		unit->increaseHunger(10);
		unit->increaseThirst(5);
		unit->decreaseMoneyCarried(20);

		unit->executeMessage(unit->Location());
	}
	else
	{
		unit->GetFSM()->changeState(goToWork::Instance());
	}
}

void goToTheRestaurant::exit(Unit* unit)
{
	cout << "\n" << unit->getName() << ": " << "Leave the restaurant with my belly full!";
}

bool goToTheRestaurant::onMessage(Unit* unit, const telegram& msg)
{
	return false;
}

////////////////////////////////////
//-------GoHomeAndSleep----------//
goHomeAndSleep* goHomeAndSleep::Instance()
{
	static goHomeAndSleep instance;
	return &instance;
}

void goHomeAndSleep::enter(Unit* unit)
{
	if (unit->Location() != unit->getHome(unit->getID()))
	{
		cout << "\n" << unit->getName() << ": " << "Walking home";
		unit->changeLocation(unit->getHome(unit->getID()));
	}
}

void goHomeAndSleep::execute(Unit* unit)
{
	unit->increaseFatigue(10);
	
	unit->executeMessage(unit->Location());

	if (unit->getFatigue() >= 95)
	{
		unit->GetFSM()->changeState(goToWork::Instance());
	}
}

void goHomeAndSleep::exit(Unit* unit)
{

}

bool goHomeAndSleep::onMessage(Unit* unit, const telegram& msg)
{
	return false;
}

////////////////////////////////
//-------GoToTheBar----------//
goToTheBar* goToTheBar::Instance()
{
	static goToTheBar instance;
	return &instance;
}

void goToTheBar::enter(Unit* unit)
{
	if (unit->Location() != pub)
	{
		cout << "\n" << unit->getName() << ": " << "Going to the pub to get something to drink!";

		unit->changeLocation(pub);
	}
}

void goToTheBar::execute(Unit* unit)
{
	if (unit->moneyCarried() > 20 && unit->getThirst() < 95)
	{
		if (unit->getMeetUp())
			unit->increaseSocialNeed(20);

		unit->increaseThirst(10);
		unit->decreaseMoneyCarried(20);

		unit->executeMessage(unit->Location());
	}
	else
	{
		unit->setMeetUp(false);
		unit->GetFSM()->changeState(goToWork::Instance());
	}
}

void goToTheBar::exit(Unit* unit)
{
	cout << "\n" << unit->getName() << ": " << "Leaving the pub satisfied!";
}

bool goToTheBar::onMessage(Unit* unit, const telegram& msg)
{
	return false;
}

///////////////////////////
//-------Death----------//
death* death::Instance()
{
	static death instance;
	return &instance;
}

void death::enter(Unit* unit)
{
	if (unit->Location() != graveyard)
	{
		cout << "\n" << unit->getName() << ": " << "I'm dead. I must find peace in the graveyard!";

		unit->changeLocation(graveyard);
	}
}

void death::execute(Unit* unit)
{

}

void death::exit(Unit* unit)
{

}

bool death::onMessage(Unit* unit, const telegram& msg)
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
	if (unit->getHunger() > 1 && unit->getThirst() > 1 && unit->getFatigue() > 1)
	{
		if ((unit->getHunger() < 40 && unit->randomizer(0, 1) < unit->getChance()) || unit->getHunger() < 10)
		{
			unit->GetFSM()->changeState(goToTheRestaurant::Instance());
		}
		else if ((unit->getThirst() < 40 && unit->randomizer(0, 1) < unit->getChance()) || unit->getThirst() < 10)
		{
			unit->GetFSM()->changeState(goToTheBar::Instance());
		}
		else if ((unit->getFatigue() < 30 && unit->randomizer(0, 1) < unit->getChance()) || unit->getFatigue() < 10)
		{
			unit->GetFSM()->changeState(goHomeAndSleep::Instance());
		}
		else if (unit->getNewClothes() == true)
		{
			unit->GetFSM()->changeState(enterShopToBuyClothes::Instance());
		}
		else if (unit->getSocialNeed() < 30 && unit->randomizer(0, 1) < unit->getChance())
		{
			locationList loc;
			if (unit->getID() == 0)
			{
				dispatcher->sendMessage(0.0f, unit->getID(), 1, msg_wantToMeet, 0);
				dispatcher->sendMessage(0.0f, unit->getID(), 2, msg_wantToMeet, 0);
				dispatcher->sendMessage(0.0f, unit->getID(), 3, msg_wantToMeet, 0);
			}		
			else if (unit->getID() == 1)
			{
				dispatcher->sendMessage(0.0f, unit->getID(), 0, msg_wantToMeet, 0);
				dispatcher->sendMessage(0.0f, unit->getID(), 2, msg_wantToMeet, 0);
				dispatcher->sendMessage(0.0f, unit->getID(), 3, msg_wantToMeet, 0);
			}
			else if (unit->getID() == 2)
			{
				dispatcher->sendMessage(0.0f, unit->getID(), 0, msg_wantToMeet, 0);
				dispatcher->sendMessage(0.0f, unit->getID(), 1, msg_wantToMeet, 0);
				dispatcher->sendMessage(0.0f, unit->getID(), 3, msg_wantToMeet, 0);
			}
			else
			{ 
				dispatcher->sendMessage(0.0f, unit->getID(), 0, msg_wantToMeet, 0);
				dispatcher->sendMessage(0.0f, unit->getID(), 1, msg_wantToMeet, 0);
				dispatcher->sendMessage(0.0f, unit->getID(), 2, msg_wantToMeet, 0);
			}
		}
	}
	else
	{
			unit->GetFSM()->changeState(death::Instance());
	}
}

void globalStates::exit(Unit* unit)
{

}

bool globalStates::onMessage(Unit* unit, const telegram& msg)
{
	if (msg.msg == msg_wantToMeet)
	{
		if (unit->getHunger() > 50 && unit->getThirst() > 50 && unit->getFatigue() > 50 && !unit->getMeetUp())
		{
			dispatcher->sendMessage(0, unit->getID(), msg.sender, msg_iCanMeet, 0);
			dispatcher->sendMessage(5.0f, unit->getID(), msg.sender, msg_meetUp, 0);
			dispatcher->sendMessage(5.0f, unit->getID(), unit->getID(), msg_meetUp, 0);
		}
		else
		{
			dispatcher->sendMessage(0, unit->getID(), msg.sender, msg_iCantMeet, 0);
		}
		return true;
	}
	else if (msg.msg == msg_meetUp)
	{
		unit->GetFSM()->changeState(goToTheBar::Instance());
		unit->setMeetUp(true);
		return true;
	}
	
	return true;	
}

