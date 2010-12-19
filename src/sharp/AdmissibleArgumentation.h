/*
* Author: 		Clemens Nopp, Matr.Nr. 0826054
* Date: 		September 2010
* This File:	Algorithm for (Admissible) Argumentation Frameworks (Header)
*/ 

#ifndef ADMARGUMENTATIONPROGRAM_H_
#define ADMARGUMENTATIONPROGRAM_H_

#include "AbstractAlgorithm.h"
#include "../ArgumentationProblem.h"

class AdmissibleArgumentationTuple : public Tuple
{
public:
	AdmissibleArgumentationTuple();
	virtual ~AdmissibleArgumentationTuple();

	//used for the mapping Argument<>Coloring
	ColoringVector colorings;
		
	//true, if credulous acceptance for the given variable holds
	bool bCredulousAcc;

	virtual bool operator<(const Tuple &other) const;
	virtual bool operator==(const Tuple &other) const;
	virtual int hash() const;
};

class AdmissibleArgumentationAlgorithm : public AbstractAlgorithm
{
public:
	AdmissibleArgumentationAlgorithm(Problem *problem, char *credulousAcc);
	virtual ~AdmissibleArgumentationAlgorithm();

protected:
	virtual Solution *selectSolution(TupleSet *tuples, const ExtendedHypertree *node);
	
	virtual TupleSet *evaluateLeafNode(const ExtendedHypertree *node);
	virtual TupleSet *evaluateBranchNode(const ExtendedHypertree *node);
	virtual TupleSet *evaluateIntroductionNode(const ExtendedHypertree *node);
	virtual TupleSet *evaluateRemovalNode(const ExtendedHypertree *node);

protected:
	ArgumentationProblem *problem;
	Argument intCredulousAcc;
	char *credulousAcc;
	
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
};

#endif
