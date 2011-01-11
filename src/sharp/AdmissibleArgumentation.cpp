/*
* Author: 		Clemens Nopp, Matr.Nr. 0826054
* Date: 		September 2010
* This File:	Algorithm for (Admissible) Argumentation Frameworks
*/ 

#include <iostream>
#include <algorithm>

#include "AdmissibleArgumentation.h"

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
		
		cout << "cred";
 
		cout << endl;
 
        for(TupleSet::iterator it = tuples->begin(); it != tuples->end(); ++it)
        {
			ColoringVector c = ((AdmissibleArgumentationTuple *)it->first)->colorings;
                    
            for(ColoringVector::iterator colIt = c.begin(); colIt != c.end(); ++colIt)
            {
				string col;
				
				if (*colIt == 0) col = "Att";
				else if (*colIt == 1) col = "Def";
				else if (*colIt == 2) col = "In";
				else if (*colIt == 3) col = "Out";
				
				cout << col << "\t";
			}      
			
			cout << ((AdmissibleArgumentationTuple *)it->first)->bCredulousAcc << "\t";

			SolutionContent *sc = (it->second)->getContent();
			EnumerationSolutionContent *sol = (EnumerationSolutionContent *)sc;

			//cout << "Solutions: " << sol->enumerations.size() << endl;
			cout << "{";
			string osep = "";
			for(set<VertexSet>::iterator oit = sol->enumerations.begin(); oit != sol->enumerations.end(); ++oit)
			{
				cout << osep << "{";
				string isep = "";
				for(VertexSet::iterator iit = oit->begin(); iit != oit->end(); ++iit)
				{
					cout << isep << problem->getVertexName(*iit);
					isep = ",";
				}
				cout << "}" << flush;
				osep = ",";
			}
			cout << "}" << endl;
        }
}
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AdmissibleArgumentationTuple::AdmissibleArgumentationTuple() 
{
	bCredulousAcc = false;
}

AdmissibleArgumentationTuple::~AdmissibleArgumentationTuple() { }

bool AdmissibleArgumentationTuple::operator<(const Tuple &other) const
{
	AdmissibleArgumentationTuple &o = (AdmissibleArgumentationTuple &)other;
	return this->colorings < o.colorings;
}

bool AdmissibleArgumentationTuple::operator==(const Tuple &other) const
{
	AdmissibleArgumentationTuple &o = (AdmissibleArgumentationTuple &)other;
	return this->colorings == o.colorings;
}

int AdmissibleArgumentationTuple::hash() const
{
	//TODO
	return -1;
}

AdmissibleArgumentationAlgorithm::AdmissibleArgumentationAlgorithm(Problem *problem, char *credulousAcc)
	: AbstractArgumentationAlgorithm(problem)
{
	this->credulousAcc = credulousAcc;
	this->intCredulousAcc = -1;

#if defined(VERBOSE) && defined(DEBUG)
	if (this->credulousAcc != NULL)
	{	
		cout << endl << "Algorithm will check credulous acceptance for '" << credulousAcc << "'." << endl; // (code: " << intCredulousAcc << ")." << endl;	
	}
#endif
	
}

AdmissibleArgumentationAlgorithm::~AdmissibleArgumentationAlgorithm() { }


Solution *AdmissibleArgumentationAlgorithm::selectSolution(TupleSet *tuples, const ExtendedHypertree *root)
{
	
	Solution *s = this->instantiator->createEmptySolution();
	bool credAcc = false;
	ColoringVector colorings;
	bool containsAtt = false;	

	for(TupleSet::iterator it = tuples->begin(); it != tuples->end(); ++it)
	{	
		colorings = ((AdmissibleArgumentationTuple *)it->first)->colorings;
		containsAtt = false;		
		
		for(unsigned int i=0; i < colorings.size(); i++)
		{
			if (colorings[i] == ATT) 
			{
				containsAtt = true;
				break;
			}
			
		}
		
		if (!containsAtt)
		{
			if(((AdmissibleArgumentationTuple *)it->first)->bCredulousAcc) credAcc = true;
			s = this->instantiator->combine(Union, s, it->second);
		}
	}

	if (credAcc)
	{
		cout << endl << "Credulous acceptance holds for the requested variable '" << credulousAcc << "'." << endl;
	} 	
	else if (credulousAcc != NULL && strlen(credulousAcc) > 0)
	{
		cout << endl << "Credulous acceptance does not hold for the requested variable '" << credulousAcc << "'." << endl;
	} 	

	cout << endl;

	return s;
}

