#ifndef SHARPSAT_H_
#define SHARPSAT_H_

#include <string>
#include <set>

#include <gmp.h>
#include <gmpxx.h>

using namespace std;

#include "../Global.h"
#include "ExtendedHypertree.h"

struct PartitionValue
{
	set<int> positive;
	set<int> negative;
	set<int> clauses;
	mutable mpz_class value;
	
	PartitionValue(set<int> positive, set<int> negative, set<int> clauses);
	PartitionValue(const PartitionValue &other);

	bool operator==(const PartitionValue &other) const;
	bool operator<(const PartitionValue &other) const;
};

class SharpSAT
{
public:
	SharpSAT(ExtendedHypertree *root, signmap &signs);
	virtual ~SharpSAT();
	virtual string evaluate() const;

protected:
	ExtendedHypertree *root;
	signmap &signs;

private:
	set<PartitionValue> &eval(const ExtendedHypertree *node) const;
	set<PartitionValue> &partition(const set<int> &variables, const set<int> &clauses) const;
	set<int> istrue(const set<int> &positives, const set<int> &negatives, const set<int> &clauses) const;
	set<PartitionValue> &introduceVariable(set<PartitionValue> &base, int variable, const set<int> &clauses) const;
};

#endif /*SHARPSAT_H_*/
