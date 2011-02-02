/*
* Author: 		Clemens Nopp, Matr.Nr. 0826054
* Date: 		September 2010
* This File:	Algorithm for (Admissible) Argumentation Frameworks (Header)
*/ 

#ifndef ADMARGUMENTATIONPROGRAM_H_
#define ADMARGUMENTATIONPROGRAM_H_

#include "AbstractArgumentation.h"

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

class AdmissibleArgumentationAlgorithm : public AbstractArgumentationAlgorithm
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
	Argument intCredulousAcc;
	char *credulousAcc;

private:
	//Tries to insert a new element in the tupleset
	void addToTupleSet(AdmissibleArgumentationTuple *t, Solution *s, TupleSet *ts, Operation op);
};

#endif
