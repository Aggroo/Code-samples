#pragma once
//Glew
#include <GL\glew.h>

//GLFW
#include <GLFW\glfw3.h>

#include <glm.hpp>

/************************************************************************/
/*               Creates a graph based on a 2D map                      */
/************************************************************************/
template <typename graph_type>
void createGraph(graph_type& graph, const char* filename, int xSize, int ySize, int cellX, int cellY)
{
	float cellMidX = (float)xSize*0.5f;
	float cellMidY = (float)ySize*0.5f;

	std::string* mapArray = new std::string[cellY*cellX];

	for (int y = 0; y < cellY; y++)
	{
		for (int x = 0; x < cellX; x++)
		{
			mapArray[y*cellX + x] = "";
		}
	}

	int squareSizeX = xSize;
	int squareSizeY = ySize;

	FILE* file;
	file = fopen(filename, "r");
	char c;
	int i = 0;
	while ((c = fgetc(file)) != EOF) {

		if (c != '\n')
		{
			mapArray[i] = c;
			i++;
		}

	}
	fclose(file);

	for (int y = 0; y < cellY; y++)
	{
		for (int x = 0; x < cellX; x++)
		{
			if (mapArray[y*cellX + x] == "B")
				graph.AddNode(NavGraphNode<void*>(graph.GetNextFreeNodeIndex(), glm::vec2(cellMidX + (x*xSize), cellMidY + (y*ySize)), NavType::cliff));
			else if (mapArray[y*cellX + x] == "V")
			{
				graph.AddNode(NavGraphNode<void*>(graph.GetNextFreeNodeIndex(), glm::vec2(cellMidX + (x*xSize), cellMidY + (y*ySize)), NavType::water));
			}		
			else if (mapArray[y*cellX + x] == "G")
			{
				graph.AddNode(NavGraphNode<void*>(graph.GetNextFreeNodeIndex(), glm::vec2(cellMidX + (x*xSize), cellMidY + (y*ySize)), NavType::swamp));
			}	
			else if (mapArray[y*cellX + x] == "T")
			{
				graph.AddNode(NavGraphNode<void*>(graph.GetNextFreeNodeIndex(), glm::vec2(cellMidX + (x*xSize), cellMidY + (y*ySize)), NavType::trees));
			}
			else
				graph.AddNode(NavGraphNode<void*>(graph.GetNextFreeNodeIndex(), glm::vec2(cellMidX + (x*xSize), cellMidY + (y*ySize)), NavType::grass));
		}
	}

	for (int y = 0; y < cellY; y++)
	{
		for (int x = 0; x < cellX; x++)
		{
			if (graph.nodes[y*cellX + x].GetType() != NavType::cliff && graph.nodes[y*cellX + x].GetType() != NavType::water)
				AddAllNeighbours(graph, x, y, cellX, cellY);
		}
	}

}

/************************************************************************/
/*               Creates a empty graph based on a 2D map                */
/************************************************************************/
template <typename graph_type>
void createEmptyGraph(graph_type& graph, const char* filename, int xSize, int ySize, int cellX, int cellY)
{
	float cellMidX = (float)xSize*0.5f;
	float cellMidY = (float)ySize*0.5f;

	std::string* mapArray = new std::string[cellY*cellX];

	for (int y = 0; y < cellY; y++)
	{
		for (int x = 0; x < cellX; x++)
		{
			mapArray[y*cellX + x] = "";
		}
	}

	int squareSizeX = xSize;
	int squareSizeY = ySize;

	FILE* file;
	file = fopen(filename, "r");
char c;
int i = 0;
while ((c = fgetc(file)) != EOF) {

	if (c != '\n')
	{
		mapArray[i] = c;
		i++;
	}

}
fclose(file);

for (int y = 0; y < cellY; y++)
{
	for (int x = 0; x < cellX; x++)
	{
		if (mapArray[y*cellX + x] == "B")
		{
			graph.AddNode(NavGraphNode<void*>(graph.GetNextFreeNodeIndex(), glm::vec2(cellMidX + (x*xSize), cellMidY + (y*ySize)), NavType::cliff));
			graph.GetNode(y*cellX + x).SetIndex(invalid_node_index);
		}
		else if (mapArray[y*cellX + x] == "V")
		{
			graph.AddNode(NavGraphNode<void*>(graph.GetNextFreeNodeIndex(), glm::vec2(cellMidX + (x*xSize), cellMidY + (y*ySize)), NavType::water));
			graph.GetNode(y*cellX + x).SetIndex(invalid_node_index);
		}
		else if (mapArray[y*cellX + x] == "G")
		{
			graph.AddNode(NavGraphNode<void*>(graph.GetNextFreeNodeIndex(), glm::vec2(cellMidX + (x*xSize), cellMidY + (y*ySize)), NavType::swamp));
			graph.GetNode(y*cellX + x).SetIndex(invalid_node_index);
		}

		else if (mapArray[y*cellX + x] == "T")
		{
			graph.AddNode(NavGraphNode<void*>(graph.GetNextFreeNodeIndex(), glm::vec2(cellMidX + (x*xSize), cellMidY + (y*ySize)), NavType::trees));
			graph.GetNode(y*cellX + x).SetIndex(invalid_node_index);
		}
		else
		{
			graph.AddNode(NavGraphNode<void*>(graph.GetNextFreeNodeIndex(), glm::vec2(cellMidX + (x*xSize), cellMidY + (y*ySize)), NavType::grass));
			graph.GetNode(y*cellX + x).SetIndex(invalid_node_index);
		}

	}
}

/*for (int y = 0; y < cellY; y++)
{
for (int x = 0; x < cellX; x++)
{
if (graph.nodes[y*cellX + x].GetType() != NavType::cliff && graph.nodes[y*cellX + x].GetType() != NavType::water)
AddAllNeighbours(graph, x, y, cellX, cellY);
}
}*/

}

