#include "AISystem.h"
#include "graphLoader.h"
#include "GraphAlgorithms.h"
#include <random>
#include "Unit.h"
#include "UnitHandler.h"
#include "TileHandler.h"
#include "shapeGenerators.hpp"
#include "HiveMind.h"


AISystem* AISystem::Instance()
{
	static AISystem instance;
	return &instance;
}


NavGraph* AISystem::GetGraph()
{
	return graph;
}

NavGraph* AISystem::GetWorkerGraph()
{
	return workerGraph;
}

float AISystem::GetTileSpeed(int ID)
{
	if (TileHandler::Instance()->GetTileAtPos(UnitHandler::Instance()->GetUnit(ID)->GetPosition())->GetType() == tileType::swamp)
	{
		return 0.5f;
	}
	else
		return 1.0f;
}

bool AISystem::GetTreeFound()
{
	return treeFound;
}

void AISystem::FoW(int ID)
{
	Unit* unit = UnitHandler::Instance()->GetUnit(ID);
	
	int nodeIndex;
	PointToIndex(unit->GetPosition(), nodeIndex);

	Object* tile = TileHandler::Instance()->GetTileAtPos(nodeIndex);

	workerGraph->GetNode(nodeIndex).SetIndex(nodeIndex);
	tile->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	DiscoverNeighbourTiles(nodeIndex, tile->getWidth(), tile->getHeight());
	
	//if (grid.size() < 1)
	//{
	//	DrawGraph(*graph, grid, programID);
	//}
	/*grid.clear();
	DrawGraph(*workerGraph, grid, programID);*/
}

void AISystem::DiscoverNeighbourTiles(int index, int cellWidth, int cellHeight)
{
	for (int i = -1; i < 2; ++i)
	{
		for (int j = -1; j < 2; ++j)
		{
			if (i == 0 && j == 0)
				continue;
			TileHandler* tiles = TileHandler::Instance();
			if (graph->isNodePresent(index + (i*tiles->GetMapSize().x) + j) && abs((index+(i*100)+j) % 100) - (index % 100) <= 1)
			{
				Tile* tile = TileHandler::Instance()->GetTileAtPos((index + (i*TileHandler::Instance()->GetMapSize().x) + j));
				
				if (workerGraph->GetNode((index + (i*TileHandler::Instance()->GetMapSize().x) + j)).Index() == invalid_node_index)
				{
					workerGraph->GetNode((index + (i*TileHandler::Instance()->GetMapSize().x) + j)).SetIndex((index + (i*TileHandler::Instance()->GetMapSize().x) + j));
				}
				
				tile->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));


				if (graph->GetNode((index + (i*TileHandler::Instance()->GetMapSize().x) + j)).GetType() != NavType::cliff && graph->GetNode((index + (i*TileHandler::Instance()->GetMapSize().x) + j)).GetType() != NavType::water)
				{
					if ((i == -1 && j == -1) && (graph->GetNode((index + ((i + 1)*TileHandler::Instance()->GetMapSize().x) + j)).GetType() == NavType::cliff || graph->GetNode((index + ((i + 1)*TileHandler::Instance()->GetMapSize().x) + j)).GetType() == NavType::water) || (graph->GetNode(index + (i*TileHandler::Instance()->GetMapSize().x) + (j + 1)).GetType() == NavType::cliff || graph->GetNode((index + (i*TileHandler::Instance()->GetMapSize().x) + (j + 1))).GetType() == NavType::water))
					{
						continue;
					}
					else if ((i == -1 && j == 1) && (graph->GetNode((index + ((i + 1)*TileHandler::Instance()->GetMapSize().x) + j)).GetType() == NavType::cliff || graph->GetNode((index + ((i + 1)*TileHandler::Instance()->GetMapSize().x) + j)).GetType() == NavType::water) || (graph->GetNode(index + (i*TileHandler::Instance()->GetMapSize().x) + (j - 1)).GetType() == NavType::cliff || graph->GetNode((index + (i*TileHandler::Instance()->GetMapSize().x) + (j - 1))).GetType() == NavType::water))
					{
						continue;
					}
					if ((i == 1 && j == -1) && (graph->GetNode((index + ((i-1)*TileHandler::Instance()->GetMapSize().x) + j)).GetType() == NavType::cliff || graph->GetNode((index + ((i - 1)*TileHandler::Instance()->GetMapSize().x) + j)).GetType() == NavType::water) || (graph->GetNode(index + (i*TileHandler::Instance()->GetMapSize().x) + (j + 1)).GetType() == NavType::cliff || graph->GetNode((index + (i*TileHandler::Instance()->GetMapSize().x) + (j + 1))).GetType() == NavType::water))
					{
						continue;
					}
					else if ((i == 1 && j == 1) && (graph->GetNode((index + ((i - 1)*TileHandler::Instance()->GetMapSize().x) + j)).GetType() == NavType::cliff || graph->GetNode((index + ((i - 1)*TileHandler::Instance()->GetMapSize().x) + j)).GetType() == NavType::water) || (graph->GetNode(index + (i*TileHandler::Instance()->GetMapSize().x) + (j - 1)).GetType() == NavType::cliff || graph->GetNode((index + (i*TileHandler::Instance()->GetMapSize().x) + (j - 1))).GetType() == NavType::water))
					{
						continue;
					}

					NavGraph::EdgeType newEdge(index, (index + (i*TileHandler::Instance()->GetMapSize().x) + j), 1.0f);
					workerGraph->AddEdge(newEdge);
				}
				
				if (tile->GetType() == tileType::trees && !tile->GetDiscovered())
				{
					for (int i = 0; i < tile->GetTreeList()->size(); i++)
					{
						tile->GetTreeList()->at(i)->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));;
					}
					treeFound = true;
					tile->SetDiscovered();
					HiveMind::Instance()->GetPrioQueue()->push_back(HiveMind::Instance()->GetTaskListValue(HarvestWood));
					
				}
				
				
			}
		}
	}
}

