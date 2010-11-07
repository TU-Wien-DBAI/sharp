/*
* Author: 		Clemens Nopp, Matr.Nr. 0826054
* Date: 		September 2010
* This File:	Algorithm for Argumentation Frameworks
*/ 

#include <iostream>

#include "Argumentation.h"

using namespace std;

#if defined(VERBOSE) && defined(DEBUG)
static void printTuples(TupleSet *tuples, const ExtendedHypertree *node, ArgumentationProblem *problem)
{
        cout << "Colorings for node " << node << ":" << endl;
		for(VertexSet::iterator it = node->getVertices().begin();
			it != node->getVertices().end();
			++it) 
		{
			cout << problem->getArgumentString((Argument)*it) << "\t";
		}
 
		cout << endl;
 
        for(TupleSet::iterator it = tuples->begin(); it != tuples->end(); ++it)
        {
			ColoringSet c = ((ArgumentationTuple *)it->first)->colorings;
                    
            for(ColoringSet::iterator colIt = c.begin(); colIt != c.end(); ++colIt)
            {
				cout << *colIt << "\t";
			}      
			      
            cout << endl;
        }
        
		cout << endl;
}
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ArgumentationTuple::ArgumentationTuple() 
{
	bCredulousAcc = false;
	cardinality = 1; 
}

ArgumentationTuple::~ArgumentationTuple() { }

bool ArgumentationTuple::operator==(const Tuple &other) const
{
	equal_to<ColoringSet> colEqual;
	ArgumentationTuple &o = (ArgumentationTuple &)other;

	return colEqual(this->colorings, o.colorings);
}

int ArgumentationTuple::hash() const
{
	//TODO
	return -1;
}

ArgumentationAlgorithm::ArgumentationAlgorithm(Problem *problem, char *credulousAcc)
	: AbstractAlgorithm(problem)
{
	this->problem = (ArgumentationProblem *)problem;
	this->credulousAcc = credulousAcc;
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
	TupleSet *ts = new TupleSet();
	return ts;

#if defined(VERBOSE) && defined(DEBUG)
	printTuples(ts, node, problem);
#endif

}

TupleSet *ArgumentationAlgorithm::evaluateBranchNode(const ExtendedHypertree *node)
{
	//just a dummy
	TupleSet *ts = new TupleSet();
	return ts;

#if defined(VERBOSE) && defined(DEBUG)
	printTuples(ts, node, problem);
#endif

}

TupleSet *ArgumentationAlgorithm::evaluateIntroductionNode(const ExtendedHypertree *node)
{
	//just a dummy
	TupleSet *ts = new TupleSet();
	return ts;

#if defined(VERBOSE) && defined(DEBUG)
	printTuples(ts, node, problem);
#endif

}

TupleSet *ArgumentationAlgorithm::evaluateRemovalNode(const ExtendedHypertree *node)
{
	//just a dummy
	TupleSet *ts = new TupleSet();
	return ts;

#if defined(VERBOSE) && defined(DEBUG)
	printTuples(ts, node, problem);
#endif

}

