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
typedef std::vector<Coloring> ColoringVector;

class ArgumentationTuple : public Tuple
{
public:
	ArgumentationTuple();
	virtual ~ArgumentationTuple();

	//used for the mapping Argument<>Coloring
	ColoringVector colorings;
	
	//cardinality of the set (# of restricted admissible sets characterized by the colorings)
	int cardinality;
	
	//true, if credulous acceptance for the given variable holds
	bool bCredulousAcc;

	virtual bool operator<(const Tuple &other) const;
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
	Argument intCredulousAcc;
	char *credulousAcc;
	
private:
	//Calculates all conflict free sets
	std::vector< ArgumentSet > conflictFreeSets(const ArgumentSet *args);
	
	//Calculates a set of arguments attacked by the given args
	ArgumentSet attackedBySet(const ArgumentSet *args, ArgumentationProblem *problem);
	
	//Decides if the given arg has the coloring ATT
	bool attCheck(const ArgumentSet *args, Argument arg, ArgumentationProblem *problem);

};

#endif
