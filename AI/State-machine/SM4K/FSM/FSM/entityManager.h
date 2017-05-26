#pragma once
#include <map>

class Object;

#define entityMgr entityManager::Instance()
//Used to register every new unit
class entityManager
{
public:
	//Singleton instance
	static entityManager* Instance();

	//Registers the unit in a register
	void registerUnit(Object* newUnit);

	//Get a unit from the register with it's id
	Object* getUnitFromID(int id) const;

	//Removes Unit from register
	void removeUnit(Object* unit);

private:

	//Used to save some pain in writing.
	typedef std::map<int, Object*> entityMap;

	//Holds the register
	entityMap unitMap;

	//constructors
	entityManager(){}
	entityManager(const entityManager&) = delete;
	entityManager& operator=(const entityManager&) = delete;
};