#include <cstring>
#include <iostream>

#include "Problem.h"

#include "htree/Node.h"
#include "htree/Hyperedge.h"
#include "htree/H_BucketElim.h"

using namespace std;

Problem::Problem(AbstractAlgorithm *algorithm)
{
	this->algorithm = algorithm;
}

Problem::~Problem()
{
}

Solution *Problem::calculateSolution(Instantiator *instantiator)
{
	this->parse();
	this->preprocess();

	Hypergraph *hg = this->buildHypergraphRepresentation();

	H_BucketElim be;
	Hypertree *ht = be.buildHypertree(hg, BE_MIW_ORDER);
	ht->swapChiLambda();
        ht->shrink(true);
        ht->swapChiLambda();

	ht = new ExtendedHypertree(ht);
	((ExtendedHypertree *)ht)->normalize();

#ifdef DEBUG
	((ExtendedHypertree *)ht)->print();
#endif
	this->algorithm->setInstantiator(instantiator);
	return this->algorithm->evaluate((ExtendedHypertree *)ht);
}

Vertex Problem::getVertexId(string vertexName)
{
	ReverseNameMap::iterator it = reverseVertexNames.find(vertexName);
	if(it != reverseVertexNames.end()) return it->second;
	return VERTEXNOTFOUND;
}

string Problem::getVertexName(Vertex vertexId)
{
	return this->vertexNames[vertexId];
}

Hypergraph *Problem::createHypergraphFromSets(VertexSet vertices, EdgeSet edges)
{
	vector<Node *> lookup; lookup.resize(vertices.size(), NULL);
	int edgeId = 0;
	Hypergraph *hg = new Hypergraph();

	for(VertexSet::iterator it = vertices.begin(); it != vertices.end(); ++it)
	{
		if((unsigned int)*it >= lookup.capacity()) lookup.resize(*it + 1);
		Node *n; CNULL(n = new Node(*it, *it));
		lookup[*it] = n;
		hg->MyNodes.push_back(n);
	}

	for(EdgeSet::iterator it = edges.begin(); it != edges.end(); ++it)
	{
		Hyperedge *e; CNULL(e = new Hyperedge(edgeId, edgeId)); ++edgeId;
		hg->MyEdges.push_back(e);

		lookup[it->first]->insEdge(e);
		lookup[it->second]->insEdge(e);
		e->insNode(lookup[it->first]);
		e->insNode(lookup[it->second]);
	}

	for(int i = 0; i < (int)hg->MyNodes.size(); ++i)
		hg->MyNodes[i]->updateNeighbourhood();
	
	for(int i = 0; i < (int)hg->MyEdges.size(); ++i)
		hg->MyEdges[i]->updateNeighbourhood();
	
	hg->iMyMaxNbrOfNodes = hg->MyNodes.size();
	hg->iMyMaxNbrOfEdges = hg->MyEdges.size();

	return hg;
}

Vertex Problem::storeVertexName(string name)
{
	Vertex v;
	if((v = getVertexId(name)) != VERTEXNOTFOUND) return v;
	
	vertexNames.push_back(name);
	reverseVertexNames.insert(ReverseNameMap::value_type(name, vertexNames.size() - 1));

	return vertexNames.size() - 1;
}

Vertex Problem::createAuxiliaryVertex()
{
	vertexNames.push_back(string(""));
	return vertexNames.size() - 1;
}
