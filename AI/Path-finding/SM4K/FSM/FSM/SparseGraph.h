#pragma once

#include <vector>
#include "NodeTypeEnum.h"

template <typename node_type, typename edge_type>
class SparseGraph
{
public:

	//Easier access to the edge and node types in the graph
	typedef node_type NodeType;
	typedef edge_type EdgeType;

	typedef std::vector<node_type> NodeVector;
	typedef std::vector<EdgeType> EdgeVector;
	typedef std::vector<EdgeVector> EdgeListVector;
	
	NodeVector nodes;
	EdgeListVector edges;

	
private:

	//Start position and goal position
	int startIndex;
	int goalIndex;

	//If the graph is digraph or not
	bool digraph;
	//Next valid node index
	int nextNodeIndex;

	//Checks if the edge is unique
	bool UniqueEdge(int from, int to) const;
	
	void RemoveInvalidEdges();

public:
	SparseGraph(bool digraph) : nextNodeIndex(0), digraph(digraph){}

	//Returns the node at a given index
	NodeType& GetNode(int indx);
	//Returns a reference to an edge
	EdgeType& GetEdge(int from, int to);

	//Gets next free node index
	int GetNextFreeNodeIndex()const{ return nextNodeIndex; }

	//Adds node to graph and returns index
	int AddNode(NodeType node);
	//Removes node by setting index to invalid_node_index (-1)
	void RemoveNode(int node);

	//Adds an edge to the graph. If graph is digraph then connect a similar edge in the opposite direction
	void AddEdge(EdgeType edge);
	//Removes an edge from the graph. If digraph then remove opposite edge too
	void RemoveEdge(int from, int to);

	//Sets the cost of an edge
	void SetEdgeCost(int from, int to, float cost);

	//Returns total number of nodes
	int NumNodes() const { return nodes.size(); }

	//Returns number of active nodes
	int NumActiveNodes()const
	{
		int count = 0;
		for (int n = 0; n < nodes.size(); ++n)
		{
			if (nodes[n].Index() != invalid_node_index)
				count++
		}
		return count;
	}
	
	//Return total number of edges in the graph
	int NumEdges() const
	{
		int tot = 0;

		for (EdgeListVector::const_iterator curEdge = edges.begin(); curEdge != edges.end(); ++curEdge)
		{
			tot += curEdge->size();
		}
		return tot;
	}

	int GetStartIndex() const { return startIndex; }
	void SetStartIndex(int val) { startIndex = val; }
	int GetGoalIndex() const { return goalIndex; }
	void SetGoalIndex(int val) { goalIndex = val; }

	bool isDigraph() const { return digraph; }
	bool isEmpty() const{ return nodes.empty(); }

	bool isNodePresent(int indx) const;

	bool isEdgePresent(int from, int to) const;

	void Clear()
	{
		nextNodeIndex = 0;
		nodes.clear();
		edges.clear();
	}

	void RemoveEdges()
	{
		for (EdgeListVector::iterator it = edges.begin(); it != edges.end(); ++it)
		{
			it->clear();
		}
	}
	

	class EdgeIterator
	{
	protected:
		std::vector<EdgeType>* edgeList;

		int currentIndex;

		//Constructor. Only usable by the "Graph" class
		EdgeIterator(std::vector<EdgeType>* list, int startIndex = 0)
		{
			this->edgeList = list;
			this->currentIndex = startIndex;
		}
	public:
		///Overloading the * operator. Returns a reference to the edge that the iterator is currently pointing to
		/**
		@return A reference to the current edge

		*Note that the iterator stores an index in a vector, and as such will be offset if the edge that the iterator is pointing to,
		or any previous node, are removed.
		*/
		EdgeType& operator*()
		{
			return this->edgeList->at(this->currentIndex);
		}

		///Overloading the != operator. Returns true if the two EdgeIterators are not the same 
		/**
		@param arg The EdgeIterator to compare inequality with
		@return true if the objects are not the same (originating from the same graph, and pointing to the same edge), otherwise false
		*/
		bool operator!=(const EdgeIterator& arg)
		{
			return !((this->currentIndex == arg.currentIndex || (this->currentIndex >= this->edgeList->size() && arg.currentIndex == invalid_node_index)) && (this->edgeList = arg.edgeList));
		}

		///Returns true if the iterator is currently pointing to a edge, and returns false if it has reached the end of the edge list
		/**
		@return A boolean specifying whether the iterator is still valid or not
		*/
		bool isValid()
		{
			return (this->currentIndex >= 0);
		}

		///Moves the iterator to the next edge belonging to the originating node
		void next()
		{
			++this->currentIndex;

			if (this->currentIndex >= this->edgeList->size())
				this->currentIndex = invalid_node_index;
		}

		template<class node_type, class edge_type>
		friend class SparseGraph;
	};

	class NodeIterator
	{
	private:
		typename NodeVector::iterator currNode;

		SparseGraph<node_type, edge_type>& sGraph;

