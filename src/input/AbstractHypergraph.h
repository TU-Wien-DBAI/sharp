#ifndef ABSTRACTHYPERGRAPH_H_
#define ABSTRACTHYPERGRAPH_H_

#include <fstream>

using namespace std;

#include "../Global.h"
#include "../htree/Hypergraph.h"

class Hypergraph;
class Parser;

class AbstractHypergraph : public Hypergraph
{
public:
	AbstractHypergraph();
	AbstractHypergraph(istream *in);
	virtual ~AbstractHypergraph();
	virtual int buildHypergraph(Parser *p = NULL);
	SignMap &getSignMap();
	NameMap &getNameMap();
	HeadMap &getHeadMap();

protected:
	virtual void addVariable(int rule, const char *name, bool negative = false, bool head = false);
	virtual int parseInput() = 0;

protected:
	SignMap signs;
	NameMap names;
	HeadMap heads;

private:
	int storeVariable(const char *name, bool &alreadyPresent);

private:
	map<string, int> reverseNames;
	vector<Node *> rules;
	vector<Node *> variables;
};

#endif
