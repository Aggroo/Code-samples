#pragma once

#include <ostream>
#include <fstream>
#include "NodeTypeEnum.h"

enum class NavType{unwalkable, ground, start, goal};

/************************************************************************/
/*                     GraphNode baseclass                              */
/************************************************************************/
class GraphNode
{
protected:

	//Index of a node. Valid if index >= 0
	int index;

public:

	//Constructors
	GraphNode(): index(invalid_node_index){}
	GraphNode(int indx) : index(indx){}

	//Virtual destructor
	virtual ~GraphNode(){}

	//Get and set for index
	int Index() const{ return index; }
	void SetIndex(int newIndex) { index = newIndex; }

};


/************************************************************************/
/*                     NavGraphNode subclass                            */
/*   Contains the position of a node and a pointer to a baseGameEnity   */
/************************************************************************/
template <class extra_info = void*>
class NavGraphNode : public GraphNode
{
protected:

	//Position of Node
	glm::vec2 position;

	NavType type;

	//Extra info if needed. For example if it's a health pack, a resource or something else
	extra_info extraInfo;

public:

	//Constructors
	NavGraphNode() : extraInfo(extra_info()), type(NavType::ground){}
	NavGraphNode(int idx, glm::vec2 pos, NavType type) : GraphNode(idx), position(pos), extraInfo(extra_info()), type(type){}

	//Virtual destructor
	virtual ~NavGraphNode(){}

	//Getters and Setters
	glm::vec2 GetPosition()const{ return position; }
	void SetPosition(glm::vec2 newPos){ position = newPos; }

	extra_info GetExtraInfo() const{ return extraInfo; }
	void SetExtraInfo(extra_info newInfo){ extraInfo = newInfo; }

	NavType GetType() const { return type; }
	void SetType(NavType val) { type = val; }

	//Returns true if the node is walkable
	bool isWalkable()
	{
		return (type != NavType::unwalkable);
	}

	
};