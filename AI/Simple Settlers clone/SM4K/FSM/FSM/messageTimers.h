#pragma once

/************************************************************************/
/*       Holds all the settings.ini variables for MessageDelays         */
/************************************************************************/
class messageTimers
{
public:
	//Singleton class
	static messageTimers* Instance();

	//Gets the different floats
	float GetCreateExplorer() { return createExplorer; }
	float GetCreateBuilder() { return createBuilder; }
	float GetCreateCoalBurner() { return createCoalBurner; }
	float GetHarvestWood() { return harvestWood; }
	float GetProduceCoal() { return produceCoal; }
	float GetBuildingDone() { return buildingDone; }

private:
	//Uses SettingsManager to read the settings.ini files @MessageDelays
	messageTimers();
	messageTimers(const messageTimers&) = delete;
	messageTimers& operator=(const messageTimers&) = delete;

	float createExplorer, createBuilder, createCoalBurner;
	float harvestWood, produceCoal;
	float buildingDone;
};

