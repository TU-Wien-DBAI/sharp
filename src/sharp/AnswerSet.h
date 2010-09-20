#ifndef ANSWERSETPROGRAM_H_
#define ANSWERSETPROGRAM_H_

#include "AbstractAlgorithm.h"
#include "../DatalogProblem.h"

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

class AnswerSetAlgorithm : public AbstractAlgorithm
{
public:
	AnswerSetAlgorithm(Problem *problem);
	virtual ~AnswerSetAlgorithm();

	VariableSet getVariables(const ExtendedHypertree *node);
	RuleSet getRules(const ExtendedHypertree *node);

protected:
	virtual Solution *selectSolution(TupleSet *tuples, const ExtendedHypertree *node);
	
	virtual TupleSet *evaluateLeafNode(const ExtendedHypertree *node);
	virtual TupleSet *evaluateBranchNode(const ExtendedHypertree *node);
	virtual TupleSet *evaluateIntroductionNode(const ExtendedHypertree *node);
	virtual TupleSet *evaluateRemovalNode(const ExtendedHypertree *node);

	virtual TupleSet *evaluateVariableIntroductionNode(const ExtendedHypertree *node);
	virtual TupleSet *evaluateVariableRemovalNode(const ExtendedHypertree *node);
	virtual TupleSet *evaluateRuleIntroductionNode(const ExtendedHypertree *node);
	virtual TupleSet *evaluateRuleRemovalNode(const ExtendedHypertree *node);

protected:
	DatalogProblem *problem;
};

#endif
