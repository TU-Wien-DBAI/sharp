#include <cstdio>
#include <cstring>

#include <iostream>

#include "../support/support.h"

#include "AbstractHypergraph.h"
#include "../htree/Hypergraph.h"
#include "../htree/Node.h"
#include "../htree/Hyperedge.h"

using namespace std;

AbstractHypergraph::AbstractHypergraph() : Hypergraph()
{
}

AbstractHypergraph::AbstractHypergraph(istream *in) : Hypergraph()
{
}

AbstractHypergraph::~AbstractHypergraph()
{
}

int AbstractHypergraph::buildHypergraph(Parser *p)
{
	if(p != NULL)
		return Hypergraph::buildHypergraph(p);

	if(!this->parseInput())
	{
		this->rules.clear();
		this->variables.clear();
		this->reverseNames.clear();
	
		for(int i = 0; i < (int)this->MyNodes.size(); ++i)
			this->MyNodes[i]->updateNeighbourhood();
	
		for(int i = 0; i < (int)this->MyEdges.size(); ++i)
			this->MyEdges[i]->updateNeighbourhood();
		
		this->iMyMaxNbrOfNodes = this->MyNodes.size();
		this->iMyMaxNbrOfEdges = this->MyEdges.size();
	
		return 0;
	}
	return -1;
}

void AbstractHypergraph::addVariable(int rule, const char *name, bool negative, bool head)
{
	static int edgeID = 0, nodeID = 0;

	bool seenVariable, seenRule;
	int variable = storeVariable(name, seenVariable);


	seenRule = (signs.find(rule) != signs.end());
	signs[rule].insert(map<Variable, bool>::value_type(variable, negative));

	if(rule == (int)heads.size()) heads.push_back(set<Variable>());
	if(head) heads[rule].insert(variable);

	Node *r, *v;
	Hyperedge *e;
	char str[256];

	CNEG(snprintf(str, 256, "E%d", edgeID));
	CNULL(e = new Hyperedge(edgeID++, strdup(str)));
	this->MyEdges.push_back(e);

	if(!seenRule)
	{
		CNEG(snprintf(str, 256, "r%d", rule));
		CNULL(r = new Node(nodeID++, strdup(str)));
		this->MyNodes.push_back(r);
		this->rules.push_back(r);
	}
	else r = this->rules[rule];

	if(!seenVariable)
	{
		CNEG(snprintf(str, 256, "v%d", variable));
		CNULL(v = new Node(nodeID++, strdup(str)));
		this->MyNodes.push_back(v);
		this->variables.push_back(v);
	}
	else v = this->variables[variable];

	e->insNode(r);
	e->insNode(v);
	r->insEdge(e);
	v->insEdge(e);
}

int AbstractHypergraph::storeVariable(const char *name, bool &alreadyPresent)
{
	map<string, int>::iterator it = reverseNames.find(string(name));

	if((alreadyPresent = it != reverseNames.end())) return it->second;
	
	names.push_back(string(name));
	reverseNames.insert(map<string, int>::value_type(string(name), names.size() - 1));

	return names.size() - 1;
}

SignMap &AbstractHypergraph::getSignMap()
{
	return this->signs;
}

NameMap &AbstractHypergraph::getNameMap()
{
	return this->names;
}

HeadMap &AbstractHypergraph::getHeadMap()
{
	return this->heads;
}
