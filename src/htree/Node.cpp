// Node.cpp: implementation of the Node class.
//
//////////////////////////////////////////////////////////////////////

#include "Node.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


Node::Node(int ID, char *cName, int iSize, int iNbrOfNeighbours) : Component(ID, cName, iSize, iNbrOfNeighbours)
{
}

Node::Node(int ID, char *cName) : Component(ID, cName)
{
}

Node::~Node()
{
}



//////////////////////////////////////////////////////////////////////
// Class methods
//////////////////////////////////////////////////////////////////////


// Returns the number of hyperedges containing the node
int Node::getNbrOfEdges()
{
	return Component::size();
}


// Inserts a hyperedge at position iPos
void Node::insEdge(Hyperedge *Comp, int iPos)
{
	Component::ins((Component *)Comp, iPos);
}

void Node::insEdge(Hyperedge *Comp)
{
	Component::ins((Component *)Comp);
}

// Removes a hyperedge
bool Node::remEdge(Hyperedge *Comp)
{
	return Component::rem((Component *)Comp);
}


// Returns the hyperedge contained at position iPos
Hyperedge *Node::getEdge(int iPos)
{
	return (Hyperedge *)Component::get(iPos);
}


// Inserts a neighbour node at position iPos
void Node::insNeighbour(Node *Neighbour, int iPos)
{
	Component::insNeighbour((Component *)Neighbour, iPos);
}


// Removes a neighbour node
bool Node::remNeighbour(Node *Neighbour)
{
	return Component::remNeighbour((Component *)Neighbour);
}


// Returns the neighbour node at position iPos
Node *Node::getNeighbour(int iPos)
{
	return (Node *)Component::getNeighbour(iPos);
}


// Creates a clone of the node
Node *Node::clone()
{
	return (Node *)Component::clone();
}

