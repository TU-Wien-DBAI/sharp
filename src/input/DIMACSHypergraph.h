#ifndef DIMACSHYPERGRAPH_H_
#define DIMACSHYPERGRAPH_H_

#include <fstream>
#include <map>
#include <list>

using namespace std;

typedef map<int, list<pair<int, bool> > > signmap;

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
protected:
	virtual void addVariable(int iClause, int iVariable, bool bNegative = false);
private:
	signmap signs;
};

#endif