TupleSet *AdmissibleArgumentationAlgorithm::evaluateLeafNode(const ExtendedHypertree *node)
{
#if defined(VERBOSE) && defined(DEBUG)
	cout << endl << "Entered evaluateLeafNode." << endl;
#endif

	TupleSet *ts = new TupleSet();
	const ArgumentSet arguments = (ArgumentSet) node->getVertices();

	//get integer value for credulous acceptance argument 
	if (intCredulousAcc == -1 && this->credulousAcc != NULL)
	{
		intCredulousAcc = problem->getVertexId(this->credulousAcc);
	}

	//calculate conflict free sets
	vector< ArgumentSet > cfSets = this->conflictFreeSets(&arguments);
	
	//go through conflict free sets and calculate colorings
	for(unsigned int i=0; i < (cfSets.size()); ++i)
	{
		AdmissibleArgumentationTuple &argTuple = *new AdmissibleArgumentationTuple();
	
		//calculate set with args attacked by cfSet
		ArgumentSet attByCF = this->attackedBySet(&cfSets[i], problem);

		//calculate coloring for every argument
		for(ArgumentSet::iterator it = arguments.begin();
			it != arguments.end();
			++it) 
		{
			
			//empty set => arg is out
			if (cfSets[i].size() == 0)
			{
				(argTuple.colorings).push_back(OUT);
				//cout << "Calculated OUT for tupel " << i << ", Argument " << *it << endl;
			}
			
			//cfSets contains arg => In
			else if ((cfSets[i]).count(*it) > 0)
			{
				//set credulous acceptance flag if current arg equals intCredulousAcc
				if(*it == intCredulousAcc) argTuple.bCredulousAcc = true;
				
				(argTuple.colorings).push_back(IN);
				//cout << "Calculated IN for tupel " << i << ", Argument " << *it << endl;
			} 
		
			//if list of args attacked by conflict free sets contains arg => Def
			else if (attByCF.count(*it) > 0)
			{
				(argTuple.colorings).push_back(DEF);
				//cout << "Calculated DEF for tupel " << i << ", Argument " << *it << endl;
			}

			//call attCheck to decide if IN-args are attacked by another arg
			else if (this->attCheck(&cfSets[i], (Argument) *it, problem)) 
			{
				(argTuple.colorings).push_back(ATT);
				//cout << "Calculated ATT for tupel " << i << ", Argument " << *it << endl;
			}

			//otherwise OUT
			else
			{
				(argTuple.colorings).push_back(OUT);
			}
		}		
		
		ts->insert(TupleSet::value_type(&argTuple, this->instantiator->createLeafSolution(cfSets[i])));
	}

#if defined(VERBOSE) && defined(DEBUG)
	printTuples(ts, node, problem);

	/*cout << "Conflict free sets:" << endl;
	for(vector<set<Argument> >::iterator it1 = cfSets.begin(); it1 != cfSets.end(); ++it1)
	{			
			for(set<Argument>::iterator it2 = it1->begin(); it2 != it1->end(); ++it2)
			{
				cout << *it2 << " ";
		    }
			cout << endl;
	}*/
#endif

	return ts;
}

