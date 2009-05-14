// Hyperedge.cpp: implementation of the Hyperedge class.
//
//////////////////////////////////////////////////////////////////////

#include "Hyperedge.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


Hyperedge::Hyperedge(int ID, char *cName, int iSize, int iNbrOfNeighbours) : Component(ID, cName, iSize, iNbrOfNeighbours)
{
}


Hyperedge::~Hyperedge()
{
}



//////////////////////////////////////////////////////////////////////
// Class methods
//////////////////////////////////////////////////////////////////////


// Returns the number of nodes contained in the hyperedge
int Hyperedge::getNbrOfNodes()
{
	return Component::size();
}


// Inserts a node at position iPos
void Hyperedge::insNode(Node *Comp, int iPos)
{
	Component::ins((Component *)Comp, iPos);
}


// Removes a node
bool Hyperedge::remNode(Node *Comp)
{
	return Component::rem((Component *)Comp);
}


// Returns the node contained at position iPos
Node *Hyperedge::getNode(int iPos)
{
	return (Node *)Component::get(iPos);
}


// Inserts a neighbour hyperedge at position iPos
void Hyperedge::insNeighbour(Hyperedge *Neighbour, int iPos)
{
	Component::insNeighbour((Component *)Neighbour, iPos);
}


// Removes a neighbour hyperedge
bool Hyperedge::remNeighbour(Hyperedge *Neighbour)
{
	return Component::remNeighbour((Component *)Neighbour);
}


// Returns the neighbour hyperedge at position iPos
Hyperedge *Hyperedge::getNeighbour(int iPos)
{
	return (Hyperedge *)Component::getNeighbour(iPos);
}


// Creates a clone of the hyperedge
Hyperedge *Hyperedge::clone()
{
	return (Hyperedge *)Component::clone();
}


