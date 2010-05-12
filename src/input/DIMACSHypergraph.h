#ifndef DIMACSHYPERGRAPH_H_
#define DIMACSHYPERGRAPH_H_

#include <fstream>

using namespace std;

#include "../Global.h"
#include "DIMACSParser.h"

#include "AbstractHypergraph.h"

class Hypergraph;
class Parser;

class DIMACSHypergraph : public AbstractHypergraph, public DIMACSParser
{
public:
	DIMACSHypergraph();
	DIMACSHypergraph(istream *in);
	virtual ~DIMACSHypergraph();

protected:
	virtual void addVariable(int clause, int variable, bool positive = false);
	virtual int parseInput();
};

#endif
