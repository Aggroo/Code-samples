#include "entityManager.h"
#include "Object.h"

entityManager* entityManager::Instance()
{
	static entityManager instance;
	return &instance;
}

void entityManager::registerUnit(Object* newUnit)
{
	unitMap.insert(std::make_pair(newUnit->getID(), newUnit));
}

Object* entityManager::getUnitFromID(int id) const
{
	entityMap::const_iterator ent = unitMap.find(id);

	assert((ent != unitMap.end()) && "<EntityManager::GetUnitFromID>: invalid ID");

	return ent->second;
}

void entityManager::removeUnit(Object* unit)
{
	unitMap.erase(unitMap.find(unit->getID()));
}