AISystem::AISystem()
{
	graph = new NavGraph(true);
	workerGraph = new NavGraph(true);
	createGraph(*graph, "map3.txt", 32, 32, 100, 100);
	createEmptyGraph(*workerGraph, "map3.txt", 32, 32, 100, 100);
	treeFound = false;
}

std::vector<glm::vec2> AISystem::CalculatePath(glm::vec2 startPos, glm::vec2 goalPos, int id)
{
	int startNodeIndex;
	int goalNodeIndex;
	PointToIndex(startPos, startNodeIndex);
	PointToIndex(goalPos, goalNodeIndex);
	AStarSearch<NavGraph> AST(graph, startNodeIndex, goalNodeIndex, goalPos);
	if (AST.Found())
	{	
		path = AST.GetPathToTarget();
		linePath.clear();
		DrawPath(*graph, path, linePath, programID);
		return path;
	}
	else
	{
		return CalculatePath(UnitHandler::Instance()->GetUnit(id)->GetPosition(), glm::vec2(AISystem::Instance()->randomizer(40, 3168), AISystem::Instance()->randomizer(40, 3168)), id);
	}
	//DrawPath(*graph, path, linePath, programID);
	
}

std::vector<glm::vec2> AISystem::CalculateWorkerPath(glm::vec2 startPos, glm::vec2 goalPos, int id)
{
	int startNodeIndex;
	int goalNodeIndex;
	PointToIndex(startPos, startNodeIndex);
	PointToIndex(goalPos, goalNodeIndex);
	AStarSearch<NavGraph> AST(workerGraph, startNodeIndex, goalNodeIndex,goalPos);
	if (AST.Found())
	{
		path = AST.GetPathToTarget();
		return path;
	}
	else
	{
		return CalculateWorkerPath(startPos, goalPos, id);
	}
}

std::vector<glm::vec2> AISystem::CalculatePathToClosestTrees(glm::vec2 startPos, int id)
{
	int startNodeIndex;
	PointToIndex(startPos, startNodeIndex);
	//PointToIndex(goalPos, goalNodeIndex);
	BreadthFirstItemSearch<NavGraph> BFIS(*workerGraph, startNodeIndex);

	path = CalculateWorkerPath(startPos, BFIS.treeFound(), id);
	linePath.clear();
	DrawPath(*workerGraph, path, treePath, programID);
	return path;
}

bool AISystem::PointToIndex(glm::vec2 p, int& NodeIndex)
{
	//convert p to an index into the graph
	int x = (int)((float)(p.x) / 32);
	int y = (int)((float)(p.y) / 32);

	//make sure the values are legal
	if ((x > 100) || (y > 100))
	{
		NodeIndex = -1;

		return false;
	}

	NodeIndex = y * 100 + x;

	return true;
}

float AISystem::randomizer(float minVal, float maxVal)
{
	float random;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(minVal, maxVal);

	random = dis(gen);
	return random;
}

