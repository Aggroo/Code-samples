#pragma once
//Glew
#include <GL\glew.h>

//GLFW
#include <GLFW\glfw3.h>

#include <glm.hpp>

#include "SparseGraph.h"
#include "GraphNodeTypes.h"
#include "GraphEdgeTypes.h"
#include "Tile.h"

class Object;

typedef SparseGraph<NavGraphNode<void*>, GraphEdge> NavGraph;

/************************************************************************/
/*						AISystem - Singleton							*/
/*					Handles unit paths and grids						*/
/************************************************************************/
class AISystem
{
public:
	static AISystem* Instance();

	//Calculates an explorers path
	std::vector<glm::vec2> CalculatePath(glm::vec2 startPos, glm::vec2 goalPos, int id);
	//Calculates the path for a worker
	std::vector<glm::vec2> CalculateWorkerPath(glm::vec2 startPos, glm::vec2 goalPos, int id);
	//Breadth first search for the closest available tree
	std::vector<glm::vec2> CalculatePathToClosestTrees(glm::vec2 startPos, int id);

	//Converts a position to a nodeIndex
	bool PointToIndex(glm::vec2 p, int& NodeIndex);
	//Randomizes a value between two intervalls
	float randomizer(float minVal, float maxVal);
	//Gets the full map Navgraph
	NavGraph* GetGraph();

	//Gets the explored map Navgraph
	NavGraph* GetWorkerGraph();

	//Return the graph grid
	std::vector<Object>* GetGrid(){ return &grid; }

	//Returns the linePath
	std::vector<Object>* GetLinePath(){ return &linePath; }

	//Getters and setters
	float GetTileSpeed(int ID);
	bool GetTreeFound();

	void SetProgramID(GLuint programID){ this->programID = programID; }

	//Fog of war. Reveals the 8 tiles around a unit
	void FoW(int ID);

	//Discover the neighbour tiles around a specific tile
	void DiscoverNeighbourTiles(int index, int cellWidth, int cellHeight);

private:
	AISystem();
	AISystem(const AISystem&) = delete;
	AISystem& operator=(const AISystem&) = delete;

	//ProgramID used for openGL
	GLuint programID;

	//The two graphs
	NavGraph* graph;
	NavGraph* workerGraph;

	//Walking path of a unit
	std::vector<glm::vec2> path;
	
	//Different paths(used for debugging, hold line objects).
	std::vector<Object> grid;
	std::vector<Object> linePath;
	std::vector<Object> treePath;

	//Checks if a treetile is found
	bool treeFound;
};

