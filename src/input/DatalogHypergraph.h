#ifndef DATALOGHYPERGRAPH_H_
#define DATALOGHYPERGRAPH_H_

#include <fstream>

using namespace std;

#include "../Global.h"
#include "DatalogParser.h"

#include "AbstractHypergraph.h"

class Hypergraph;
class Parser;

class DatalogHypergraph : public AbstractHypergraph, public DatalogParser
{
public:
	DatalogHypergraph();
	DatalogHypergraph(istream *in);
	virtual ~DatalogHypergraph();

protected:
	virtual void addVariable(int rule, const char *name, bool negative = false, bool head = false);
	virtual int parseInput();
};

#endif
