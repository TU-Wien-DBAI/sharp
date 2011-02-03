#include <cstring>
#include <iostream>

#include "Problem.h"

#include "htree/Node.h"
#include "htree/Hyperedge.h"
#include "htree/H_BucketElim.h"

using namespace std;

static Timer t;

Problem::Problem(AbstractAlgorithm *algorithm, bool benchmark)
{
	this->algorithm = algorithm;
	this->parsed = false;
	this->benchmark = benchmark;

	this->setDecompositionOptions();
	
	// start numbering of vertices at 1 instead of 0
	this->createAuxiliaryVertex();
}

Problem::~Problem()
{
}

void Problem::setDecompositionOptions(DecompositionOptions options, void *parameter)
{
	this->decompositionOptions = options;
	this->decompositionParameter = parameter;
}

Solution *Problem::calculateSolution(Instantiator *instantiator)
{
	Timer t1, t2;

	if(benchmark) { cout << "Parsing... " << flush; t1.start(); }

	if(!this->parsed)
	{
		this->parse();
		this->preprocess();
		this->parsed = true;
	}

	if(benchmark) { cout << "done. (took "; t1.printStop(); cout << " seconds)" << endl; }
	if(benchmark) { cout << "Starting decomposition..." << endl; t1.start(); }

	Hypergraph *hg = this->buildHypergraphRepresentation();

	H_BucketElim be;
	Hypertree *ht = NULL;

	if(benchmark) { cout << "\tbuilding hypergraph took "; t1.printStop(); t2.start(); cout << " seconds)" << endl; }

	// when decomposing a bipartite graph
	if(this->decompositionOptions & BipartiteGraph)
		ht = be.buildHypertree(hg, BE_MIW_ORDER, (int)this->decompositionParameter);
	// when decomposing a default graph
	else 
		ht = be.buildHypertree(hg, BE_MIW_ORDER);

	if(benchmark) { cout << "\tbuilding hypertree decomposition took "; t2.printStop(); t2.start(); cout << " seconds)" << endl; }

	ht = new ExtendedHypertree(ht);
	((ExtendedHypertree *)ht)->normalize();

#ifdef DEBUG
	((ExtendedHypertree *)ht)->print();
#endif
	
	if(benchmark) { cout << "\tnormalization took "; t2.printStop(); cout << " seconds)" << endl; }
	if(benchmark) { cout << "done. (took "; t1.printStop(); cout << " seconds)" << endl; }
	if(benchmark) { cout << "Calculating solution... " << flush; t1.start(); }
	this->algorithm->setInstantiator(instantiator);
	Solution *s = this->algorithm->evaluate((ExtendedHypertree *)ht);
	if(benchmark) { cout << "done. (took "; t1.printStop(); cout << " seconds)" << endl; }

	return s;
}

int Problem::calculateTreeWidth()
{
        if(!this->parsed)
        {
                this->parse();
                this->preprocess();
                this->parsed = true;
        }

        Hypergraph *hg = this->buildHypergraphRepresentation();

        H_BucketElim be;
        Hypertree *ht = NULL;

        // when decomposing a bipartite graph
        if(this->decompositionOptions & BipartiteGraph)
                ht = be.buildHypertree(hg, BE_MIW_ORDER, (int)this->decompositionParameter);
        // when decomposing a default graph
        else
                ht = be.buildHypertree(hg, BE_MIW_ORDER);

	return ht->getTreeWidth();
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

Hypergraph *Problem::createGraphFromSets(VertexSet vertices, EdgeSet edges)
{
	return createGraphFromDisjointSets(vertices, VertexSet(), edges);
}

Hypergraph *Problem::createGraphFromDisjointSets(VertexSet v1, VertexSet v2, EdgeSet edges)
{
	vector<Node *> lookup; lookup.resize(v1.size() + v2.size(), NULL);
	int edgeId = 0;
	Hypergraph *hg = new Hypergraph();

	for(VertexSet::iterator it = v1.begin(); it != v1.end(); ++it)
	{
		if((unsigned int)*it >= lookup.capacity()) lookup.resize(*it + 1);
		Node *n; CNULL(n = new Node(*it, *it));
		lookup[*it] = n;
		hg->MyNodes.push_back(n);
	}

	for(VertexSet::iterator it = v2.begin(); it != v2.end(); ++it)
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

unsigned int Problem::getVertexCount()
{
	return vertexNames.size() - 1;
}

Vertex Problem::createAuxiliaryVertex()
{
	vertexNames.push_back(string("__aux"));
	return vertexNames.size() - 1;
}

void Problem::printVertexNames(ostream &out)
{
	for(unsigned int i = 0; i < this->vertexNames.size(); ++i)
		out << "(" << i << "=" << this->vertexNames[i] << ")";
	out << endl;
}