/************************************************************************/
/*          Adds all edges around a node if the node is walkable        */
/*                         Used by createGraph                          */
/************************************************************************/
template <typename graph_type>
void AddAllNeighbours(graph_type& graph, int x, int y, int cellX, int cellY)
{
	for (int i = -1; i < 2; ++i)
	{
		for (int j = -1; j < 2; ++j)
		{
			int nodeX = x + j;
			int nodeY = y + i;

			if (i == 0 && j == 0)
				continue;
			
			//Horrible pile of if statements
			if (graph.nodes[nodeY*cellX + nodeX].GetType() != NavType::cliff && graph.nodes[nodeY*cellX + nodeX].GetType() != NavType::water)
			{
				if ((i == -1 && j == -1) && ((graph.nodes[y*cellX + (x - 1)].GetType() == NavType::cliff || graph.nodes[y*cellX + (x - 1)].GetType() == NavType::water) || (graph.nodes[(y - 1)*cellX + x].GetType() == NavType::cliff || graph.nodes[(y - 1)*cellX + x].GetType() == NavType::water)))
				{
					continue;
				}
				else if ((i == -1 && j == 1) && ((graph.nodes[y*cellX + (x + 1)].GetType() == NavType::cliff || graph.nodes[y*cellX + (x + 1)].GetType() == NavType::water) || (graph.nodes[(y - 1)*cellX + x].GetType() == NavType::cliff || graph.nodes[(y - 1)*cellX + x].GetType() == NavType::water)))
				{
					continue;
				}
				else if ((i == 1 && j == -1) && ((graph.nodes[y*cellX + (x - 1)].GetType() == NavType::cliff || graph.nodes[y*cellX + (x - 1)].GetType() == NavType::water) || (graph.nodes[(y + 1)*cellX + x].GetType() == NavType::cliff || graph.nodes[(y + 1)*cellX + x].GetType() == NavType::water)))
				{
					continue;

				}
				else if ((i == 1 && j == 1) && ((graph.nodes[y*cellX + (x + 1)].GetType() == NavType::cliff || graph.nodes[y*cellX + (x + 1)].GetType() == NavType::water) || (graph.nodes[(y + 1)*cellX + x].GetType() == NavType::cliff || graph.nodes[(y + 1)*cellX + x].GetType() == NavType::water)))
				{
					continue;
				}
					
				
				glm::vec2 posNode = graph.GetNode(y*cellX + x).GetPosition();
				glm::vec2 posNeighbour = graph.GetNode(nodeY*cellX + nodeX).GetPosition();

				float dist = glm::distance(posNode, posNeighbour);

				graph_type::EdgeType NewEdge(y*cellX + x, nodeY*cellX + nodeX, dist * WeightedCostTerrain(graph, y*cellX + x, nodeY*cellX + nodeX));
				graph.AddEdge(NewEdge);

				if (!graph.isDigraph())
				{
					graph_type::EdgeType NewEdge(nodeY*cellX + nodeX, y*cellX + x, dist);

					graph.AddEdge(NewEdge);
				}
			}

		}
	}
}

/************************************************************************/
/*              Draws the graph made by createGraph                     */
/************************************************************************/
template <class graph_type>
float WeightedCostTerrain(graph_type& graph, int fromIndex,int toIndex)
{
	if (graph.GetNode(fromIndex).GetType() == NavType::swamp || graph.GetNode(toIndex).GetType() == NavType::swamp)
		return 2.0f;
	else
		return 1.0f;

}

/************************************************************************/
/*              Draws the graph made by createGraph                     */
/************************************************************************/
template <class graph_type>
void DrawGraph(graph_type& graph, std::vector<Object>& grid, GLuint programID)
{
	if (graph.NumNodes() == 0)
		return;

	for (int y = 0; y < graph.nodes.size(); y++)
	{
		for (int x = 0; x < graph.edges[y].size(); x++)
		{

			glm::vec2 from = graph.nodes[y].GetPosition();
			glm::vec2 to = graph.GetNode(graph.edges[y][x].GetTo()).GetPosition();

			Model line1 = shapeGenerator::line(from, to, programID);
			line1.initLine(from, to);
			Object lines = Object(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
			lines.setModel(line1);
			lines.setPosition(0, 0);
			grid.push_back(lines);
		}
	}
}


/************************************************************************/
/*           Draws a path based on a pathfinding algorithm              */
/************************************************************************/
template <class graph_type>
void DrawPath(graph_type& graph, std::vector<glm::vec2> path, std::vector<Object>& linePath, GLuint programID)
{
	if (path.size() == 0)
		return;

	for (int i = 0; i < path.size() - 1; i++)
	{
		float fade = (float) i / (float)path.size();
		glm::vec2 from = path[i];
		glm::vec2 to = path[i + 1];

		Model line1 = shapeGenerator::line(from, to, programID);
		line1.initLine(from, to);
		Object lines = Object(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		lines.setModel(line1);
		lines.setPosition(0, 0);
		linePath.push_back(lines);
	}
}