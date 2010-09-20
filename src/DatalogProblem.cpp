#include <fstream>
#include <iostream>

#include "DatalogProblem.h"

#undef yyFlexLexer
#define yyFlexLexer DatalogFlexLexer
#include <FlexLexer.h>

#include "input/DatalogParser.h"

#include "sharp/AnswerSet.h"
#include "sharp/HeadCycleFreeAnswerSet.h"

using namespace std;

DatalogProblem::DatalogProblem(istream *stream, bool enableHCFExtension)
	: Problem(!enableHCFExtension ? 
			new AnswerSetAlgorithm(this) : 
			new HeadCycleFreeAnswerSetAlgorithm(this))
{
	this->parser = new DatalogParser(new DatalogFlexLexer(), stream, this);
}

DatalogProblem::~DatalogProblem()
{
}

SignMap &DatalogProblem::getSignMap()
{
	return this->signs;
}

HeadMap &DatalogProblem::getHeadMap()
{
	return this->heads;
}

TypeMap &DatalogProblem::getTypeMap()
{
	return this->types;
}

bool DatalogProblem::isRule(Vertex v)
{
	return this->types[v] == RuleVertex;
}

bool DatalogProblem::isVariable(Vertex v)
{
	return this->types[v] == VariableVertex;
}

Rule DatalogProblem::addNewRule()
{
	Vertex r = createAuxiliaryVertex();

	if((unsigned int)r >= this->heads.capacity()) this->heads.resize(r + 100);
	this->heads[r] = VariableSet();

	this->types.push_back(RuleVertex);

	return r;
}

Variable DatalogProblem::addVariable(string name)
{
	Vertex v = storeVertexName(name);

	if(this->types.size() == (unsigned int)v) this->types.push_back(VariableVertex);

	return v;
}

void DatalogProblem::addEdge(Rule rule, Variable variable, bool positive, bool head)
{
	static int ignore = -1;
	if(ignore == rule) return;

	if(head) heads[rule].insert(variable);
	// check if variable appears in rule twice
	if(signs[rule].find(variable) != signs[rule].end() && !head)
	{
		// case: variable appears in head and negated in body - remove from head and body
		if(heads[rule].find(variable) != heads[rule].end() && !positive)
		{ heads[rule].erase(variable); signs[rule].erase(variable); signs[rule].insert(make_pair(createAuxiliaryVertex(), false)); }
		// case: variable appears in body twice (negated or unnegated) - do nothing
		else if(signs[rule][variable] == positive) { }
		// case: any other - ignore the rule completely
		else { heads[rule].clear(); signs[rule].clear(); signs.erase(rule); ignore = rule; }
	}
	else
	{
		signs[rule].insert(map<Variable, bool>::value_type(variable, positive));
	}
}

void DatalogProblem::parse()
{
	this->parser->yyparse();
}

void DatalogProblem::preprocess()
{
	if(signs.size() == (unsigned int)0)
	{
		Rule r = createAuxiliaryVertex();
		Variable var = createAuxiliaryVertex();
		heads[r].insert(var);
		signs[r].insert(make_pair(var, true));
		var = createAuxiliaryVertex();
		signs[r].insert(make_pair(var, true));
	}
}

#ifdef DEBUG
static void printSignMap(SignMap &eht)
{
        for(map<int, map<int, bool> >::iterator it = eht.begin(); it != eht.end(); ++it)
        {
                cout << "clause " << it->first << ": ";
                for(map<int, bool>::iterator sit = it->second.begin(); sit != it->second.end(); ++sit)
                        cout << (sit->second ? "-" : "\0") << sit->first << ", "; cout << "END" << endl;
        }
}
#endif

Hypergraph *DatalogProblem::buildHypergraphRepresentation()
{
#ifdef DEBUG
	printSignMap(signs);
#endif

	VertexSet vertices;
	EdgeSet edges;

	for(SignMap::iterator it = this->signs.begin(); it != this->signs.end(); ++it)
	{
		vertices.insert(it->first);

		for(map<Variable, bool>::iterator v = it->second.begin(); v != it->second.end(); ++v)
		{
			vertices.insert(v->first);
			edges.insert(make_pair(it->first, v->first));
		}
	}

	return Problem::createHypergraphFromSets(vertices, edges);
}
