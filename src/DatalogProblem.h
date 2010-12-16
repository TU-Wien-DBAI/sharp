#ifndef DATALOGPROBLEM_H_
#define DATALOGPROBLEM_H_

#include <string>

#include "Global.h"
#include "Problem.h"

typedef Vertex Rule;
typedef Vertex Variable;

typedef VertexSet RuleSet;
typedef VertexSet VariableSet;

enum DatalogVertexType
{
	RuleVertex = 0,
	VariableVertex = 1
};

typedef std::map<Rule, std::map<Variable, bool> > SignMap;
typedef std::vector<VariableSet> HeadMap;
typedef std::vector<DatalogVertexType> TypeMap;

class DatalogParser;

class DatalogProblem : public Problem
{
public:
	DatalogProblem(std::istream *stream, int type = DatalogProblem::ASP);
	virtual ~DatalogProblem();

	SignMap &getSignMap();
	HeadMap &getHeadMap();
	TypeMap &getTypeMap();

	bool isRule(Vertex v);
	bool isVariable(Vertex v);

	Rule addNewRule();
	Variable addVariable(std::string name);
	void addEdge(Rule rule, Variable variable, bool positive, bool head);

protected:
	virtual void parse();
	virtual void preprocess();
	virtual Hypergraph *buildHypergraphRepresentation();

private:
	SignMap signs;
	HeadMap heads;
	TypeMap types;

	DatalogParser *parser;

public:
	static const int ASP = 0;
	static const int OLDHCF = 1;
	static const int NEWHCF = 2;
};

#endif //DATALOGPROBLEM_H_
