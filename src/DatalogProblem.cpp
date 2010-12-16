#include <fstream>
#include <iostream>

#include "DatalogProblem.h"

#undef yyFlexLexer
#define yyFlexLexer DatalogFlexLexer
#include <FlexLexer.h>

#include "input/DatalogParser.h"

#include "sharp/AnswerSet.h"
#include "sharp/HeadCycleFreeAnswerSet.h"
#include "sharp/HCFAnswerSet.h"

using namespace std;

DatalogProblem::DatalogProblem(istream *stream, int type)
	: Problem(type == DatalogProblem::ASP ? (AbstractAlgorithm *)new AnswerSetAlgorithm(this)
		: (type == DatalogProblem::OLDHCF ? (AbstractAlgorithm *)new HeadCycleFreeAnswerSetAlgorithm(this)
		: /* otherwise */ (AbstractAlgorithm *)new HCFAnswerSetAlgorithm(this)))
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

	if((unsigned int)r >= this->heads.size()) this->heads.resize(r + 100);
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
		{ 
			heads[rule].erase(variable);
			signs[rule].erase(variable);
			signs[rule].insert(make_pair(createAuxiliaryVertex(), false)); 
			types.push_back(VariableVertex);
		}
		// case: variable appears in body twice (negated or unnegated) - do nothing
		else if(signs[rule][variable] == positive) { }
		// case: any other - ignore the rule completely
		else { heads[rule].clear(); signs[rule].clear(); signs.erase(rule); ignore = rule; }
	}
	else
	{
		signs[rule].insert(make_pair(variable, positive));
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
static void printSignMap(SignMap &sm, HeadMap &hm)
{
        for(map<int, map<int, bool> >::iterator it = sm.begin(); it != sm.end(); ++it)
        {
                cout << "rule " << it->first << ": ";
                for(map<int, bool>::iterator sit = it->second.begin(); sit != it->second.end(); ++sit)
                        cout << (sit->second ? "+" : "-") 
				<< sit->first 
				<< (hm[it->first].find(sit->first) == hm[it->first].end() ? "b" : "h") 
				<< ", ";
		cout << "END" << endl;
        }
}
#endif

Hypergraph *DatalogProblem::buildHypergraphRepresentation()
{
#ifdef DEBUG
	printVertexNames(cout);
	printSignMap(signs, heads);
#endif

	VertexSet vertices1;
	VertexSet vertices2;
	EdgeSet edges;

	for(SignMap::iterator it = this->signs.begin(); it != this->signs.end(); ++it)
	{
		vertices1.insert(it->first);

		for(map<Variable, bool>::iterator v = it->second.begin(); v != it->second.end(); ++v)
		{
			vertices2.insert(v->first);
			edges.insert(make_pair(it->first, v->first));
		}
	}

	// ENABLE BIPARTITE GRAPH OPTIMIZATIONS
	// this->setDecompositionOptions(BipartiteGraph, (void *)vertices1.size());
	return Problem::createGraphFromDisjointSets(vertices1, vertices2, edges);
}
