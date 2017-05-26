#pragma once
#include "Object.h"

/*****************************************/
/*			TileHandle - Class			 */
/*		   Handles a tree object		 */
/*****************************************/
class TreeEntity : public Object
{
public:

	TreeEntity(const glm::vec4 &color);
	~TreeEntity();

	//Checks and sets if a tree is harvested
	bool isHarvested();
	void Harvested();

	//Checks and sets if a tree is reserved for a unit
	bool isReserved(){ return reserved; }
	void SetReserved(bool val) { reserved = val; }
private:
	
	bool reserved;
	bool harvested;
	
};

