#pragma once

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
			if (mapArray[y*cellX + x] == "X")
				graph.AddNode(NavGraphNode<void*>(graph.GetNextFreeNodeIndex(), glm::vec2(cellMidX + (x*xSize), cellMidY + (y*ySize)), NavType::unwalkable));
			else if (mapArray[y*cellX + x] == "S")
			{
				graph.AddNode(NavGraphNode<void*>(graph.GetNextFreeNodeIndex(), glm::vec2(cellMidX + (x*xSize), cellMidY + (y*ySize)), NavType::start));
				graph.SetStartIndex(y*cellX + x);
			}
			else if (mapArray[y*cellX + x] == "G")
			{
				graph.AddNode(NavGraphNode<void*>(graph.GetNextFreeNodeIndex(), glm::vec2(cellMidX + (x*xSize), cellMidY + (y*ySize)), NavType::goal));
				graph.SetGoalIndex(y*cellX + x);
			}
			else
				graph.AddNode(NavGraphNode<void*>(graph.GetNextFreeNodeIndex(), glm::vec2(cellMidX + (x*xSize), cellMidY + (y*ySize)), NavType::ground));
		}
	}

	for (int y = 0; y < cellY; y++)
	{
		for (int x = 0; x < cellX; x++)
		{
			if (graph.nodes[y*cellX + x].GetType() != NavType::unwalkable)
				AddAllNeighbours(graph, x, y, cellX, cellY);
		}
	}

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

			if (graph.nodes[nodeY*cellX + nodeX].GetType() != NavType::unwalkable)
			{

				glm::vec2 posNode = graph.GetNode(y*cellX + x).GetPosition();
				glm::vec2 posNeighbour = graph.GetNode(nodeY*cellX + nodeX).GetPosition();

				float dist = glm::distance(posNode, posNeighbour);

				graph_type::EdgeType NewEdge(y*cellX + x, nodeY*cellX + nodeX, dist);
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
			Unit lines = Unit(glm::vec3(1.0f, 0.0f, 0.0f));
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
void DrawPath(graph_type& graph, std::vector<int> path, std::vector<Object>& linePath, GLuint programID)
{
	if (path.size() == 0)
		return;

	for (int i = 0; i < path.size() - 1; i++)
	{
		float fade = (float) i / (float)path.size();
		glm::vec2 from = graph.nodes[path[i]].GetPosition();
		glm::vec2 to = graph.nodes[path[i+1]].GetPosition();

		Model line1 = shapeGenerator::line(from, to, programID);
		Unit lines = Unit(glm::vec3(fade, 1 - fade, fade));
		lines.setModel(line1);
		lines.setPosition(0, 0);
		linePath.push_back(lines);
	}
}