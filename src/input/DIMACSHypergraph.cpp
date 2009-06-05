#include <cstdio>
#include <cstring>

#include "../support/support.h"

#include "DIMACSHypergraph.h"
#include "../htree/Hypergraph.h"
#include "../htree/Node.h"
#include "../htree/Hyperedge.h"

static Node **aClauses = NULL;
static Node **aVariables = NULL;

DIMACSHypergraph::DIMACSHypergraph() : Hypergraph(), DIMACSParser(NULL)
{
}

DIMACSHypergraph::DIMACSHypergraph(istream *in) : Hypergraph(), DIMACSParser(in)
{
}

DIMACSHypergraph::~DIMACSHypergraph()
{
}

void DIMACSHypergraph::buildHypergraph(Parser *p)
{
	if(p != NULL)
	{
		Hypergraph::buildHypergraph(p);
		return;
	}

	this->yyparse();

	delete [] aClauses;
	delete [] aVariables;

	for(int i = 0; i < (int)this->MyNodes.size(); ++i)
		this->MyNodes[i]->updateNeighbourhood();

	for(int i = 0; i < (int)this->MyEdges.size(); ++i)
		this->MyEdges[i]->updateNeighbourhood();

	this->iMyMaxNbrOfNodes = this->MyNodes.size();
	this->iMyMaxNbrOfEdges = this->MyEdges.size();
}

void DIMACSHypergraph::addVariable(int iClause, int iVariable, bool bNegative)
{
	static int iEdgeID = 0, iNodeID = 0;

	if(signs.find(iClause) == signs.end()) signs.insert(signmap::value_type(iClause, map<int, bool>()));
	signs[iClause].insert(map<int, bool>::value_type(iVariable, bNegative));

	Node *c, *v;
	Hyperedge *e;
	char str[256];

	if(aClauses == NULL)
	{
		CNULL(aClauses = new Node *[this->getClauseCount()+1]);
		for(int i = 0; i < this->getClauseCount(); ++i) aClauses[i] = NULL;
		CNULL(aVariables = new Node *[this->getVariableCount()+1]);
		for(int i = 0; i < this->getVariableCount()+1; ++i) aVariables[i] = NULL;
	}

	CNEG(snprintf(str, 256, "E%d", iEdgeID));
	CNULL(e = new Hyperedge(iEdgeID++, strdup(str)));
	this->MyEdges.push_back(e);

	if((c = aClauses[iClause]) == NULL)
	{
		CNEG(snprintf(str, 256, "c%d", iClause));
		CNULL(c = new Node(iNodeID++, strdup(str)));
		this->MyNodes.push_back(c);
		aClauses[iClause] = c;
	}

	if((v = aVariables[iVariable]) == NULL)
	{
		CNEG(snprintf(str, 256, "v%d", iVariable));
		CNULL(v = new Node(iNodeID++, strdup(str)));
		this->MyNodes.push_back(v);
		aVariables[iVariable] = v;
	}

	e->insNode(c);
	e->insNode(v);
	c->insEdge(e);
	v->insEdge(e);
}

signmap &DIMACSHypergraph::getSignMap()
{
	return this->signs;
}
