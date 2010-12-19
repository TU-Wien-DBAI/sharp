/*
* Author: 		Clemens Nopp, Matr.Nr. 0826054
* Date: 		September 2010
* This File:	Algorithm for (Preferred) Argumentation Frameworks (Header)
*/ 

#ifndef PREFARGUMENTATIONPROGRAM_H_
#define PREFARGUMENTATIONPROGRAM_H_

#include "AbstractAlgorithm.h"
#include "../ArgumentationProblem.h"

class PreferredArgumentationTuple : public Tuple
{
public:
	PreferredArgumentationTuple();
	virtual ~PreferredArgumentationTuple();

	//used for the mapping Argument<>Coloring
	ColoringVector colorings;
		
	//true, if credulous acceptance for the given variable holds
	bool bScepticalAcc;
	
	//std::set<ColoringVector *> certificates;
	int coloringNbr;
	std::set<int> certificates;

	virtual bool operator<(const Tuple &other) const;
	virtual bool operator==(const Tuple &other) const;
	virtual int hash() const;
};

class PreferredArgumentationAlgorithm : public AbstractAlgorithm
{
public:
	PreferredArgumentationAlgorithm(Problem *problem, char *credulousAcc);
	virtual ~PreferredArgumentationAlgorithm();

protected:
	virtual Solution *selectSolution(TupleSet *tuples, const ExtendedHypertree *node);
	
	virtual TupleSet *evaluateLeafNode(const ExtendedHypertree *node);
	virtual TupleSet *evaluateBranchNode(const ExtendedHypertree *node);
	virtual TupleSet *evaluateIntroductionNode(const ExtendedHypertree *node);
	virtual TupleSet *evaluateRemovalNode(const ExtendedHypertree *node);

protected:
	ArgumentationProblem *problem;
	Argument intScepticalAcc;
	char *scepticalAcc;
	
private:
	//Calculates all conflict free sets
	std::vector< ArgumentSet > conflictFreeSets(const ArgumentSet *args);
	
	//Calculates a set of arguments attacked by the given args
	ArgumentSet attackedBySet(const ArgumentSet *args, ArgumentationProblem *problem);
	
	//Decides if the given arg has the coloring ATT
	bool attCheck(const ArgumentSet *args, Argument arg, ArgumentationProblem *problem);
	
	//Returns the set of arguments which are colored 'IN' in the ColoringVector
	ArgumentSet getInArgs(const ArgumentSet *args, ColoringVector *colorings);
	
	//Tries to insert a new element in the tupleset
	void addToTupleSet(Tuple *t, Solution *s, TupleSet *ts, Operation op);
	
	//Returns true, if the first argument is a subset of the second one (just args with IN)
	bool isSubset (ColoringVector v1, ColoringVector v2);
};

#endif
