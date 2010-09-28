/*
* Author: 		Clemens Nopp, Matr.Nr. 0826054
* Date: 		September 2010
* This File:	Algorithm for Argumentation Frameworks
*/ 

#include <iostream>

#include "Argumentation.h"

using namespace std;

#if defined(VERBOSE) && defined(DEBUG)
static void printTuples(TupleSet *tuples, const ExtendedHypertree *node)
{
       
}
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ArgumentationTuple::ArgumentationTuple() { }

ArgumentationTuple::~ArgumentationTuple() { }

ArgumentationAlgorithm::ArgumentationAlgorithm(Problem *problem)
	: AbstractAlgorithm(problem)
{
	this->problem = (ArgumentationProblem *)problem;
}

ArgumentationAlgorithm::~ArgumentationAlgorithm() { }


Solution *ArgumentationAlgorithm::selectSolution(TupleSet *tuples, const ExtendedHypertree *root)
{
	//just a dummy
	return this->instantiator->createEmptySolution();
}

TupleSet *ArgumentationAlgorithm::evaluateLeafNode(const ExtendedHypertree *node)
{
	//just a dummy
	return new TupleSet();

#if defined(VERBOSE) && defined(DEBUG)
	printTuples(ts, node);
#endif

}

TupleSet *ArgumentationAlgorithm::evaluateBranchNode(const ExtendedHypertree *node)
{
	//just a dummy
	return new TupleSet();

#if defined(VERBOSE) && defined(DEBUG)
	printTuples(ts, node);
#endif

}

TupleSet *ArgumentationAlgorithm::evaluateIntroductionNode(const ExtendedHypertree *node)
{
	//just a dummy
	return new TupleSet();

#if defined(VERBOSE) && defined(DEBUG)
	printTuples(ts, node);
#endif

}

TupleSet *ArgumentationAlgorithm::evaluateRemovalNode(const ExtendedHypertree *node)
{
	//just a dummy
	return new TupleSet();

#if defined(VERBOSE) && defined(DEBUG)
	printTuples(ts, node);
#endif

}

