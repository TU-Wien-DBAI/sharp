#ifndef HEADCYCLEFREEANSWERSET_H_
#define HEADCYCLEFREEANSWERSET_H_

#include "AbstractAlgorithm.h"

#include <gmp.h>
#include <gmpxx.h>

class HeadCycleFreeAnswerSetConsistencySolutionContent : public SolutionContent
{
public:
	HeadCycleFreeAnswerSetConsistencySolutionContent();
	HeadCycleFreeAnswerSetConsistencySolutionContent(const std::set<Variable> &enumeration);
	virtual ~HeadCycleFreeAnswerSetConsistencySolutionContent();
	
public:
	bool consistent;
};

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

class HeadCycleFreeAnswerSetConsistencyInstantiator : public Instantiator
{
public:
	HeadCycleFreeAnswerSetConsistencyInstantiator();
	virtual ~HeadCycleFreeAnswerSetConsistencyInstantiator();

public:
	virtual Solution *createEmptySolution() const;
	virtual Solution *createLeafSolution(const std::set<Variable> &partition) const;
	virtual Solution *combine(Operation operation, Solution *left, Solution *right) const;
	virtual Solution *addDifference(Solution *child, int difference) const;
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
