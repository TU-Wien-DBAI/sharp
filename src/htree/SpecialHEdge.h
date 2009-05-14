/*
***Description***
Class SpecialHEdge models a special hyperedge that can be both a hyperedge and a set of hyperedges.
*/
//////////////////////////////////////////////////////////////////////


#if !defined(CLS_SPECIALHEDGE)
#define CLS_SPECIALHEDGE


#include "Hyperedge.h"
#include "CompSet.h"


class SpecialHEdge : public Hyperedge, public CompSet  
{
public:
	// Constructor
	SpecialHEdge(int iNbrOfEdges);

	// Destructor
	virtual ~SpecialHEdge();
};


#endif // !defined(CLS_SPECIALHEDGE)
