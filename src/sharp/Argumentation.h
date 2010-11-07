/*
* Author: 		Clemens Nopp, Matr.Nr. 0826054
* Date: 		September 2010
* This File:	Algorithm for Argumentation Frameworks (Header)
*/ 

#ifndef ARGUMENTATIONPROGRAM_H_
#define ARGUMENTATIONPROGRAM_H_

#include "AbstractAlgorithm.h"
#include "../ArgumentationProblem.h"

//colorings for the arguments
enum Coloring {ATT, DEF, IN, OUT};

//typedef std::set<std::pair<Argument, Coloring> > ColoringSet;
typedef std::set<Coloring> ColoringSet;

class ArgumentationTuple : public Tuple
{
public:
	ArgumentationTuple();
	virtual ~ArgumentationTuple();

	//used for the mapping Argument<>Coloring
	ColoringSet colorings;
	
	//cardinality of the set (# of restricted admissible sets characterized by the colorings)
	int cardinality;
	
	//true, if credulous acceptance for the given variable holds
	bool bCredulousAcc;

	virtual bool operator==(const Tuple &other) const;
	virtual int hash() const;
};

class ArgumentationAlgorithm : public AbstractAlgorithm
{
public:
	ArgumentationAlgorithm(Problem *problem, char *credulousAcc);
	virtual ~ArgumentationAlgorithm();

protected:
	virtual Solution *selectSolution(TupleSet *tuples, const ExtendedHypertree *node);
	
	virtual TupleSet *evaluateLeafNode(const ExtendedHypertree *node);
	virtual TupleSet *evaluateBranchNode(const ExtendedHypertree *node);
	virtual TupleSet *evaluateIntroductionNode(const ExtendedHypertree *node);
	virtual TupleSet *evaluateRemovalNode(const ExtendedHypertree *node);

protected:
	ArgumentationProblem *problem;
	char *credulousAcc;
};

#endif