		void GetNextValidNode(typename NodeVector::iterator& it)
		{
			if (currNode == sGraph.nodes.end() || it->Index() == invalid_node_index)
				return;

			while (it->Index() == invalid_node_index)
			{
				it++;

				if (currNode == sGraph.nodes.end())
					break;
			}
		}

	public:
		NodeIterator(SparseGraph<node_type, edge_type>& graph) : sGraph(graph)
		{
			currNode = sGraph.nodes.begin();
		}

		NodeType* begin()
		{
			currNode = sGraph.nodes.begin();

			GetNextValidNode(currNode);

			return &(*currNode);
		}

		NodeType* next()
		{
			currNode++;

			GetNextValidNode(currNode);

			return &(*currNode);
		}

		bool end()
		{
			return(currNode == sGraph.nodes.end());
		}

	};

	friend class NodeIterator;

	EdgeIterator StartEdge(int nodeIndex);

	EdgeIterator EndEdge(int nodeIndex);

};

//--------------------------- isNodePresent --------------------------------
//
//  returns true if a node with the given index is present in the graph
//--------------------------------------------------------------------------

template <typename node_type, typename edge_type>
bool SparseGraph<node_type, edge_type>::isNodePresent(int indx) const
{
	if (nodes[indx].Index() == invalid_node_index || indx >= nodes.size())
	{
		return false;
	}
	else
		return true;
}

//--------------------------- isEdgePresent --------------------------------
//
//  returns true if an edge with the given from/to is present in the graph
//--------------------------------------------------------------------------
template <class node_type, class edge_type>
bool SparseGraph<node_type, edge_type>::isEdgePresent(int from, int to) const
{
	if (isNodePresent(from) && isNodePresent(to))
	{
		for (EdgeIterator::const_iterator currEdge = edges[from].begin(); currEdge != edges[from].end(); currEdge++)
		{
			if (currEdge->GetTo() == to)
				return true;
		}

		return false;
	}
	else
		return false;
}

//------------------------------ GetNode -------------------------------------
//
//			Method for obtaining a reference to a specific node
//----------------------------------------------------------------------------
template <class node_type, class edge_type>
node_type& SparseGraph<node_type, edge_type>::GetNode(int indx)
{
	assert(indx < (int)nodes.size() && indx >= 0 && "<SparseGraph::GetNode> Invalid index!");

	return nodes[indx];
}

//------------------------------ GetEdge -------------------------------------
//
//			Method for obtaining a reference to a specific edge
//----------------------------------------------------------------------------

template <typename node_type, typename edge_type>
edge_type& SparseGraph<node_type, edge_type>::GetEdge(int from, int to)
{
	assert(from < (int)nodes.size() && from >= 0 && nodes[from].Index() != invalid_node_index && "<SparseGraph::GetNode> Invalid 'from' index!");

	assert(to < (int)nodes.size() && to >= 0 && nodes[to].Index() != invalid_node_index && "<SparseGraph::GetNode> Invalid 'from' index!");

	for (EdgeIterator::const_iterator currEdge = edges[from].begin(); currEdge != edges[from].end(); currEdge++)
	{
		if (currEdge->GetTo() == to)
			return *currEdge;
	}

	assert(0 && "<SparseGraph::GetEdge>: edge does not exist");
}

//-------------------------- AddEdge ------------------------------------------
//
//  Use this to add an edge to the graph. The method will ensure that the
//  edge passed as a parameter is valid before adding it to the graph. If the
//  graph is a digraph then a similar edge connecting the nodes in the opposite
//  direction will be automatically added.
//-----------------------------------------------------------------------------
template <typename node_type, typename edge_type>
void SparseGraph<node_type, edge_type>::AddEdge(EdgeType edge)
{
	assert(edge.GetFrom() < nextNodeIndex && edge.GetTo() < nextNodeIndex && "<SparseGraph::AddEdge>: invalid node index>");

	if (nodes[edge.GetTo()].Index() != invalid_node_index && nodes[edge.GetFrom()].Index() != invalid_node_index)
	{
		if (UniqueEdge(edge.GetFrom(), edge.GetTo()))
		{
			edges[edge.GetFrom()].push_back(edge);
		}

		if (!digraph)
		{
			if (UniqueEdge(edge.GetTo(), edge.GetFrom()))
			{
				EdgeType newEdge = edge;

				newEdge.SetTo(edge.GetFrom());
				newEdge.SetFrom(edge.GetTo());

				edges[edge.GetTo()].push_back(newEdge);
			}
		}
	}
}


