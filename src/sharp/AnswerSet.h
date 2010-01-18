#ifndef ANSWERSETPROGRAM_H_
#define ANSWERSETPROGRAM_H_

#include "AbstractAlgorithm.h"

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
