#ifndef EXTENDEDHYPERTREE_H_
#define EXTENDEDHYPERTREE_H_

#include <list>
#include <set>

using namespace std;

#include "../htree/Hypertree.h"

class ExtendedHypertree : public Hypertree
{
public:
	enum { LEAF = 1, BRANCH = 2, VARREM = 3, CLREM = 4, VARINTR = 5, CLINTR = 6 };
	
public:
	ExtendedHypertree(Hypertree *node);
	ExtendedHypertree(set<int> clauses, set<int> variables);
	virtual ~ExtendedHypertree();
	void normalize();
	int getType() const;
	int getDifference() const;
	bool isRoot() const;
	const set<int> &getClauses() const;
	const set<int> &getVariables() const;
	ExtendedHypertree *parent() const;
	ExtendedHypertree *firstChild() const;
	ExtendedHypertree *secondChild() const;

private:
	int type;
	int difference;
	
	set<int> clauses;
	set<int> variables;

	void adapt();
	int calculateType();

	static ExtendedHypertree *createChild(ExtendedHypertree *child, set<int> clauses, set<int> variables);
};
#endif
