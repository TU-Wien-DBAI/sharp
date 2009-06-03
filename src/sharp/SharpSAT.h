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
	mpz_class value;
	
	PartitionValue();
	PartitionValue(const PartitionValue &other);

	bool operator==(const PartitionValue &other) const;
	bool operator!=(const PartitionValue &other) const;
	bool operator>(const PartitionValue &other) const;
	bool operator<=(const PartitionValue &other) const;
	bool operator>=(const PartitionValue &other) const;
	bool operator<(const PartitionValue &other) const;
	PartitionValue &operator=(const PartitionValue &other);
	PartitionValue &operator+=(const mpz_class value);	
};

class SharpSAT
{
private:
	static set<PartitionValue &> &partition(const set<int> &variables, const set<int> &clauses);

public:
	SharpSAT(ExtendedHypertree *root, signmap &signs);
	virtual ~SharpSAT();
	virtual string evaluate() const;

protected:
	ExtendedHypertree *root;
	signmap &signs;

private:
	set<PartitionValue &> &eval(const ExtendedHypertree *node) const;
};

#endif /*SHARPSAT_H_*/
