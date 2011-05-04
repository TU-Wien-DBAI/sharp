#include <MaximumCardinalitySearchOrdering.hpp>

#include <probSol.hpp>
#include <Hypergraph.hpp>
#include <Node.hpp>
using namespace sharp;

MaximumCardinalitySearchOrdering::MaximumCardinalitySearchOrdering()
{
}

MaximumCardinalitySearchOrdering::~MaximumCardinalitySearchOrdering()
{
}

Ordering MaximumCardinalitySearchOrdering::getOrdering(Hypergraph *g)
{
	if(g->getNbrOfNodes() > 20000) return g->getMCSOrder();
	
	probSol ps;
	ps.createGraphFromHyp(g);
	ps.MCSOrdering();

	int size = g->getNbrOfNodes();
	Ordering order = new Node*[size+1];

	// Initialize variable order
	for(int i=0; i < size; i++) {
		
		int k = ps.iNodePosInOrdering[i];
			
		order[size-k-1] = g->getNode(i);
		order[size-k-1]->setLabel(0);
	}
	order[size] = NULL;

	return order;
}
