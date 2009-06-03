#include <vector>
#include <algorithm>

using namespace std;

#include "../support/support.h"
#include "SharpSAT.h"

typedef PartitionValue PV;

static set<PV &> &introduceVariable(set<PV &> &base, int variable);
static set<PV &> &removeVariable(set<PV &> &base, int variable);
static set<PV &> &introduceClause(set<PV &> &base, int clause);
static set<PV &> &removeClause(set<PV &> &base, int clause);
static set<PV &> &merge(set<PV &> &left, set<PV &> &right);
static set<PV &> &partition(const set<int> &variables, const set<int> &clauses);
static int head(const set<int> &base, const set<int> &minus);

PartitionValue::PartitionValue()
{
	this->value = 0;
}

PartitionValue::PartitionValue(const PartitionValue &other)
{
	this->positive = other.positive;
	this->negative = other.negative;
	this->clauses = other.clauses;
	this->value = other.value;
}

PartitionValue &PartitionValue::operator=(const PartitionValue &other)
{
	this->positive = other.positive;
	this->negative = other.negative;
	this->clauses = other.clauses;
	this->value = other.value;
}

bool PartitionValue::operator==(const PartitionValue &other) const
{
	
	return 	this->positive.size() <= other.positive.size() ? 
			equal(this->positive.begin(), this->positive.end(), other.positive.begin()) :
			equal(other.positive.begin(), other.positive.end(), this->positive.begin()) &&
		this->negative.size() <= other.negative.size() ?
			equal(this->negative.begin(), this->negative.end(), other.negative.begin()) :
			equal(other.negative.begin(), other.negative.end(), this->negative.begin()) &&
		this->clauses.size() <= other.clauses.size() ?
			equal(this->clauses.begin(), this->clauses.end(), other.clauses.begin()) :
			equal(other.clauses.begin(), other.clauses.end(), this->clauses.begin());
}

bool PartitionValue::operator!=(const PartitionValue &other) const
{
	return !(*this == other);
}

bool PartitionValue::operator<(const PartitionValue &other) const
{
	set<int>::iterator i, j;
	
	if(this->positive.size() <= other.positive.size())
	{
		for(i = this->positive.begin(), j = other.positive.begin(); i != this->positive.end(); ++i)
			if(*i < *j) return true; else if(*j < *i) return false; else ++j;
		if(this->positive.size() != other.positive.size()) return true;
	}
	else
	{
		for(i = other.positive.begin(), j = this->positive.begin(); i != other.positive.end(); ++i)
			if(*i > *j) return true; else if(*j > *i) return false; else ++j;
		return false;
	}

	if(this->negative.size() <= other.negative.size())
	{
		for(i = this->negative.begin(), j = other.negative.begin(); i != this->negative.end(); ++i)
			if(*i < *j) return true; else if(*j < *i) return false; else ++j;
		if(this->negative.size() != other.negative.size()) return true;
	}
	else
	{
		for(i = other.negative.begin(), j = this->negative.begin(); i != other.negative.end(); ++i)
			if(*i > *j) return true; else if(*j > *i) return false; else ++j;
		return false;
	}
	
	if(this->clauses.size() <= other.clauses.size())
	{
		for(i = this->clauses.begin(), j = other.clauses.begin(); i != this->clauses.end(); ++i)
			if(*i < *j) return true; else if(*j < *i) return false; else ++j;
		if(this->clauses.size() != other.clauses.size()) return true;
	}
	else
	{
		for(i = other.clauses.begin(), j = this->clauses.begin(); i != other.clauses.end(); ++i)
			if(*i > *j) return true; else if(*j > *i) return false; else ++j;
		return false;
	}

	return false;
}

bool PartitionValue::operator<=(const PartitionValue &other) const
{
	return *this == other || *this < other;
}

bool PartitionValue::operator>=(const PartitionValue &other) const
{
	return !(*this < other);
}

bool PartitionValue::operator>(const PartitionValue &other) const
{
	return !(*this <= other);
}

PartitionValue &PartitionValue::operator+=(const mpz_class value)
{
	this->value += value;
	return *this;
}

SharpSAT::SharpSAT(ExtendedHypertree *root, signmap &signs) : signs(signs)
{
	this->root = root;
}

SharpSAT::~SharpSAT()
{
}

string SharpSAT::evaluate() const
{
	set<PV &> &result = eval(this->root);
	//TODO
	return "wow";
}

set<PV &> &SharpSAT::eval(const ExtendedHypertree *node) const
{
	switch(node->getType())
	{
	case ExtendedHypertree::LEAF:
		return partition(node->getVariables(), node->getClauses());
	case ExtendedHypertree::VARREM:
		return removeVariable(eval(node->firstChild(), head(node->firstChild()->getVariables(), node->getVariables())));
	case ExtendedHypertree::VARINTR:
		return introduceVariable(eval(node->firstChild(), head(node->getVariables(), node->firstChild()->getVariables())));
	case ExtendedHypertree::BRANCH:
		return merge(eval(node->firstChild()), eval(node->secondChild()));
	case ExtendedHypertree::CLINTR:
		return introduceClause(eval(node->firstChild()), head(node->firstChild()->getClauses(), node->getClauses()));
	case ExtendedHypertree::CLREM:
		return removeClause(eval(node->firstChild()), head(node->getClauses(), node->firstChild()->getClauses()));
	}
	
	CNULL(NULL);
}

static set<PV &> &introduceVariable(set<PV &> &base, int variable)
{
	//TODO
}

static set<PV &> &removeVariable(set<PV &> &base, int variable)
{
	//TODO
}

static set<PV &> &introduceClause(set<PV &> &base, int clause)
{
	//TODO
}

static set<PV &> &removeClause(set<PV &> &base, int clause)
{
	//TODO
}

static set<PV &> &merge(set<PV &> &left, set<PV &> &right)
{
	//TODO
}

static set<PV &> &partition(const set<int> &variables, const set<int> &clauses)
{
	//TODO
}

static int head(const set<int> &base, const set<int> &minus)
{
	vector<int> diff(base.size());
	vector<int>::iterator i;

	i = set_difference(base.begin(), base.end(), minus.begin(), minus.end(), diff.begin());
	return diff[0];	
}
