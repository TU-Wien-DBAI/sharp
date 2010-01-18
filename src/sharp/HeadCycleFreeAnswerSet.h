#ifndef HEADCYCLEFREEANSWERSET_H_
#define HEADCYCLEFREEANSWERSET_H_

#include "AbstractAlgorithm.h"

class HeadCycleFreeAnswerSetTuple : public Tuple
{
public:
	HeadCycleFreeAnswerSetTuple();
	virtual ~HeadCycleFreeAnswerSetTuple();

public:
	set<Variable> variables;
	set<Rule> rules;

	Order order;
	OrderTypes ordertypes;

	map<Variable, set<Rule> > guards;
	set<set<Rule> > guardsdown;

public:
	virtual bool operator<(const Tuple &other) const;
	virtual bool operator==(const Tuple &other) const;
	virtual int hash() const;
};

class HeadCycleFreeAnswerSetAlgorithm : public AbstractAlgorithm
{
public:
	HeadCycleFreeAnswerSetAlgorithm(const Instantiator *instantiator, const ExtendedHypertree *root, 
		const SignMap &signMap, const HeadMap &headMap, const NameMap &nameMap);
	virtual ~HeadCycleFreeAnswerSetAlgorithm();

protected:
	virtual Solution *selectSolution(TupleSet *tuples);
	
	virtual TupleSet *evaluateLeafNode(const ExtendedHypertree *node);
	virtual TupleSet *evaluateBranchNode(const ExtendedHypertree *node);
	virtual TupleSet *evaluateVariableIntroductionNode(const ExtendedHypertree *node);
	virtual TupleSet *evaluateVariableRemovalNode(const ExtendedHypertree *node);
	virtual TupleSet *evaluateRuleIntroductionNode(const ExtendedHypertree *node);
	virtual TupleSet *evaluateRuleRemovalNode(const ExtendedHypertree *node);
};

#endif
