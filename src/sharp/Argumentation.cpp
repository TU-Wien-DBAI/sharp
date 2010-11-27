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
        cout << endl << "Colorings for node " << node << ":" << endl;
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
	
#if defined(VERBOSE) && defined(DEBUG)
	if (credulousAcc != NULL)
	{
		cout << endl << "Algorithm will check credulous acceptance for '" << credulousAcc << "'." << endl;
	}
#endif	
}

ArgumentationAlgorithm::~ArgumentationAlgorithm() { }


Solution *ArgumentationAlgorithm::selectSolution(TupleSet *tuples, const ExtendedHypertree *root)
{
	Solution *s = this->instantiator->createEmptySolution();

	for(TupleSet::iterator it = tuples->begin(); it != tuples->end(); ++it)
	{
		s = this->instantiator->combine(Union, s, it->second);
	}

	return s;
}

TupleSet *ArgumentationAlgorithm::evaluateLeafNode(const ExtendedHypertree *node)
{
#if defined(VERBOSE) && defined(DEBUG)
	cout << endl << "Entered evaluateLeafNode." << endl;
#endif

	TupleSet *ts = new TupleSet();

	



#if defined(VERBOSE) && defined(DEBUG)
	printTuples(ts, node, problem);
#endif

	return ts;
}

TupleSet *ArgumentationAlgorithm::evaluateBranchNode(const ExtendedHypertree *node)
{
#if defined(VERBOSE) && defined(DEBUG)
	cout << endl << "Entered evaluateBranchNode." << endl;
#endif

	TupleSet *left = this->evaluateNode(node->firstChild()), 
		*right = this->evaluateNode(node->secondChild());

	TupleSet *ts = new TupleSet();

#if defined(VERBOSE) && defined(DEBUG)
	printTuples(ts, node, problem);
#endif

	return ts;

}

TupleSet *ArgumentationAlgorithm::evaluateIntroductionNode(const ExtendedHypertree *node)
{
#if defined(VERBOSE) && defined(DEBUG)
	cout << endl << "Entered evaluateIntroductionNode." << endl;
#endif
	
	TupleSet *base = this->evaluateNode(node->firstChild());
	TupleSet *ts = new TupleSet();

#if defined(VERBOSE) && defined(DEBUG)
	printTuples(ts, node, problem);
#endif

	return ts;
}

TupleSet *ArgumentationAlgorithm::evaluateRemovalNode(const ExtendedHypertree *node)
{
#if defined(VERBOSE) && defined(DEBUG)
	cout << endl << "Entered evaluateRemovalNode." << endl;
#endif

	TupleSet *base = this->evaluateNode(node->firstChild());
	TupleSet *ts = new TupleSet();

#if defined(VERBOSE) && defined(DEBUG)
	printTuples(ts, node, problem);
#endif

	return ts;
}

set< set<Argument> > ArgumentationAlgorithm::conflictFreeSets(set<Argument> *args)
{
}
