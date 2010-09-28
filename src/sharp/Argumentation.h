/*
* Author: 		Clemens Nopp, Matr.Nr. 0826054
* Date: 		September 2010
* This File:	Algorithm for Argumentation Frameworks (Header)
*/ 

#ifndef ARGUMENTATIONPROGRAM_H_
#define ARGUMENTATIONPROGRAM_H_

#include "AbstractAlgorithm.h"
#include "../ArgumentationProblem.h"

class ArgumentationTuple : public Tuple
{
public:
	ArgumentationTuple();
	virtual ~ArgumentationTuple();

};

class ArgumentationAlgorithm : public AbstractAlgorithm
{
public:
	ArgumentationAlgorithm(Problem *problem);
	virtual ~ArgumentationAlgorithm();


protected:
	virtual Solution *selectSolution(TupleSet *tuples, const ExtendedHypertree *node);
	
	virtual TupleSet *evaluateLeafNode(const ExtendedHypertree *node);
	virtual TupleSet *evaluateBranchNode(const ExtendedHypertree *node);
	virtual TupleSet *evaluateIntroductionNode(const ExtendedHypertree *node);
	virtual TupleSet *evaluateRemovalNode(const ExtendedHypertree *node);

protected:
	ArgumentationProblem *problem;
};

#endif
