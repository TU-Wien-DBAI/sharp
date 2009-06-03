#ifndef EXTENDEDHYPERTREE_H_
#define EXTENDEDHYPERTREE_H_

#include <list>
#include <set>

using namespace std;

#include "../htree/Hypertree.h"

class ExtendedHypertree : public Hypertree
{
public:
	enum { ROOT = 0, LEAF = 1, BRANCH = 2, VARREM = 3, CLREM = 4, VARINTR = 5, CLINTR = 6 };
	
public:
	ExtendedHypertree(Hypertree *node);
	ExtendedHypertree(set<int> clauses, set<int> variables);
	virtual ~ExtendedHypertree();
	void normalize();
	int getType() const;
	bool isRoot() const;
	const set<int> &getClauses() const;
	const set<int> &getVariables() const;
	ExtendedHypertree *parent() const;
	ExtendedHypertree *firstChild() const;
	ExtendedHypertree *secondChild() const;

private:
	set<int> clauses;
	set<int> variables;

	void adapt();

	static Hypertree *createChild(Hypertree *child, set<int> clauses, set<int> variables);
};
#endif
