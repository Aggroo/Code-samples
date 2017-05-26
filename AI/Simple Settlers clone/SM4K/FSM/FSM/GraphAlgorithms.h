#pragma once
#include <stack>
#include <queue>

/************************************************************************/
/*               Does a Depth First search on a graph                   */
/************************************************************************/
template <class graph_type>
class DepthFirstSearch
{
private:
	//Enum that hold data about a node
	//Used by visitedNode
	enum 
	{
		unvisited,
		visited,
		noParent
	};

	//Shortens code and saves my fingers
	typedef typename graph_type::EdgeType Edge;
	typedef typename graph_type::NodeType Node;


private:
	graph_type& graph;

	//Holds all the visitedNodes
	std::vector<int> visitedNode;
	//Saves the route the algorithm searched
	std::vector<int> route;

	//Start and goal
	int source, target;

	bool pathFound;
	
	//Does the search algorithm
	bool Search();

public:
	//Constructor
	DepthFirstSearch(graph_type& graph, int source, int target = -1) : graph(graph), source(source), target(target), pathFound(false), visitedNode(graph.NumNodes(), unvisited), route(graph.NumNodes(), noParent)
	{
		pathFound = Search();
	}
	
	//Checks if path is found
	bool Found()
	{
		return pathFound;
	}

	//Returns the path from the start to the goal
	std::vector<int> GetPathToTarget();

};


/************************************************************************/
/*                   Depth First search on a graph                      */
/************************************************************************/
template <class graph_type>
bool DepthFirstSearch<graph_type>::Search()
{
	std::stack<Edge*> stack;

	Edge dummy(source, source, 0);

	stack.push(&dummy);

	while (!stack.empty())
	{
		Edge* next = stack.top();

		stack.pop();

		route[next->GetTo()] = next->GetFrom();
		
		visitedNode[next->GetTo()] = visited;

		if (next->GetTo() == target)
		{
			return true;
		}

		int edgesSize = graph.edges[next->GetTo()].size();
		for (int pe = 0; pe < edgesSize; pe++)
		{

			if (visitedNode[graph.edges[next->GetTo()][pe].GetTo()] == unvisited)
			{
				stack.push(&graph.edges[next->GetTo()][pe]);
			}
		}
	}

	return false;
}

template <class graph_type>
std::vector<int> DepthFirstSearch<graph_type>::GetPathToTarget()
{
	std::vector<int> path;

	if (!pathFound || target < 0)
		return path;

	int nd = target;

	path.push_back(nd);

	while (nd != source)
	{
		nd = route[nd];

		path.push_back(nd);
	}

	return path;
}

/************************************************************************/
/*              Does a Breadth First search on a graph                  */
/************************************************************************/
template <class graph_type>
class BreadthFirstSearch
{
private:
	//Enum that hold data about a node
	//Used by visitedNode
	enum
	{
		unvisited,
		visited,
		noParent
	};

	typedef typename graph_type::EdgeType Edge;
	typedef typename graph_type::NodeType Node;


private:
	graph_type& graph;

	//Holds all the visitedNodes
	std::vector<int> visitedNode;
	//Saves the route the algorithm searched
	std::vector<int> route;

	//Start and goal
	int source, target;

	bool pathFound;

	//Starts the search algorithm
	bool Search();

public:

	//Constructor
	BreadthFirstSearch(graph_type& graph, int source, int target = -1) : graph(graph), source(source), target(target), pathFound(false), visitedNode(graph.NumNodes(), unvisited), route(graph.NumNodes(), noParent)
	{
		pathFound = Search();
	}

	//Checks if path is found
	bool Found()
	{
		return pathFound;
	}

	//Returns the path from the start to the goal
	std::vector<int> GetPathToTarget();

};

/************************************************************************/
/*                  Breadth First search on a graph                     */
/************************************************************************/
template <class graph_type>
bool BreadthFirstSearch<graph_type>::Search()
{
	std::queue<Edge*> q;

	Edge dummy(source, source, 0);

	q.push(&dummy);

	visitedNode[source] = visited;

	while (!q.empty())
	{
		Edge* next = q.front();

		q.pop();

		route[next->GetTo()] = next->GetFrom();


		if (next->GetTo() == target)
		{
			return true;
		}

		int edgesSize = graph.edges[next->GetTo()].size();
		for (int pe = 0; pe < edgesSize; pe++)
		{

			if (visitedNode[graph.edges[next->GetTo()][pe].GetTo()] == unvisited)
			{
				q.push(&graph.edges[next->GetTo()][pe]);
				visitedNode[graph.edges[next->GetTo()][pe].GetTo()] = visited;
			}
		}
	}

	return false;
}

