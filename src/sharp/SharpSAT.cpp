#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

#include "../support/support.h"
#include "SharpSAT.h"

typedef PartitionValue PV;

static set<PV> &removeVariable(set<PV> &base, int variable);
static set<PV> &introduceClause(set<PV> &base, int clause);
static set<PV> &removeClause(set<PV> &base, int clause);
static set<PV> &merge(set<PV> &left, set<PV> &right);
static int head(const set<int> &base, const set<int> &minus);

PartitionValue::PartitionValue(set<int> positive, set<int> negative, set<int> clauses) : positive(positive), negative(negative), clauses(clauses)
{
	this->value = 1;
}

PartitionValue::PartitionValue(const PartitionValue &other) : positive(other.positive), negative(other.negative), clauses(other.clauses) 
{
	this->value = other.value;
}

bool PartitionValue::operator==(const PartitionValue &other) const
{
	equal_to<set<int> > eq;	
	return eq(this->positive, other.positive) && eq(this->negative, other.negative) && eq(this->clauses, other.clauses);
}

bool PartitionValue::operator<(const PartitionValue &other) const
{
	less<set<int> > l; equal_to<set<int> > eq;

	if(l(this->positive, other.positive)) return true;
	else if(eq(this->positive, other.positive))
	{
		if(l(this->negative, other.negative)) return true;
		else if(eq(this->negative, other.negative) && l(this->clauses, other.clauses)) return true;
	}

	return false;
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
	set<PV> &result = eval(this->root);
	//TODO
	return "wow";
}

set<PV> &SharpSAT::eval(const ExtendedHypertree *node) const
{
	switch(node->getType())
	{
	case ExtendedHypertree::LEAF:
		return partition(node->getVariables(), node->getClauses());
	case ExtendedHypertree::VARREM:
		return removeVariable(eval(node->firstChild()), head(node->firstChild()->getVariables(), node->getVariables()));
	case ExtendedHypertree::VARINTR:
		return introduceVariable(eval(node->firstChild()), head(node->getVariables(), node->firstChild()->getVariables()), node->getClauses());
	case ExtendedHypertree::BRANCH:
		return merge(eval(node->firstChild()), eval(node->secondChild()));
	case ExtendedHypertree::CLINTR:
		return introduceClause(eval(node->firstChild()), head(node->firstChild()->getClauses(), node->getClauses()));
	case ExtendedHypertree::CLREM:
		return removeClause(eval(node->firstChild()), head(node->getClauses(), node->firstChild()->getClauses()));
	}
	
	CNULL(NULL);
}

set<int> SharpSAT::istrue(const set<int> &positives, const set<int> &negatives, const set<int> &clauses) const
{
	set<int> newclauses;

	for(set<int>::const_iterator cl = clauses.begin(); cl != clauses.end(); ++cl)
        {
		bool add = false;
                for(set<int>::const_iterator var = positives.begin(); !add && var != positives.end(); ++var)
                {
                	map<int, bool> posneg = this->signs[*cl];
                        map<int, bool>::iterator it = posneg.find(*var);
                        add = it != posneg.end() && it->second;
                }
                for(set<int>::const_iterator var = negatives.begin(); !add && var != negatives.end(); ++var)
                {
                        map<int, bool> posneg = this->signs[*cl];
                        map<int, bool>::iterator it = posneg.find(*var);
                        add = it != posneg.end() && it->second;
                }
                if(add) newclauses.insert(*cl);
    	}
	
	return newclauses;	
}

set<PV> &SharpSAT::partition(const set<int> &variables, const set<int> &clauses) const
{
	typedef set<int> set_t;
	typedef set_t::const_iterator iter_t;

	vector<set_t> positives(int(pow(2, variables.size()))), negatives(int(pow(2, variables.size())));
	vector<iter_t> elements;

	set<PV> &partvals = *new set<PV>();

	do
	{
		set_t newpos, newneg;
		vector<iter_t>::iterator vi = elements.begin();
		for(iter_t si = variables.begin(); si != variables.end(); ++si)
			if(vi != elements.end() && si == *vi) { newpos.insert(*si); ++vi; }
			else { newneg.insert(*si); }

		positives.push_back(newpos); negatives.push_back(newneg);

		if(!elements.empty() && ++elements.back() == variables.end()) { elements.pop_back(); }
		else for(iter_t si = elements.empty() ? variables.begin() : elements.back(); si != variables.end(); ++si) elements.push_back(elements.empty() ? si : ++si);
	}
	while(!elements.empty());
	
	for(int i = 0; i < int(pow(2, variables.size())); ++i)
		partvals.insert(PartitionValue(positives[i], negatives[i], this->istrue(positives[i], negatives[i], clauses)));

	return partvals;
}

set<PV> &SharpSAT::introduceVariable(set<PV> &base, int variable, const set<int> &clauses) const
{
	set<int> one, empty; one.insert(variable);
	set<int> pos = istrue(one, empty, clauses), neg = istrue(empty, one, clauses);
	set<PV> ptemp = base, &ntemp = base;
	
	
	for(set<PV>::iterator it = base.begin(); it != base.end(); ++it)
		if(it->positive.find(variable) == it->positive.end())
		{
			PV p = PV(*it);
			ptemp.erase(*it);
			p.positive.insert(variable);
			set<PV>::iterator pit = ptemp.find(p);
			if(pit != ptemp.end()) pit->value += p.value;
			else ptemp.insert(p);
		}
		else if(it->negative.find(variable) == it->negative.end())
		{
			PV p = PV(*it);
			ntemp.erase(*it);
			p.negative.insert(variable);
			set<PV>::iterator pit = ntemp.find(p);
			if(pit != ntemp.end()) pit->value += p.value;
			else ntemp.insert(p);
		}

	for(set<PV>::iterator it = ptemp.begin(); it != ptemp.end(); ++it)
	{
		set<PV>::iterator pit = base.find(*it);
		if(pit != base.end()) pit->value += it->value;
		else base.insert(*it);
	}

	return base;
}
		

static set<PV> &removeVariable(set<PV> &base, int variable)
{
	list<set<PV>::iterator> temp;
	
	for(set<PV>::iterator it = base.begin(); it != base.end(); ++it) 
		if(it->negative.find(variable) != it->negative.end() || it->positive.find(variable) != it->positive.end()) 
			temp.push_back(it);

	for(list<set<PV>::iterator>::iterator it = temp.begin(); it != temp.end(); ++it)
	{
		PV p = PV(**it);
		base.erase(*it);
		p.positive.erase(variable); p.negative.erase(variable);
		set<PV>::iterator pit = base.find(p);
		if(pit != base.end()) pit->value += p.value;
		else base.insert(p);
	}

	return base;
}

static set<PV> &introduceClause(set<PV> &base, int clause)
{
	//TODO
}

static set<PV> &removeClause(set<PV> &base, int clause)
{
	//TODO
}

static set<PV> &merge(set<PV> &left, set<PV> &right)
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
