#include <cmath>
#include <vector>
#include <algorithm>

#include <iostream>

using namespace std;

#include "../support/support.h"
#include "SharpSAT.h"

typedef PartitionValue PV;

static set<PV> &removeVariable(set<PV> &base, int variable);
static set<PV> &removeClause(set<PV> &base, int clause);
static set<PV> &merge(set<PV> &left, set<PV> &right);
static int head(const set<int> &base, const set<int> &minus);

static void printPartitions(set<PV> &partvals)
{
	cout << "PARTITONS: " << partvals.size() << endl;
	for(set<PV>::iterator it = partvals.begin(); it != partvals.end(); ++it)
	{
		cout << "pos: "; printIntSet(it->positive);
		cout << ", neg: "; printIntSet(it->negative);
		cout << ", cla: "; printIntSet(it->clauses);
		cout << ", val: " << it->value << endl;
	}
	cout << "---" << endl;	
}

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
	equal_to<set<int> > eq;
	mpz_class sum = 0;

	for(set<PV>::iterator it = result.begin(); it != result.end(); ++it)
	{
		if(eq(it->clauses, this->root->getClauses())) sum += it->value;
	}
	
	return sum.get_str();
}

set<PV> &SharpSAT::eval(ExtendedHypertree *node) const
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
	case ExtendedHypertree::CLREM:
		return removeClause(eval(node->firstChild()), head(node->firstChild()->getClauses(), node->getClauses()));
	case ExtendedHypertree::CLINTR:
		return introduceClause(eval(node->firstChild()), head(node->getClauses(), node->firstChild()->getClauses()));
	}
	
	CNULL(NULL);
	return *((set<PV> *)NULL);
}

set<int> SharpSAT::istrue(const set<int> &positives, const set<int> &negatives, const set<int> &clauses) const
{
	set<int> newclauses;

	for(set<int>::const_iterator cl = clauses.begin(); cl != clauses.end(); ++cl)
        {
		bool add = false;
                for(set<int>::const_iterator var = positives.begin(); !add && var != positives.end(); ++var)
                {
                	map<int, bool> &posneg = this->signs[*cl];
                        map<int, bool>::iterator it = posneg.find(*var);
                        add = it != posneg.end() && !it->second;
                }
                for(set<int>::const_iterator var = negatives.begin(); !add && var != negatives.end(); ++var)
                {
                        map<int, bool> &posneg = this->signs[*cl];
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
	typedef set_t::iterator iter_t;

	vector<set_t> positives, negatives; positives.reserve(int(pow(2, variables.size()))); negatives.reserve(int(pow(2, variables.size())));
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

		if(!elements.empty() && ++elements.back() == variables.end()) elements.pop_back();
		else 
		{
			iter_t si;
			if(elements.empty()) si = variables.begin(); else { si = elements.back(); ++si; }
			for(; si != variables.end(); ++si) elements.push_back(si);
		}

	}
	while(!elements.empty());
	
	for(int i = 0; i < int(pow(2, variables.size())); ++i)
		partvals.insert(PV(positives[i], negatives[i], this->istrue(positives[i], negatives[i], clauses)));

	return partvals;
}

set<PV> &SharpSAT::introduceVariable(set<PV> &base, int variable, const set<int> &clauses) const
{
	set<int> one, empty; one.insert(variable);
	set<int> pos = istrue(one, empty, clauses), neg = istrue(empty, one, clauses);
	set<PV> &ptemp = *new set<PV>(), ntemp;
	
	for(set<PV>::iterator it = base.begin(); it != base.end(); ++it)
	{
		PV p = PV(*it), n = PV(*it);
		p.positive.insert(variable);
		n.negative.insert(variable);
		p.clauses.insert(pos.begin(), pos.end());
		n.clauses.insert(neg.begin(), neg.end());
		set<PV>::iterator pit = ptemp.find(p);
		if(pit != ptemp.end()) pit->value += p.value; else ptemp.insert(p);
		pit = ntemp.find(n);
		if(pit != ntemp.end()) pit->value += n.value; else ntemp.insert(n);
	}

	for(set<PV>::iterator it = ntemp.begin(); it != ntemp.end(); ++it)
	{
		set<PV>::iterator pit = ptemp.find(*it);
		if(pit != ptemp.end()) pit->value += it->value;
		else ptemp.insert(*it);
	}

	delete &base;
	
	return ptemp;
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

set<PV> &SharpSAT::introduceClause(set<PV> &base, int clause) const
{
	set<int> temp; temp.insert(clause);
	set<PV> &result = *new set<PV>();
	
	for(set<PV>::iterator it = base.begin(); it != base.end(); ++it)
	{
		PV p = PV(*it);

		if(it->clauses.find(clause) == it->clauses.end() && this->istrue(it->positive, it->negative, temp).size() != 0) 
			p.clauses.insert(clause);

		set<PV>::iterator pit = result.find(p);
		if(pit != result.end()) pit->value += p.value;
		else result.insert(p);		
	}

	delete &base;

	return result;
}

static set<PV> &removeClause(set<PV> &base, int clause)
{
	set<PV> &result = *new set<PV>();
	list<set<PV>::iterator> temp;
	
	for(set<PV>::iterator it = base.begin(); it != base.end(); ++it) 
		if(it->clauses.find(clause) != it->clauses.end()) 
			temp.push_back(it);

	for(list<set<PV>::iterator>::iterator it = temp.begin(); it != temp.end(); ++it)
	{
		PV p = PV(**it);
		p.clauses.erase(clause);
		set<PV>::iterator pit = result.find(p);
		if(pit != result.end()) pit->value += p.value;
		else result.insert(p);
	}

	delete &base;

	return result;
}

static set<PV> &merge(set<PV> &left, set<PV> &right)
{
	int i = 0;
	equal_to<set<int> > eq;
	set<PV> &merged = *new set<PV>();

	set<PV>::iterator rit = right.begin();
	for(set<PV>::iterator lit = left.begin(); lit != left.end();)
	{
		if(rit != right.end() 
			&& lit->positive.size() == rit->positive.size() 
			&& lit->negative.size() == rit->negative.size()
			&& eq(lit->positive, rit->positive) 
			&& eq(lit->negative, rit->negative))
		{
			PV p = PV(*lit);
			for(set<int>::iterator it = rit->clauses.begin(); it != rit->clauses.end(); ++it) p.clauses.insert(*it);
			p.value = lit->value * (rit++)->value; ++i;
			set<PV>::iterator pit = merged.find(p);
			if(pit != merged.end()) pit->value += p.value;
			else merged.insert(p);
			continue;
		}

		if(i != 0) { for(; i > 0; --i) --rit; ++lit; }
		else { if(rit == right.end()) break; if(*lit < *rit) ++lit; else ++rit; }
	}

	delete &left; delete &right;

	return merged;
}

static int head(const set<int> &base, const set<int> &minus)
{
	vector<int> diff(base.size());
	vector<int>::iterator i;

	i = set_difference(base.begin(), base.end(), minus.begin(), minus.end(), diff.begin());
	return diff[0];	
}
