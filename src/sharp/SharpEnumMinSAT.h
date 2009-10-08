#ifndef SHARPENUMMINSAT_H_
#define SHARPENUMMINSAT_H_

#include <string>
#include <vector>
#include <list>
#include <set>

#include <gmp.h>
#include <gmpxx.h>

using namespace std;

#include "../Global.h"
#include "ExtendedHypertree.h"
#include "LazySolutionSet.h"


struct SharpValue
{
	set<int> assignmentVariables;
	set<int> assignmentClauses;

	mutable set<Atom> guards;

	mutable mpz_class value;
	mutable LazySolutionSet *solutionSet;

	bool operator<(const SharpValue &other) const;
};

class SharpEnumMinSAT
{
public:
	SharpEnumMinSAT(ExtendedHypertree *root, SignMap &signs);
	virtual ~SharpEnumMinSAT();
	virtual pair<mpz_class, SolutionSet> evaluate() const;

protected:
	ExtendedHypertree *root;
	SignMap &signs;

	set<SharpValue> &eval(ExtendedHypertree *node) const;

	set<SharpValue> &evalLeaf(ExtendedHypertree *node) const;
	set<SharpValue> &evalBranch(ExtendedHypertree *node) const;
	set<SharpValue> &evalVariableIntroduction(ExtendedHypertree *node) const;
	set<SharpValue> &evalVariableRemoval(ExtendedHypertree *node) const;
	set<SharpValue> &evalClauseIntroduction(ExtendedHypertree *node) const;
	set<SharpValue> &evalClauseRemoval(ExtendedHypertree *node) const;

	set<int>	trueClauses(const set<int> &positives, const set<int> &negatives, const set<int> &clauses) const;
	bool		isTrue(const set<int> &positives, const set<int> &all, int clause) const;
	
	static Partition	partition(const set<int> &variables);
};

#endif /*SHARPENUMMINSAT_H_*/
