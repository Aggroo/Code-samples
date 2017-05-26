#include "GameHandler.h"
#include "Object.h"

GameHandler* GameHandler::Instance()
{
	static GameHandler instance;
	return &instance;
}

