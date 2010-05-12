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
		this->reverseNames.clear();

		this->moveToHypergraph();
	
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

void AbstractHypergraph::moveToHypergraph()
{
	int nodeID = 0, edgeID = 0;
	map<Variable, Node *> lookup;

	for(int rule = 0; rule < (int)heads.size(); ++rule)
	{
		if((int)signs[rule].size() == 0) continue;

		Node *r, *v;
		Hyperedge *e;
		char str[256];

		CNEG(snprintf(str, 256, "r%d", rule));
		CNULL(r = new Node(nodeID++, strdup(str)));
		this->MyNodes.push_back(r);

		for(map<Variable, bool>::iterator it = signs[rule].begin(); it != signs[rule].end(); ++it)
		{
			if(lookup.find(it->first) == lookup.end())
			{
				CNEG(snprintf(str, 256, "v%d", it->first));
				CNULL(v = new Node(nodeID++, strdup(str)));
				this->MyNodes.push_back(v);
				lookup.insert(make_pair(it->first, v));
			}
			else v = lookup[it->first];

			CNEG(snprintf(str, 256, "E%d", edgeID));
			CNULL(e = new Hyperedge(edgeID++, strdup(str)));
			this->MyEdges.push_back(e);

			e->insNode(r);
			e->insNode(v);
			r->insEdge(e);
			v->insEdge(e);
		}	
	}

	if(nodeID == 0)
	{
		int var = createTemporaryVariable();
		heads[0].insert(var);
		signs[0].insert(make_pair(var, true));
		var = createTemporaryVariable();
		signs[0].insert(make_pair(var, true));

		moveToHypergraph();
	}
}

void AbstractHypergraph::addVariable(int rule, const char *name, bool positive, bool head)
{
	static int ignore = -1;
	if(ignore == rule) return;

	int variable = storeVariable(name);

	if(rule == (int)heads.size()) heads.push_back(set<Variable>());
	if(head) heads[rule].insert(variable);

	// check if variable appears in rule twice
	if(signs[rule].find(variable) != signs[rule].end() && !head)
	{
		// case: variable appears in head and negated in body - remove from head and body
		if(heads[rule].find(variable) != heads[rule].end() && !positive)
		{ heads[rule].erase(variable); signs[rule].erase(variable); signs[rule].insert(make_pair(createTemporaryVariable(), false)); }
		// case: variable appears in body twice (negated or unnegated) - do nothing
		else if(signs[rule][variable] == positive) { }
		// case: any other - ignore the rule completely
		else { heads[rule].clear(); signs[rule].clear(); ignore = rule; }
	}
	else
	{
		signs[rule].insert(map<Variable, bool>::value_type(variable, positive));
	}
}

int AbstractHypergraph::storeVariable(const char *name)
{
	map<string, int>::iterator it = reverseNames.find(string(name));

	if(it != reverseNames.end()) return it->second;
	
	names.push_back(string(name));
	reverseNames.insert(map<string, int>::value_type(string(name), names.size() - 1));

	return names.size() - 1;
}

int AbstractHypergraph::createTemporaryVariable()
{
	names.push_back(string(""));
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
