#include "Unit.h"
#include "AISystem.h"
#include "SettingsManager.h"
#include "graphLoader.h"
#include <gtx\perpendicular.hpp>
#include "TileHandler.h"
#include "UnitHandler.h"
#include <algorithm>
#include "UnitStates.h"
#include "messageDispatcher.h"


Unit::Unit(const glm::vec4 &color, profession prof, int ID) : Object(color), stateMachine(this), building(false), harvesting(false)
{
	stateMachine.setCurrentState(idle::Instance());
	stateMachine.setGlobalState(globalStates::Instance());
	//setPosition(100, 100);
	this->prof = prof;
	this->ID = ID;
	SettingsManager* sManager = SettingsManager::Instance();
	sManager->CreateReader("settings/settings.ini", "settings");
	speed = sManager->GetReader("settings").GetFloat(5.0f, "Agents", "MoveSpeed");
	maxSpeed = sManager->GetReader("settings").GetFloat(5.0f, "Agents", "maxSpeed");
	mass = sManager->GetReader("settings").GetFloat(5.0f, "Agents", "mass");
	radius = sManager->GetReader("settings").GetFloat(1.0f, "Agents", "radius");
	deceleration = sManager->GetReader("settings").GetFloat(0.5f, "Agents", "deceleration");
	//turnSpeed = sManager->GetReader("settings").GetFloat(0.5f, "Agents", "turnSpeed");
	minDetectionLength = sManager->GetReader("settings").GetFloat(0.3f, "Agents", "minDetectionLength");
	aiSystem = AISystem::Instance();
	unitHandler = UnitHandler::Instance();
	radiusSearch = 0;

	firstRun = true;
	walkingPath = false;

	
	
}
	


Unit::~Unit()
{
}
/********************************
*		ALL LOGIC GOES HERE		*
*		EXAMPLE YOUR AI!!!!		*
*********************************/
void Unit::update(float deltaTime)
{
	this->deltaTime = deltaTime;

	stateMachine.update(deltaTime);
	dispatcher->sendDelayedMessage();
	if (prof == explorer)
	{
		if (firstRun)
		{
			firstRun = false;
			//SetTarget(glm::vec2(aiSystem->randomizer(40, 3168), aiSystem->randomizer(40, 3168)), ID);
			SetTarget(glm::vec2(aiSystem->randomizer(unitHandler->GetHomePosition().x - 200, unitHandler->GetHomePosition().x + 200), aiSystem->randomizer(unitHandler->GetHomePosition().y - 200, unitHandler->GetHomePosition().y + 200)), ID);
			radiusSearch++;
			//printf("\nRadiusSearch: %i\n", radiusSearch);
		}

		if (this->GetPosition() != goalPosition)
		{
			moveToPosition();
			aiSystem->FoW(ID);
		}
		else
		{
			
			if (radiusSearch < 5)
			{
				SetTarget(glm::vec2(aiSystem->randomizer(unitHandler->GetHomePosition().x - 200, unitHandler->GetHomePosition().x + 200), aiSystem->randomizer(unitHandler->GetHomePosition().y - 200, GetPosition().y + 200)), ID);
				radiusSearch++;
				//printf("RadiusSearch: %i\n", radiusSearch);
			}
			else
			{
				SetTarget(glm::vec2(aiSystem->randomizer(60, 3100), aiSystem->randomizer(60, 3100)), ID);
			}
			
		}
	}

	//else
	//{
	//	aiSystem->FoW(ID);
	//}
	
	
}

bool Unit::handleMessage(const telegram& msg)
{
	return stateMachine.handleMessage(msg);
}

StateMachine<Unit>* Unit::GetFSM()
{
	return &stateMachine;
}


void Unit::moveToPosition()
{

	if (prof == explorer)
	{
		if (closeToEqual(GetPosition(), path.back(), radius))
		{
			//printf("Unit %i: Walking towards new pos %f, %f\n", ID, path.back().x, path.back().y);
			path.pop_back();
		}
	}
	else
	{
		if (closeToEqual(GetPosition(), path.back(), radius-3))
		{
			//printf("Unit %i: Walking towards new pos %f, %f\n", ID, path.back().x,path.back().y);
			path.pop_back();
		}
	}

	
	if (path.size() < 1)
	{
		goalPosition = this->GetPosition();
		return;
	}

	//glm::vec2 steeringForce = Seek(aiSystem->GetGraph()->GetNode(path.back()).GetPosition());
	glm::vec2 steeringForce = Arrive(path.back());
	glm::vec2 acceleration = steeringForce / mass;
	glm::vec2 locVec = path.back();

	velocity += acceleration*deltaTime;
	velocity = glm::normalize(velocity)*glm::max(glm::length(velocity), maxSpeed);

	//glm::vec2 deltaDist = (locVec - GetPosition());


	posX += velocity.x*deltaTime*aiSystem->GetTileSpeed(ID);
	posY += velocity.y*deltaTime*aiSystem->GetTileSpeed(ID);
	//printf("Unit %i: \nVelocity: X: %f Y: %f\n PosX: %f PosY: %f\n", ID, velocity.x, velocity.y, posX, posY);
	modelMatrix = getModelMatrix();

	if (glm::length(velocity) > 0.00000001)
	{
		heading = glm::normalize(velocity);

		side = glm::perp(side, heading);
		
	}

}


glm::vec2 Unit::Seek(glm::vec2 targetPos)
{
	glm::vec2 desiredVelocity = glm::normalize(targetPos - GetPosition())*maxSpeed;

	return (desiredVelocity - velocity);
}

glm::vec2 Unit::Arrive(glm::vec2 targetPos)
{
	glm::vec2 toTarget = targetPos - GetPosition();

	float dist = glm::length(toTarget);
	if (dist > 0)
	{

		const float DecelerationTweaker = 0.3;

		float speed = dist / (deceleration * DecelerationTweaker);

		speed = std::min(speed, maxSpeed);

		glm::vec2 DesiredVelocity = toTarget * speed / dist;
		return (DesiredVelocity - velocity);
	}
	return glm::vec2(0, 0);
}

//glm::vec2 Unit::ObstacleAvoidance(std::vector<Object>* obstacles)
//{
//	//TODO: Implement
//}

std::vector<glm::vec2> Unit::GetPath()
{
	return path;
}

void Unit::SetTarget(glm::vec2 pos, int id)
{
	goalPosition = pos;
	path = aiSystem->CalculatePath(GetPosition(), goalPosition, id);
	//printf("Unit %i: Walking towards a new path: X: %f Y: %f\n", ID, goalPosition.x, goalPosition.y);
}

profession Unit::GetProfession()
{
	return prof;
}

void Unit::SetProfession(profession prof)
{
	this->prof = prof;
}

void Unit::searchTrees()
{	
	path = aiSystem->CalculatePathToClosestTrees(GetPosition(), ID);
	goalPosition = TileHandler::Instance()->GetTileAtPos(path[0])->GetPosition();
}

bool Unit::closeToEqual(glm::vec2 pos1, glm::vec2 pos2, float radius)
{
	glm::vec2 delta = pos2 - pos1;
	if (abs(delta.x) < radius && abs(delta.y) < radius)
		return true;
	else
		return false;
}

int Unit::GetID()
{
	return ID;
}