TupleSet *AdmissibleArgumentationAlgorithm::evaluateBranchNode(const ExtendedHypertree *node)
{
#if defined(VERBOSE) && defined(DEBUG)
	cout << endl << "Entered evaluateBranchNode." << endl;
#endif

	TupleSet *left = this->evaluateNode(node->firstChild()), 
		*right = this->evaluateNode(node->secondChild());

	TupleSet *ts = new TupleSet();

	const ArgumentSet arguments = (ArgumentSet) node->getVertices();
	const ArgumentSet leftArguments = (ArgumentSet) (node->firstChild())->getVertices();
	const ArgumentSet rightArguments = (ArgumentSet) (node->secondChild())->getVertices();

	//iterate through left tuples
	for(TupleSet::iterator lit = left->begin(); lit != left->end(); ++lit)
	{	
		ColoringVector leftColoring = ((AdmissibleArgumentationTuple *)lit->first)->colorings;
		ArgumentSet leftIn = this->getInArgs(&leftArguments, &leftColoring);
		
		//iterate through right tuples and generate new tuplesets
		for(TupleSet::iterator rit = right->begin(); rit != right->end(); ++rit)
		{	
			ColoringVector rightColoring = ((AdmissibleArgumentationTuple *)rit->first)->colorings;
			ArgumentSet rightIn = this->getInArgs(&rightArguments, &rightColoring);
			
			//check if IN-sets are equal => generate new tuple
			if ((leftIn.size() == rightIn.size()) && (equal(leftIn.begin(), leftIn.end(), rightIn.begin())))
			{
				AdmissibleArgumentationTuple &argTuple = *new AdmissibleArgumentationTuple();
				
				//calculate bCredulousAcc
				argTuple.bCredulousAcc = ((AdmissibleArgumentationTuple *)lit->first)->bCredulousAcc ||
										 ((AdmissibleArgumentationTuple *)rit->first)->bCredulousAcc;
				
				//calculate colorings
				argTuple.colorings = getBranchColorings(leftColoring, rightColoring, arguments);
				
				//generate cross join solution
				Solution *s = this->instantiator->combine(CrossJoin, lit->second, rit->second);
				
				//add to tuple set with alternative operation 'union'
				addToTupleSet(&argTuple, s, ts, Union);
			}
		}
	}

	delete left;
	delete right;

#if defined(VERBOSE) && defined(DEBUG)
	printTuples(ts, node, problem);
#endif

	return ts;

}

TupleSet *AdmissibleArgumentationAlgorithm::evaluateIntroductionNode(const ExtendedHypertree *node)
{
#if defined(VERBOSE) && defined(DEBUG)
	cout << endl << "Entered evaluateIntroductionNode." << endl;
#endif
	
	TupleSet *base = this->evaluateNode(node->firstChild());
	TupleSet *ts = new TupleSet();
	
	const ArgumentSet arguments = (ArgumentSet) node->getVertices();
	const ArgumentSet childArguments = (ArgumentSet) (node->firstChild())->getVertices();

	//iterate through tuples and generate new tupleset
	for(TupleSet::iterator it = base->begin(); it != base->end(); ++it)
	{	
		AdmissibleArgumentationTuple &argTuple = *new AdmissibleArgumentationTuple();	
		ColoringVector childColoring = ((AdmissibleArgumentationTuple *)it->first)->colorings;
		bool addTuple = false;
		
		//take over bCredulousAcc
		argTuple.bCredulousAcc = ((AdmissibleArgumentationTuple *)it->first)->bCredulousAcc;

		ArgumentSet in = this->getInArgs(&childArguments, &childColoring);
				
		//calculate colorings
		argTuple.colorings = getIntroColorings(&in, &childColoring, node->getDifference(), &addTuple, arguments);
		
		if (addTuple)
		{	
			AdmissibleArgumentationTuple &additionalArgTuple = *new AdmissibleArgumentationTuple();	
			
			//take over bCredulousAcc
			additionalArgTuple.bCredulousAcc = ((AdmissibleArgumentationTuple *)it->first)->bCredulousAcc;
			
			int index = 0;
			for(set<Argument>::iterator it2 = arguments.begin(); it2 != arguments.end(); ++it2)
			{
				//current arg is the new one
				if (*it2 == node->getDifference())
				{
					(additionalArgTuple.colorings).push_back(IN);	
					
					//set credulous acceptance flag if current arg equals intCredulousAcc
					if(*it2 == intCredulousAcc) additionalArgTuple.bCredulousAcc = true;
				}
				//current arg is not the new one
				else
				{
					//IN if arg is the new one (see above) or if the corresponding child arg is IN
					if (childColoring[index] == IN)
					{
						(additionalArgTuple.colorings).push_back(IN);	
					
						//set credulous acceptance flag if current arg equals intCredulousAcc
						if(*it2 == intCredulousAcc) additionalArgTuple.bCredulousAcc = true;
					}
					
					//DEF if the new arg attacks the current arg or if the corresponding child arg is DEF 
					else if ((problem->getAttacksFromArg(node->getDifference()) != NULL &&
					          (problem->getAttacksFromArg(node->getDifference()))->count(*it2) > 0) ||
							  childColoring[index] == DEF)
					{
						(additionalArgTuple.colorings).push_back(DEF);	
					}
					
					//OUT if the corresponding child arg is OUT and there is no attack from 
					//the current arg to the new one or otherwise
					/*else if  (problem->getAttacksFromArg(node->getDifference()) != NULL &&
							 (problem->getAttacksFromArg(node->getDifference()))->count(*it2) == 0 &&
							  problem->getAttacksFromArg(*it2) != NULL &&
							 (problem->getAttacksFromArg(*it2))->count(node->getDifference()) == 0 &&	
							  childColoring[index] == OUT)*/
					else if  ((problem->getAttacksFromArg(node->getDifference()) == NULL ||
							 (problem->getAttacksFromArg(node->getDifference()))->count(*it2) == 0) &&
							  (problem->getAttacksFromArg(*it2) == NULL ||
							 (problem->getAttacksFromArg(*it2))->count(node->getDifference()) == 0) &&
							  childColoring[index] == OUT)
					{
						(additionalArgTuple.colorings).push_back(OUT);
					}
					
					//ATT otherwise
					else
					{
						(additionalArgTuple.colorings).push_back(ATT);	
					}
					
					index++;
				}
			}	
			addTuple = false;
			
			//generate 'add difference' solution
			Solution *s = this->instantiator->addDifference(it->second,	node->getDifference());
			
			//add new tuple to tuple set with alternative operation 'union'
			addToTupleSet(&additionalArgTuple, s, ts, Union);
		}	
		
		//add 'old' tuple to tuple set with alternative operation 'union'
		addToTupleSet(&argTuple, it->second, ts, Union);
	}	

	delete base;
	
#if defined(VERBOSE) && defined(DEBUG)
	printTuples(ts, node, problem);
#endif

	return ts;
}

