#pragma once
#include "Object.h"
#include "TreeEntity.h"

enum tileType
{
	cliff,
	water,
	grass,
	trees,
	tree,
	swamp
};

/**********************************************/
/*					Tile Class				  */
/*				Handles a tiles values		  */
/**********************************************/
class Tile : public Object
{
public:
	Tile(const glm::vec4 &color);
	~Tile();

	//Sets and Gets tileType
	void SetType(tileType type){ this->type = type; }
	tileType GetType(){ return type; }

	//Gets the treeList
	std::vector<TreeEntity*>* GetTreeList() { return &treeList; }

	//Gets, sets, increases and decreases the amount of wood on a single tile
	int GetTreeAmount() const { return treeAmount; }
	void SetTreeAmount(int val) { treeAmount = val; }
	void IncreaseTreeAmount() { treeAmount++; }
	void DecreaseTreeAmountByTwo() { treeAmount -= 2; }

	//Checks if the tile is discovered. (Right now only used for tree tiles)
	bool GetDiscovered() const { return discovered; }
	void SetDiscovered() { discovered = !discovered; }

	//Checks if a building is on the tile
	bool GetBuilding() { return building; }
	void SetBuilding(bool val) { building = val; }
private:
	tileType type;

	bool discovered;
	bool building;
	int treeAmount;

	std::vector<TreeEntity*> treeList;
};

