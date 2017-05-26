#include "UnitHandler.h"
#include "shapeGenerators.hpp"
#include "AISystem.h"


UnitHandler* UnitHandler::Instance()
{
	static UnitHandler instance;
	return &instance;
}

void UnitHandler::CreateUnit(Model* model, glm::vec2 position, glm::vec4 color, profession prof)
{ 
	Unit* triangleObject = new Unit(color, prof, id);
	triangleObject->setModel(model);
	triangleObject->setPosition(position.x, position.y);
	triangleObject->SetStartPosition(position);
	objects.push_back(triangleObject);
	id++;
}

void UnitHandler::CreateDummy(Model* model, glm::vec2 position, glm::vec4 color, profession prof)
{
	Unit* dummy = new Unit(color, prof, NULL);
	dummy->setModel(model);
	dummy->setPosition(position.x, position.y);
	objects.push_back(dummy);
}

std::vector<Unit*>* UnitHandler::GetUnits()
{
	return &objects;
}

Unit* UnitHandler::GetUnit(int ID)
{
	return objects[ID];
}

void UnitHandler::SetProgramID(GLuint programID)
{
	this->programID = programID;
}

void UnitHandler::SetupUnitModels()
{
	Model worker = shapeGenerator::square(4, 5, programID, "textures/unit.png");
	worker.init();
	unitModels[profession::worker] = worker;
	Model explorer = shapeGenerator::square(4, 5, programID, "textures/unitExplorer.png");
	explorer.init();
	unitModels[profession::explorer] = explorer;
	Model coalBurner = shapeGenerator::square(4, 5, programID, "textures/unitCoal.png");
	coalBurner.init();
	unitModels[profession::coalBurner] = coalBurner;
	Model builder = shapeGenerator::square(4, 5, programID, "textures/unitBuilder.png");
	builder.init();
	unitModels[profession::builder] = builder;
	Model workerTree = shapeGenerator::square(4, 5, programID, "textures/unitWood.png");
	workerTree.init();
	unitModels[profession::workerTree] = workerTree;
	
}

Model* UnitHandler::GetUnitModel(profession prof)
{
	return &unitModels[prof];
}

UnitHandler::UnitHandler()
{
	id = 1;
}

UnitHandler::~UnitHandler()
{
	for (int i = 0; i < objects.size(); i++)
	{
		delete objects[i];
	}
}

