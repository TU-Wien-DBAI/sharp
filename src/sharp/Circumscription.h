#ifndef CIRCUMSCRIPTION_H_
#define CIRCUMSCRIPTION_H_

#include "AbstractAlgorithm.h"

#include <gmp.h>
#include <gmpxx.h>

class CircumscriptionEnumSolutionContent : public SolutionContent
{
public:
	CircumscriptionEnumSolutionContent();
	CircumscriptionEnumSolutionContent(const std::set<Variable> &enumeration);
	virtual ~CircumscriptionEnumSolutionContent();
	
public:
	std::set<set<Variable> > enumerations;
};

class LazyCircumscriptionEnumSolution : public LazySolution
{
public:
	LazyCircumscriptionEnumSolution(Operation operation, 
		Solution *left, Solution *right);
	LazyCircumscriptionEnumSolution(Solution *child, int difference);
	virtual ~LazyCircumscriptionEnumSolution();

protected:
	virtual void calculateUnion();
	virtual void calculateCrossJoin();
	virtual void calculateAddDifference();
};

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

class CircumscriptionEnumInstantiator : public Instantiator
{
public:
	CircumscriptionEnumInstantiator();
	virtual ~CircumscriptionEnumInstantiator();

public:
	virtual Solution *createEmptySolution() const;
	virtual Solution *createLeafSolution(const std::set<Variable> &partition) const;
	virtual Solution *combine(Operation operation, Solution *left, Solution *right) const;
	virtual Solution *addDifference(Solution *child, int difference) const;
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
