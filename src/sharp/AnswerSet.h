#ifndef ANSWERSETPROGRAM_H_
#define ANSWERSETPROGRAM_H_

#include "AbstractAlgorithm.h"

#include <gmp.h>
#include <gmpxx.h>

class AnswerSetEnumSolutionContent : public SolutionContent
{
public:
	AnswerSetEnumSolutionContent();
	AnswerSetEnumSolutionContent(const std::set<Variable> &enumeration);
	virtual ~AnswerSetEnumSolutionContent();
	
public:
	std::set<set<Variable> > enumerations;
};

class LazyAnswerSetEnumSolution : public LazySolution
{
public:
	LazyAnswerSetEnumSolution(Operation operation, 
		Solution *left, Solution *right);
	LazyAnswerSetEnumSolution(Solution *child, int difference);
	virtual ~LazyAnswerSetEnumSolution();

protected:
	virtual void calculateUnion();
	virtual void calculateCrossJoin();
	virtual void calculateAddDifference();
};

class AnswerSetTuple : public Tuple
{
public:
	AnswerSetTuple();
	virtual ~AnswerSetTuple();

public:
	set<Variable> variables;
	set<Rule> rules;
	set<Atom> guards;

public:
	virtual bool operator<(const Tuple &other) const;
	virtual bool operator==(const Tuple &other) const;
	virtual int hash() const;
};

class AnswerSetEnumInstantiator : public Instantiator
{
public:
	AnswerSetEnumInstantiator();
	virtual ~AnswerSetEnumInstantiator();

public:
	virtual Solution *createEmptySolution() const;
	virtual Solution *createLeafSolution(const std::set<Variable> &partition) const;
	virtual Solution *combine(Operation operation, Solution *left, Solution *right) const;
	virtual Solution *addDifference(Solution *child, int difference) const;
};

class AnswerSetAlgorithm : public AbstractAlgorithm
{
public:
	AnswerSetAlgorithm(const Instantiator *instantiator, const ExtendedHypertree *root, const SignMap &signMap, const HeadMap &headMap, const NameMap &nameMap);
	virtual ~AnswerSetAlgorithm();

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
