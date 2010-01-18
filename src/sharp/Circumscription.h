#ifndef CIRCUMSCRIPTION_H_
#define CIRCUMSCRIPTION_H_

#include "AbstractAlgorithm.h"

class CircumscriptionTuple : public Tuple
{
public:
	CircumscriptionTuple();
	virtual ~CircumscriptionTuple();

public:
	set<Variable> variables;
	set<Rule> rules;
	set<Atom> guards;

public:
	virtual bool operator<(const Tuple &other) const;
	virtual bool operator==(const Tuple &other) const;
	virtual int hash() const;
};

class CircumscriptionAlgorithm : public AbstractAlgorithm
{
public:
	CircumscriptionAlgorithm(const Instantiator *instantiator, const ExtendedHypertree *root, const SignMap &signMap, const HeadMap &headMap, const NameMap &nameMap);
	virtual ~CircumscriptionAlgorithm();

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
