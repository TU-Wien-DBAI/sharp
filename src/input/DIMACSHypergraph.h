#ifndef DIMACSHYPERGRAPH_H_
#define DIMACSHYPERGRAPH_H_

#include <fstream>
#include <map>
#include <list>

using namespace std;

#include "../Global.h"
#include "DIMACSParser.h"

class Hypergraph;
class Parser;

class DIMACSHypergraph : public Hypergraph, public DIMACSParser
{
public:
	DIMACSHypergraph();
	DIMACSHypergraph(istream *in);
	virtual ~DIMACSHypergraph();
	virtual void buildHypergraph(Parser *p = NULL);
	signmap &getSignMap();

protected:
	virtual void addVariable(int iClause, int iVariable, bool bNegative = false);

protected:
	signmap signs;
};

#endif
