#include <cstdio>
#include <cstring>

#include "../support/support.h"

#include "DatalogHypergraph.h"
#include "../htree/Hypergraph.h"
#include "../htree/Node.h"
#include "../htree/Hyperedge.h"

#undef yyFlexLexer
#define yyFlexLexer DatalogFlexLexer
#include <FlexLexer.h>

DatalogHypergraph::DatalogHypergraph() 
	: AbstractHypergraph(), DatalogParser(new DatalogFlexLexer(), NULL)
{
}

DatalogHypergraph::DatalogHypergraph(istream *in) 
	: AbstractHypergraph(), DatalogParser(new DatalogFlexLexer(), in)
{
}

DatalogHypergraph::~DatalogHypergraph()
{
}

void DatalogHypergraph::addVariable(int rule, const char *name, bool positive, bool head)
{
	AbstractHypergraph::addVariable(rule, name, positive, head);
}

int DatalogHypergraph::parseInput()
{
	return this->yyparse();
}