template <class graph_type>
std::vector<int> BreadthFirstSearch<graph_type>::GetPathToTarget()
{
	std::vector<int> path;

	if (!pathFound || target < 0)
		return path;

	int nd = target;

	path.push_back(nd);

	while (nd != source)
	{
		nd = route[nd];

		path.push_back(nd);
	}

	return path;
}

/************************************************************************/
/*              Does a Breadth First search on a graph                  */
/************************************************************************/
template <class graph_type>
class BreadthFirstItemSearch
{
private:
	//Enum that hold data about a node
	//Used by visitedNode
	enum
	{
		unvisited,
		visited,
		noParent
	};

	typedef typename graph_type::EdgeType Edge;
	typedef typename graph_type::NodeType Node;


private:
	graph_type& graph;
	//Holds all the visitedNodes
	std::vector<int> visitedNode;
	//Saves the route the algorithm searched
	std::vector<int> route;

	//Start and goal
	int source, targetFound;

	TreeEntity* pathFound;

	//Starts the search algorithm
	TreeEntity* Search();

public:

	//Constructor
	BreadthFirstItemSearch(graph_type& graph, int source) : graph(graph), source(source), pathFound(false), visitedNode(graph.NumNodes(), unvisited), route(graph.NumNodes(), noParent)
	{
		pathFound = Search();
	}

	//Checks if path is found
	glm::vec2 treeFound()
	{
		if (pathFound != NULL)
		{
			return pathFound->GetPosition();
		}
		else
		{
			return graph.GetNode(source).GetPosition();
		}
	}

	//Returns the path from the start to the goal
	//std::vector<int> GetPathToTarget();

};

/************************************************************************/
/*                  Breadth First search on a graph                     */
/************************************************************************/
template <class graph_type>
TreeEntity* BreadthFirstItemSearch<graph_type>::Search()
{
	std::queue<Edge*> q;

	Edge dummy(source, source, 0);

	q.push(&dummy);

	visitedNode[source] = visited;

	while (!q.empty())
	{
		Edge* next = q.front();

		q.pop();

		route[next->GetTo()] = next->GetFrom();

		Tile* tempTile = TileHandler::Instance()->GetTileAtPos(next->GetTo());
		if (!tempTile->GetTreeList()->empty())
		{
			for (int i = 0; i < tempTile->GetTreeList()->size(); i++)
			{
				if (!tempTile->GetTreeList()->at(i)->isHarvested() && !tempTile->GetTreeList()->at(i)->isReserved())
				{
					tempTile->GetTreeList()->at(i)->SetReserved(true);
					//printf("Position %f ,%f \n", tempTile->GetTreeList()->at(i)->GetPosition().x, tempTile->GetTreeList()->at(i)->GetPosition().y);
					return tempTile->GetTreeList()->at(i);
				}
				
			}
			
		}

		int edgesSize = graph.edges[next->GetTo()].size();
		for (int pe = 0; pe < edgesSize; pe++)
		{

			if (visitedNode[graph.edges[next->GetTo()][pe].GetTo()] == unvisited)
			{
				q.push(&graph.edges[next->GetTo()][pe]);
				visitedNode[graph.edges[next->GetTo()][pe].GetTo()] = visited;
			}
		}
	}

	return nullptr;
}

//template <class graph_type, class item>
//std::vector<int> BreadthFirstItemSearch<graph_type, item>::GetPathToTarget()
//{
//	std::vector<int> path;
//
//	if (!pathFound || targetFound < 0)
//		return path;
//
//	int nd = targetFound;
//
//	path.push_back(nd);
//
//	while (nd != source)
//	{
//		nd = route[nd];
//
//		path.push_back(nd);
//	}
//
//	return path;
//}

