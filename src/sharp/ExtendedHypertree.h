#ifndef EXTENDEDHYPERTREE_H_
#define EXTENDEDHYPERTREE_H_

#include <list>
#include <set>

using namespace std;

#include "../htree/Hypertree.h"

class ExtendedHypertree : public Hypertree
{
	enum { ROOT = 0, LEAF = 1, BRANCH = 2, VARREM = 3, CLREM = 4, VARINTR = 5, CLINTR = 6 };
public:
	ExtendedHypertree(Hypertree *node);
	ExtendedHypertree(set<int> clauses, set<int> variables);
	virtual ~ExtendedHypertree();
	void normalize();
	int getType();
	bool isRoot();

	set<int> &getClauses();
	set<int> &getVariables();

private:
	set<int> clauses;
	set<int> variables;

	void adapt();
	ExtendedHypertree *parent();

	static Hypertree *createChild(Hypertree *child, set<int> clauses, set<int> variables);
};
#endif