TupleSet *AdmissibleArgumentationAlgorithm::evaluateRemovalNode(const ExtendedHypertree *node)
{
#if defined(VERBOSE) && defined(DEBUG)
	cout << endl << "Entered evaluateRemovalNode." << endl;
#endif

	TupleSet *base = this->evaluateNode(node->firstChild());
	TupleSet *ts = new TupleSet();
	
	const ArgumentSet arguments = (ArgumentSet) node->getVertices();
	const ArgumentSet childArguments = (ArgumentSet) (node->firstChild())->getVertices();
	
	//iterate through tuples and generate new tupleset
	for(TupleSet::iterator it = base->begin(); it != base->end(); ++it)
	{	
		AdmissibleArgumentationTuple &argTuple = *new AdmissibleArgumentationTuple();	
		bool insertFlag = true;
		int index = 0;
		ColoringVector childColoring = ((AdmissibleArgumentationTuple *)it->first)->colorings;
		
		//take over bCredulousAcc
		argTuple.bCredulousAcc = ((AdmissibleArgumentationTuple *)it->first)->bCredulousAcc;
		
		for(set<Argument>::iterator it2 = childArguments.begin(); it2 != childArguments.end(); ++it2)
		{
			//insert coloring if current argument is not the removed one
			if (*it2 != node->getDifference())
			{	
				(argTuple.colorings).push_back(childColoring[index]);	
			}
			
			//if current argument is the removed one and the previous coloring
			//was ATT => do not insert into new tupleset
			else if ((*it2 == node->getDifference()) && childColoring[index] == ATT)
			{
				insertFlag = false;
				break;
			}
			
			index++; 
		}
		
		if(insertFlag)
		{
			//add tuple to tuple set with alternative operation 'union'
			addToTupleSet(&argTuple, it->second, ts, Union);
		}	
	}
		
	delete base;

#if defined(VERBOSE) && defined(DEBUG)
	printTuples(ts, node, problem);
#endif

	return ts;
}

