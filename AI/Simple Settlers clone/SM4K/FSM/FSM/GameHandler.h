#pragma once
#include <map>
#include <vector>
#include "TreeEntity.h"

//Used for global game handling (Barely used)
class GameHandler
{
public:
	//Singleton instance
	static GameHandler* Instance();

	//Gets the list of trees
	std::vector<TreeEntity*>* GetTreeList(){ return &treeList; }
private:

	//Holds all the trees in the map
	std::vector<TreeEntity*> treeList;

	//constructors
	GameHandler(){}
	GameHandler(const GameHandler&) = delete;
	GameHandler& operator=(const GameHandler&) = delete;
};