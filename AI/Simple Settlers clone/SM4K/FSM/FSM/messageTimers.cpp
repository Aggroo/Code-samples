#include "messageTimers.h"
#include "SettingsManager.h"


messageTimers* messageTimers::Instance()
{
	static messageTimers instance;
	return &instance;
}

messageTimers::messageTimers()
{
	SettingsManager* sManager = SettingsManager::Instance();

	sManager->CreateReader("settings/settings.ini", "timerSettings");
	createExplorer= sManager->GetReader("timerSettings").GetFloat(3.0f, "MessageDelays", "createExplorer");
	createBuilder = sManager->GetReader("timerSettings").GetFloat(3.0f, "MessageDelays", "createBuilder");
	createCoalBurner = sManager->GetReader("timerSettings").GetFloat(3.0f, "MessageDelays", "createCoalBurner");
	harvestWood = sManager->GetReader("timerSettings").GetFloat(3.0f, "MessageDelays", "harvestWood");
	produceCoal = sManager->GetReader("timerSettings").GetFloat(3.0f, "MessageDelays", "produceCoal");
	buildingDone = sManager->GetReader("timerSettings").GetFloat(3.0f, "MessageDelays", "buildingDone");
}
