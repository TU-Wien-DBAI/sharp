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

DatalogProblem::DatalogProblem(istream *stream, int type, bool benchmark)
	: Problem(type == DatalogProblem::ASP ? (AbstractAlgorithm *)new AnswerSetAlgorithm(this)
		: (type == DatalogProblem::OLDHCF ? (AbstractAlgorithm *)new HeadCycleFreeAnswerSetAlgorithm(this)
		: /* otherwise */ (AbstractAlgorithm *)new HCFAnswerSetAlgorithm(this)), benchmark)
{
	this->parser = new DatalogParser(new DatalogFlexLexer(), stream, this);
	this->types.push_back(RuleVertex); // numbering starts at 1, so create a temporary entry at 0
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

	///NEW
	this->rulebody.push_back(vector<bool>());
	this->rulenegatives.push_back(vector<bool>());
	this->ruleheads.push_back(vector<bool>());
	///ENDNEW

	return r;
}

Variable DatalogProblem::addVariable(string name)
{
	Vertex v = storeVertexName(name);

	if(this->types.size() == (unsigned int)v) this->types.push_back(VariableVertex);

	///NEW
	this->rulebody.push_back(vector<bool>());
	this->rulenegatives.push_back(vector<bool>());
	this->ruleheads.push_back(vector<bool>());
	///ENDNEW

	return v;
}

void DatalogProblem::addEdge(Rule rule, Variable variable, bool positive, bool head)
{
	static Rule ignore = -1;
	if(ignore == rule) return;

	Variable currAux = -1;
	
	if(head) heads[rule].insert(variable);
	// check if variable appears in rule twice
	if(signs[rule].find(variable) != signs[rule].end() && !head)
	{
		// case: variable appears in head and negated in body - remove from head and body
		if(heads[rule].find(variable) != heads[rule].end() && !positive)
		{ 
			heads[rule].erase(variable);
			signs[rule].erase(variable);
			signs[rule].insert(make_pair(currAux = createAuxiliaryVertex(), false)); 
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

	///NEW
	if(this->ruleheads[rule].size() <= variable) this->ruleheads[rule].resize(variable + 1);
	if(this->rulebody[rule].size() <= variable) this->rulebody[rule].resize(variable + 1);
	if(this->rulenegatives[rule].size() <= variable) this->rulenegatives[rule].resize(variable + 1);

	if(head) this->ruleheads[rule][variable] = true;
	
	if(this->rulebody[rule][variable] && !head)
	{
		if(this->ruleheads[rule][variable] && !positive)
		{
			this->ruleheads[rule][variable] = false;
			this->rulebody[rule][variable] = false;
			this->rulenegatives[rule][variable] = false;
			this->rulebody[rule].resize(currAux + 1);
			this->rulenegatives[rule].resize(currAux + 1);
			this->rulebody[rule][currAux] = true;
			this->rulenegatives[rule][currAux] = true;
		}
		else if(this->isInRuleBody(rule, variable) && this->isInNegativeRuleBody(rule, variable) == !positive) { }
		else { this->rulebody[rule].clear(); this->ruleheads[rule].clear(); this->rulenegatives[rule].clear(); ignore = rule; }
	}
	else
	{
		this->rulebody[rule][variable] = true;
		this->rulenegatives[rule][variable] = !positive;
	}
	///ENDNEW
}

void DatalogProblem::parse()
{
	this->parser->yyparse();
}

void DatalogProblem::preprocess()
{
	Variable currAuxVar1 = VERTEXNOTFOUND,
		 currAuxVar2 = VERTEXNOTFOUND;
	Rule currAuxRule = VERTEXNOTFOUND;

	if(signs.size() == (unsigned int)0)
	{
		Rule r = currAuxRule = createAuxiliaryVertex();
		Variable var = currAuxVar1 = createAuxiliaryVertex();
		heads[r].insert(var);
		signs[r].insert(make_pair(var, true));
		var = currAuxVar2 = createAuxiliaryVertex();
		signs[r].insert(make_pair(var, true));
	}

	///NEW
	if(this->rulebody.size() == (unsigned int)0)
	{
		this->rulebody.resize(currAuxRule + 1);
		this->rulebody[currAuxRule].resize(currAuxVar2 + 1);
		this->rulebody[currAuxRule][currAuxVar1] = true;
		this->rulebody[currAuxRule][currAuxVar2] = true;
	}

	unsigned int overallSize = this->getVertexCount();
	for(unsigned int i = 0; i < overallSize; ++i)
	{
		if(this->isRule((Vertex)i))
		{
			this->rulebody[i].resize(overallSize + 1);
			this->rulenegatives[i].resize(overallSize + 1);
			this->ruleheads[i].resize(overallSize + 1);
		}
	}
	///ENDNEW
}

#ifdef DEBUG
static void printSignMap(SignMap &sm, HeadMap &hm)
{
        for(SignMap::iterator it = sm.begin(); it != sm.end(); ++it)
        {
                cout << "rule " << it->first << ": ";
                for(map<Variable, bool>::iterator sit = it->second.begin(); sit != it->second.end(); ++sit)
                        cout << (sit->second ? "+" : "-") 
				<< sit->first 
				<< (hm[it->first].find(sit->first) == hm[it->first].end() ? "b" : "h") 
				<< ", ";
		cout << "END" << endl;
        }
}

static void printNewSignMaps(VariableMap &body, VariableMap &neg, VariableMap &heads)
{
	for(unsigned int i = 1; i < body.size(); ++i)
	{
		if(body[i].size() == (unsigned int)0) continue;

		cout << "vertex: " << i << endl;
		cout << "body: "; printBoolVector(body[i]); cout << endl;
		cout << "neg:  "; printBoolVector(neg[i]); cout << endl;
		cout << "head: "; printBoolVector(heads[i]); cout << endl;
	}
}
#endif

Hypergraph *DatalogProblem::buildHypergraphRepresentation()
{
#ifdef DEBUG
	printVertexNames(cout);
	printSignMap(signs, heads);
	printNewSignMaps(rulebody, rulenegatives, ruleheads);
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

bool DatalogProblem::isInRuleBody(Rule r, Variable v)
{
	return this->rulebody[r][v];
}

bool DatalogProblem::isInNegativeRuleBody(Rule r, Variable v)
{
	return this->rulenegatives[r][v];
}

bool DatalogProblem::isInRuleHead(Rule r, Variable v)
{
	return this->ruleheads[r][v];
}

