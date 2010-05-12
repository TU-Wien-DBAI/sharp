#include <cstdio>
#include <cstring>
#include <iostream>

#include "../support/support.h"

#include "DIMACSHypergraph.h"
#include "../htree/Hypergraph.h"
#include "../htree/Node.h"
#include "../htree/Hyperedge.h"

#undef yyFlexLexer
#define yyFlexLexer DIMACSFlexLexer
#include <FlexLexer.h>

using namespace std;

DIMACSHypergraph::DIMACSHypergraph() 
	: AbstractHypergraph(), DIMACSParser(new DIMACSFlexLexer(), NULL)
{
}

DIMACSHypergraph::DIMACSHypergraph(istream *in) 
	: AbstractHypergraph(), DIMACSParser(new DIMACSFlexLexer(), in)
{
}

DIMACSHypergraph::~DIMACSHypergraph()
{
}

void DIMACSHypergraph::addVariable(int clause, int variable, bool positive)
{
	char n[256];
	snprintf(n, 256, "%d", variable);

	AbstractHypergraph::addVariable(clause, n, positive);
}

int DIMACSHypergraph::parseInput()
{
	return this->yyparse();
}

