#pragma once

#include <ostream>
#include <fstream>
#include "NodeTypeEnum.h"

/************************************************************************/
/*             GraphEdge - Handles the structure of a edge              */
/************************************************************************/
class GraphEdge
{
protected:

	//The nodes the edge are connected to
	int from, to;

	//the cost to traverse the edge
	float cost;

public:

	//Constructors
	GraphEdge(int f, int t, float c) : from(f), to(t), cost(c){}
	GraphEdge(int f, int t) : from(f), to(t), cost(1.0f){}
	GraphEdge() :from(invalid_node_index), to(invalid_node_index), cost(1.0f) {}

	//Getters and setters
	int GetFrom() const { return from; }
	void SetFrom(int val) { from = val; }

	int GetTo() const { return to; }
	void SetTo(int val) { to = val; }

	float GetCost() const { return cost; }
	void SetCost(float val) { cost = val; }

};