//----------------------------- RemoveEdge ---------------------------------
//
//		Removes an edge. Makes sure the edge is valid
//
//--------------------------------------------------------------------------
template <typename node_type, typename edge_type>
void SparseGraph<node_type, edge_type>::RemoveEdge(int from, int to)
{
	assert(from < (int)nodes.size() && to < (int)nodes.size() && "<SparseGraph::RemoveEdge>: Invalid node index");

	EdgeVector::iterator currEdge;

	if (!digraph)
	{
		for (currEdge = edges[to].begin(); currEdge != edges[to].end(); currEdge++)
		{
			if (currEdge->GetTo() == from)
			{
				currEdge = edges[to].erase(currEdge);
				break;
			}
		}

		for (currEdge = edges[from].begin(); currEdge != edges[from].end(); currEdge++)
		{
			if (currEdge->GetTo() == to)
			{
				currEdge = edges[from].erase(currEdge);
				break;
			}
		}
	}
}

//-------------------------- AddNode -------------------------------------
//
//  Given a node this method first checks to see if the node has been added
//  previously but is now inactive. If it is, it is reactivated.
//
//  If the node has not been added previously, it is checked to make sure its
//  index matches the next node index before being added to the graph
//------------------------------------------------------------------------
template <typename node_type, typename edge_type>
int SparseGraph<node_type, edge_type>::AddNode(NodeType node)
{
	if (node.Index() < (int)nodes.size())
	{
		assert(nodes[node.Index()].Index() == invalid_node_index && "<SparseGraph::AddNode>: Attempting to add a node with a duplicate ID");

		nodes[node.Index()] = node;
		return nextNodeIndex;
	}
	else
	{
		nodes.push_back(node);
		edges.push_back(EdgeVector());

		return nextNodeIndex++;
	}
}

//----------------------- RemoveInvalidEdges ------------------------------------
//
//  iterates through all the edges in the graph and removes any that point
//  to an invalidated node
//-----------------------------------------------------------------------------
template <typename node_type, typename edge_type>
void SparseGraph<node_type, edge_type>::RemoveInvalidEdges()
{
	for (EdgeListVector::iterator currEdgeList = edges.begin(); currEdgeList != edges.end(); currEdgeList++)
	{
		for (EdgeVector::iterator currEdge = (*currEdgeList).begin(); currEdge != *currEdgeList.end(); currEdgeList++)
		{
			if (nodes[currEdge->GetTo()].Index() == invalid_node_index || nodes[currEdge->GetFrom()].Index() == invalid_node_index)
			{
				currEdge = *currEdgeList.erase(currEdge);
			}
		}
	}
}

//------------------------------- RemoveNode -----------------------------
//
//  Removes a node from the graph and removes any links to neighboring
//  nodes
//------------------------------------------------------------------------
template <typename node_type, typename edge_type>
void SparseGraph<node_type, edge_type>::RemoveNode(int node)
{
	assert(node < (int)nodes.size() && "<SparseGraph::RemoveNode>: invalid node index");

	nodes[node].SetIndex(invalid_node_index);

	if (!digraph)
	{
		for (EdgeVector::iterator currEdge = edges[node].begin(); currEdge != edges[node].end(); currEdge++)
		{
			for (EdgeVector::iterator currE = edges[currEdge->GetTo()].begin(); currEdge != edges[currEdge->GetTo()].end(); currE++)
			{
				if (currE->GetTo() == node)
				{
					currE = edges[currEdge->GetTo()].erase(currE);
					break;
				}
			}

			edges[node].clear();
		}
	}
	else
	{
		RemoveInvalidEdges();
	}
}


//-------------------------- SetEdgeCost ---------------------------------
//
//				Sets the cost of a specific edge
//------------------------------------------------------------------------
template <typename node_type, typename edge_type>
void SparseGraph<node_type, edge_type>::SetEdgeCost(int from, int to, float newCost)
{
	assert(from < nodes.size() && to < nodes.size() && "<SparseGraph::SetEdgeCost>: invalid index");

	for (EdgeVector::iterator currEdge = edges[from].begin(); currEdge != edges[from].end(); currEdge++)
	{
		if (currEdge->GetTo() == to)
		{
			currEdge->SetCost(newCost);
			break;
		}
	}
}


//-------------------------------- UniqueEdge ----------------------------
//
//  returns true if the edge is unique. Used when adding
//  edges to prevent duplication
//------------------------------------------------------------------------
template <typename node_type, typename edge_type>
bool SparseGraph<node_type, edge_type>::UniqueEdge(int from, int to) const
{
	for (EdgeVector::const_iterator currEdge = edges[from].begin(); currEdge != edges[from].end(); currEdge++)
	{
		if (currEdge->GetTo() == to)
		{
			return false;
		}
	}
	return true;
}

template<class node_type, class edge_type>
typename SparseGraph<node_type, edge_type>::EdgeIterator SparseGraph<node_type, edge_type>::StartEdge(int nodeIndex)
{
	return SparseGraph<node_type, edge_type>::EdgeIterator(&(this->edges[nodeIndex]));
}

template<class node_type, class edge_type>
typename SparseGraph<node_type, edge_type>::EdgeIterator SparseGraph<node_type, edge_type>::EndEdge(int nodeIndex)
{
	return SparseGraph<node_type, edge_type>::EdgeIterator(&(this->edges[nodeIndex]), invalid_node_index);
}