/************************************************************************/
/*                    Does a A* search on a graph                       */
/************************************************************************/
template <class graph_type>
class AStarSearch
{
private:
	//Shortens the code
	typedef typename graph_type::EdgeType Edge;

	//Custom struct that holds a index and a cost
	struct IndexCost
	{
		int index;
		float cost;

		IndexCost(int index, float cost)
		{
			this->index = index;
			this->cost = cost;
		}

		friend bool operator<(const IndexCost& i1, const IndexCost& i2)
		{
			return i1.cost > i2.cost;
		}

	};

private:

	graph_type* graph;

	//List of all the G Costs
	std::vector<float> gCosts;
	//List of all F costs
	std::vector<float> fCosts;

	//List of Edges that holds the shortest path to the goal
	std::vector<Edge*> shortestPathTree;
	//List of Edges that holds all the searched nodes
	std::vector<Edge*> searchFrontier;

	glm::vec2 goalPos;
	//Start and goal
	int source;
	int target;

	bool pathFound;

	//Starts the search
	bool Search();

public:
	//Constructor
	AStarSearch(graph_type* graph, int source, int target, glm::vec2 goalPos) : graph(graph), 
															 shortestPathTree(graph->NumNodes()),
															 searchFrontier(graph->NumNodes()),
															 gCosts(graph->NumNodes(), 0.0f),
															 fCosts(graph->NumNodes(), 0.0f),
															 source(source),
															 target(target),
															 goalPos(goalPos)
	{
		pathFound = Search();
	}

	//Checks if path is found
	bool Found()
	{
		return pathFound;
	}
	//Returns the path to the target
	std::vector<glm::vec2> GetPathToTarget();

	//Returns the total cost to target
	float GetCostToTarget() const{ return gCost[target]; }

	//Calculates the manhattan distance between a node and the goal
	float ManhattanDistance(int currentNode, int goalNode);
};

/************************************************************************/
/*                       A* search on a graph                           */
/************************************************************************/
template <class graph_type>
bool AStarSearch<graph_type>::Search()
{
	std::priority_queue<AStarSearch::IndexCost> pq;

	pq.push(AStarSearch::IndexCost(source, fCosts[source]));

	while (!pq.empty())
	{
		int nextClosestNode = pq.top().index;
		pq.pop();

		shortestPathTree[nextClosestNode] = searchFrontier[nextClosestNode];

		if (nextClosestNode == target)
			return true;

		
		for (graph_type::EdgeIterator edgeIt = graph->StartEdge(nextClosestNode); edgeIt != graph->EndEdge(nextClosestNode); edgeIt.next())
		{

			if (graph->GetNode((*edgeIt).GetTo()).GetType() == NavType::cliff)
				continue;

			float hCost = ManhattanDistance((*edgeIt).GetTo(), target);

			float gCost = gCosts[nextClosestNode] + (*edgeIt).GetCost();

			if (searchFrontier[(*edgeIt).GetTo()] == NULL)
			{
				fCosts[(*edgeIt).GetTo()] = gCost + hCost;
				gCosts[(*edgeIt).GetTo()] = gCost;

				pq.push(AStarSearch::IndexCost((*edgeIt).GetTo(), fCosts[(*edgeIt).GetTo()]));

				searchFrontier[(*edgeIt).GetTo()] = &(*edgeIt);
			}
			else if (gCost < gCosts[(*edgeIt).GetTo()] && shortestPathTree[(*edgeIt).GetTo()] == NULL)
			{
				fCosts[(*edgeIt).GetTo()] = gCost + hCost;
				gCosts[(*edgeIt).GetTo()] = gCost;

				pq.push(AStarSearch::IndexCost((*edgeIt).GetTo(), fCosts[(*edgeIt).GetTo()]));

				searchFrontier[(*edgeIt).GetTo()] = &(*edgeIt);
			}
		}

	}

	return false;
}


template <class graph_type>
std::vector<glm::vec2> AStarSearch<graph_type>::GetPathToTarget()
{
	std::vector<glm::vec2> path;

	if ( target < 0)
		return path;

	int nd = target;
	path.push_back(goalPos);
	path.push_back(graph->nodes[nd].GetPosition());

	while (nd != source && shortestPathTree[nd] != 0)
	{
		nd = shortestPathTree[nd]->GetFrom();

		path.push_back(graph->nodes[nd].GetPosition());
	}
	return path;
}

