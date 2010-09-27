#ifndef ARGUMENTATIONPROBLEM_H_
#define ARGUMENTATIONPROBLEM_H_

#include <string>
#include <fstream>

#include "Global.h"
#include "Problem.h"

typedef Vertex Argument;
typedef VertexSet ArgumentSet;
typedef std::set<std::pair<std::string, std::string> > AttackSet;

class ArgumentationParser;

class ArgumentationProblem : public Problem
{
public:
	ArgumentationProblem(std::istream *stream);
	virtual ~ArgumentationProblem();

	void addArgument(std::string argumentId);
	void addAttack(std::string attackerId, std::string attackedId);

protected:
	virtual void parse();
	virtual void preprocess();
	virtual Hypergraph *buildHypergraphRepresentation();

private:
	AttackSet attacks;
	ArgumentSet args;
	ArgumentationParser *parser;
};

#endif //ARGUMENTATIONPROBLEM_H_