template <class graph_type>
float AStarSearch<graph_type>::ManhattanDistance(int currentNode, int goalNode)
{
	glm::vec2 currentPos = graph->GetNode(currentNode).GetPosition();
	glm::vec2 goalPos = graph->GetNode(goalNode).GetPosition();

	return std::max(abs(goalPos.x - currentPos.x), abs(goalPos.y - currentPos.y));
}

/************************************************************************/
/*         Does a Bidirectional Depth First search on a graph           */
/************************************************************************/
template <class graph_type>
class CustomSearch
{
private:
	//Enum that hold data about a node
	//Used by visitedNode
	enum
	{
		unvisited,
		visitedA,
		visitedB,
		noParent
	};

	//Shortens the code
	typedef typename graph_type::EdgeType Edge;
	typedef typename graph_type::NodeType Node;


private:
	graph_type& graph;

	//Holds all the visited nodes
	std::vector<int> visitedNode;
	//Search ones search route
	std::vector<int> routeA;
	//Search twos search route
	std::vector<int> routeB;

	//Start and goal
	int source, target;

	//The Node where the searches connected
	int nodeFound;

	//If the path was found
	bool pathFound;

	//Starts the search
	bool Search();

public:

	//Constructor
	CustomSearch(graph_type& graph, int source, int target = -1) : graph(graph), 
																   source(source), 
																   target(target), 
																   pathFound(false), 
																   visitedNode(graph.NumNodes(), unvisited), 
																   routeA(graph.NumNodes(), noParent), 
																   routeB(graph.NumNodes(), noParent)
	{
		pathFound = Search();
	}

	//Checks if the path was found
	bool Found()
	{
		return pathFound;
	}

	//Gets the path to the target
	std::vector<int> GetPathToTarget();

};

/************************************************************************/
/*             Bidirectional Depth First search on a graph              */
/************************************************************************/
template <class graph_type>
bool CustomSearch<graph_type>::Search()
{
	std::stack<Edge*> stackA;
	std::stack<Edge*> stackB;

	Edge dummyA(source, source, 0);
	Edge dummyB(target, target, 0);

	stackA.push(&dummyA);
	stackB.push(&dummyB);

	while (!stackA.empty() || !stackB.empty())
	{
		Edge* nextA = stackA.top();
		Edge* nextB = stackB.top();

		stackA.pop();
		stackB.pop();

		routeA[nextA->GetTo()] = nextA->GetFrom();
		routeB[nextB->GetTo()] = nextB->GetFrom();

		if (visitedNode[nextA->GetTo()] == visitedB)
		{
			nodeFound = nextA->GetTo();
			return true;
		}
		if (visitedNode[nextB->GetTo()] == visitedA)
		{
			nodeFound = nextB->GetTo();
			return true;
		}

		visitedNode[nextA->GetTo()] = visitedA;
		visitedNode[nextB->GetTo()] = visitedB;
		

		int edgesSizeA = graph.edges[nextA->GetTo()].size();
		for (int pe = 0; pe < edgesSizeA; pe++)
		{

			if (visitedNode[graph.edges[nextA->GetTo()][pe].GetTo()] == unvisited)
			{
				stackA.push(&graph.edges[nextA->GetTo()][pe]);
			}
		}
		int edgesSizeB = graph.edges[nextB->GetTo()].size();
		for (int pe = 0; pe < edgesSizeB; pe++)
		{

			if (visitedNode[graph.edges[nextB->GetTo()][pe].GetTo()] == unvisited)
			{
				stackB.push(&graph.edges[nextB->GetTo()][pe]);
			}
		}
	}

	return false;
}

template <class graph_type>
std::vector<int> CustomSearch<graph_type>::GetPathToTarget()
{
	std::vector<int> path;

	if (!pathFound || target < 0)
		return path;

	int nd = nodeFound;
	int ndB = nodeFound;

	path.push_back(nd);

	while (nd != source)
	{
		nd = routeA[nd];

		path.insert(path.begin(), nd);
	}

	while (ndB != target)
	{
		ndB = routeB[ndB];

		path.push_back(ndB);
	}

	return path;
}