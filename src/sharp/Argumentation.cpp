/*
* Author: 		Clemens Nopp, Matr.Nr. 0826054
* Date: 		September 2010
* This File:	Algorithm for Argumentation Frameworks
*/ 

#include <iostream>
#include <algorithm>

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
		
		cout << "#\tcred";
 
		cout << endl;
 
        for(TupleSet::iterator it = tuples->begin(); it != tuples->end(); ++it)
        {
			ColoringVector c = ((ArgumentationTuple *)it->first)->colorings;
                    
            for(ColoringVector::iterator colIt = c.begin(); colIt != c.end(); ++colIt)
            {
				string col;
				
				if (*colIt == 0) col = "Att";
				else if (*colIt == 1) col = "Def";
				else if (*colIt == 2) col = "In";
				else if (*colIt == 3) col = "Out";
				
				cout << col << "\t";
			}      
			
			cout << ((ArgumentationTuple *)it->first)->cardinality << "\t";
			cout << ((ArgumentationTuple *)it->first)->bCredulousAcc << endl;			
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

bool ArgumentationTuple::operator<(const Tuple &other) const
{
	ArgumentationTuple &o = (ArgumentationTuple &)other;
	return this->colorings < o.colorings;
}

bool ArgumentationTuple::operator==(const Tuple &other) const
{
	bool eq = true;
	ArgumentationTuple &o = (ArgumentationTuple &)other;
	
	for (unsigned int i = 0; i < (this->colorings).size(); ++i)
	{
		if ((this->colorings)[i] != (o.colorings)[i]) eq = false;
	}

	return eq;
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

	if (credulousAcc != NULL)
	{	
#if defined(VERBOSE) && defined(DEBUG)
		cout << endl << "Algorithm will check credulous acceptance for '" << credulousAcc << "' (code: " << intCredulousAcc << ")." << endl;
#endif	
	}
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
	const ArgumentSet arguments = (ArgumentSet) node->getVertices();

	//get integer value for credulous acceptance argument 
	intCredulousAcc = problem->getVertexId(credulousAcc);

	//calculate conflict free sets
	vector< set<Argument> > cfSets = ArgumentationAlgorithm::conflictFreeSets(&arguments);
	
	//go through conflict free sets and calculate colorings
	for(unsigned int i=0; i < (cfSets.size()); ++i)
	{
		ArgumentationTuple &argTuple = *new ArgumentationTuple();
	
		//calculate set with args attacked by cfSet
		set<Argument> attByCF = ArgumentationAlgorithm::attackedBySet(&cfSets[i], problem);

		//calculate coloring for every argument
		for(VertexSet::iterator it = arguments.begin();
			it != arguments.end();
			++it) 
		{
			
			//empty set => arg is out
			if (cfSets[i].size() == 0)
			{
				(argTuple.colorings).push_back(OUT);
				cout << "Calculated OUT for tupel " << i << ", Argument " << *it << endl;
			}
			
			//cfSets contains arg => In
			else if ((cfSets[i]).count(*it) > 0)
			{
				//set credulous acceptance flag if current arg equals intCredulousAcc
				if(*it == intCredulousAcc) argTuple.bCredulousAcc = true;
				
				(argTuple.colorings).push_back(IN);
				cout << "Calculated IN for tupel " << i << ", Argument " << *it << endl;
			} 
		
			//call attCheck to decide if IN-args are attacked by another arg
			else if (ArgumentationAlgorithm::attCheck(&cfSets[i], (Argument) *it, problem)) 
			{
				(argTuple.colorings).push_back(ATT);
				cout << "Calculated ATT for tupel " << i << ", Argument " << *it << endl;
			}
		
			//if list of args attacked by conflict free sets contains arg => Def
			else if (attByCF.count(*it) > 0)
			{
				(argTuple.colorings).push_back(DEF);
				cout << "Calculated DEF for tupel " << i << ", Argument " << *it << endl;
			}
		
			
		}		
		
		ts->insert(TupleSet::value_type(&argTuple, this->instantiator->createLeafSolution(cfSets[i])));
	}

/*
#if defined(VERBOSE) && defined(DEBUG)
	printTuples(ts, node, problem);

	cout << "Conflict free sets:" << endl;
	for(vector<set<Argument> >::iterator it1 = cfSets.begin(); it1 != cfSets.end(); ++it1)
	{			
			for(set<Argument>::iterator it2 = it1->begin(); it2 != it1->end(); ++it2)
			{
				cout << *it2 << " ";
		    }
			cout << endl;
	}
#endif
*/


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

	delete left;
	delete right;

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
	set<ColoringVector> allColorings;
	
	const ArgumentSet arguments = (ArgumentSet) node->getVertices();
	const ArgumentSet childArguments = (ArgumentSet) (node->firstChild())->getVertices();

	//iterate through tuples and generate new tupleset
	for(TupleSet::iterator it = base->begin(); it != base->end(); ++it)
	{	
		ArgumentationTuple &argTuple = *new ArgumentationTuple();	
		ColoringVector childColoring = ((ArgumentationTuple *)it->first)->colorings;
		bool addTuple = false;
		
		//take over bCredulousAcc and cardinality
		argTuple.bCredulousAcc = ((ArgumentationTuple *)it->first)->bCredulousAcc;
		argTuple.cardinality = ((ArgumentationTuple *)it->first)->cardinality;
		
		//get set with IN arguments
		ArgumentSet in;		
		int index = 0;
		
		for(set<Argument>::iterator it2 = childArguments.begin(); it2 != childArguments.end(); ++it2)
		{
			if (childColoring[index] == IN)
			{	
				in.insert(*it2);	
			}			
			index++; 
		}
		
		//calculate colorings
		index = 0;
		for(set<Argument>::iterator it2 = arguments.begin(); it2 != arguments.end(); ++it2)
		{
			//current arg is not the new one
			if (*it2 != node->getDifference())
			{
				(argTuple.colorings).push_back(childColoring[index]);	
				index++;
			}
			//current arg is the new one
			else
			{
				//calculate coloring of new arg
				set<Argument> attByIn = ArgumentationAlgorithm::attackedBySet(&in, problem);
				
				if (attByIn.count((Argument)*it2) > 0)
				{
					(argTuple.colorings).push_back(DEF);
					//cout << "Calculated DEF for tupel " << i << ", Argument " << *it2 << endl;
				}
				
				//call attCheck to decide if IN-args are attacked by current argument
				else if (ArgumentationAlgorithm::attCheck(&in, (Argument) *it2, problem)) 
				{
					(argTuple.colorings).push_back(ATT);
					//cout << "Calculated ATT for tupel " << i << ", Argument " << *it2 << endl;
				}
				
				//otherwise OUT
				else
				{
					(argTuple.colorings).push_back(OUT);
					//cout << "Calculated OUT for tupel " << i << ", Argument " << *it2 << endl;
					
					//add tuple if arg does not attack itself
					if (ArgumentationAlgorithm::attCheck(problem->getAttacksFromArg(*it2), (Argument) *it2, problem))
					{
						addTuple = true;
					}	
				}
			}
		}
		
		if (addTuple)
		{
			ArgumentationTuple &additionalArgTuple = *new ArgumentationTuple();	
			
			//take over bCredulousAcc and cardinality
			additionalArgTuple.bCredulousAcc = ((ArgumentationTuple *)it->first)->bCredulousAcc;
			additionalArgTuple.cardinality = ((ArgumentationTuple *)it->first)->cardinality;

			
			index = 0;
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
					else if ((problem->getAttacksFromArg(node->getDifference()))->count(*it2) > 0 ||
							  childColoring[index] == DEF)
					{
						(additionalArgTuple.colorings).push_back(DEF);	
					}
					
					//OUT if the corresponding child arg is OUT and there is no attack from 
					//the current arg to the new one or otherwise
					else if ((problem->getAttacksFromArg(node->getDifference()))->count(*it2) == 0 &&
							 (problem->getAttacksFromArg(*it2))->count(node->getDifference()) == 0 &&	
							  childColoring[index] == OUT)
					{
						(additionalArgTuple.colorings).push_back(DEF);	
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
			
			if (allColorings.count(additionalArgTuple.colorings) == 0)
			{
				//TODO solution
				ts->insert(TupleSet::value_type(&additionalArgTuple, this->instantiator->createEmptySolution()));
				allColorings.insert(additionalArgTuple.colorings);		
			}
			else
			{
				//search for ColoringVector in ts
				for(std::map<Tuple *, Solution *, less<Tuple *> >::iterator it2 = ts->begin(); it2 != ts->end(); ++it2)
				{
					if (additionalArgTuple.colorings == ((ArgumentationTuple *)it2->first)->colorings)
					{
						((ArgumentationTuple *)it2->first)->cardinality = ((ArgumentationTuple *)it2->first)->cardinality + 1;
					}
				}
			}
		}	

		if (allColorings.count(argTuple.colorings) == 0)
		{
			//TODO solution
			ts->insert(TupleSet::value_type(&argTuple, this->instantiator->createEmptySolution()));
			allColorings.insert(argTuple.colorings);		
		}
		else
		{
			//search for ColoringVector in ts
			for(std::map<Tuple *, Solution *, less<Tuple *> >::iterator it2 = ts->begin(); it2 != ts->end(); ++it2)
			{
				if (argTuple.colorings == ((ArgumentationTuple *)it2->first)->colorings)
				{
					((ArgumentationTuple *)it2->first)->cardinality = ((ArgumentationTuple *)it2->first)->cardinality + 1;
				}
			}
		}
	}	

	delete base;
	
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
	set<ColoringVector> allColorings;
	
	const ArgumentSet arguments = (ArgumentSet) node->getVertices();
	const ArgumentSet childArguments = (ArgumentSet) (node->firstChild())->getVertices();
	
	//iterate through tuples and generate new tupleset
	for(TupleSet::iterator it = base->begin(); it != base->end(); ++it)
	{	
		ArgumentationTuple &argTuple = *new ArgumentationTuple();	
		bool insertFlag = true;
		int index = 0;
		ColoringVector childColoring = ((ArgumentationTuple *)it->first)->colorings;
		
		//take over bCredulousAcc and cardinality
		argTuple.bCredulousAcc = ((ArgumentationTuple *)it->first)->bCredulousAcc;
		argTuple.cardinality = ((ArgumentationTuple *)it->first)->cardinality;
		
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
			if (allColorings.count(argTuple.colorings) == 0)
			{
				//TODO solution
				ts->insert(TupleSet::value_type(&argTuple, this->instantiator->createEmptySolution()));
				allColorings.insert(argTuple.colorings);
			}
			else
			{
				//search for ColoringVector in ts
				for(std::map<Tuple *, Solution *, less<Tuple *> >::iterator it2 = ts->begin(); it2 != ts->end(); ++it2)
				{
					if (argTuple.colorings == ((ArgumentationTuple *)it2->first)->colorings)
					{
						((ArgumentationTuple *)it2->first)->cardinality = ((ArgumentationTuple *)it2->first)->cardinality + 1;
					}
				}
			}
		}	
	}
		
	delete base;

#if defined(VERBOSE) && defined(DEBUG)
	printTuples(ts, node, problem);
#endif

	return ts;
}

/*
***Description***
Calculates all conflict free sets

INPUT:	a set of arguments
OUTPUT:	a vector with sets of conflict free argument combinations
*/
vector< ArgumentSet > ArgumentationAlgorithm::conflictFreeSets(const ArgumentSet *args)
{
	vector< ArgumentSet > result;

	// 1 << args.size is the amount of power sets
	for(int i=0; i < (1 << (*args).size()); ++i)
	{
		//cout << "Entered i = " << i << endl;
		
		ArgumentSet tmpTuple;
		int j = 0;
		
		//add args to temporary tuple
		for(set<Argument>::iterator it = args->begin();	it != args->end(); ++it)
		{
			//true if j is '1' at this position
			if((1<<j)&i)
			{ 
				tmpTuple.insert(*it);
				//cout << "Element " << *it << " inserted in tmpTuple" << endl;
			}
			
			j++;
		}
		
		//cout << "Now check if tmpTuple is conflict free" << endl;
		
		//check if new element of powerset is conflict free
		bool conflictFree = true;
		
		for(set<Argument>::iterator it = tmpTuple.begin(); it != tmpTuple.end(); ++it)
		{
			ArgumentSet *attackedElements = problem->getAttacksFromArg((Argument)*it); 		

			//cout << "Got attacked elements" << endl;

			//calculate intersection if current argument attacks elements
			if (attackedElements != NULL)
			{
				vector<int> v(attackedElements->size());
				vector<int>::iterator vit;
				
				vit = set_intersection (tmpTuple.begin(), tmpTuple.end(), attackedElements->begin(), attackedElements->end(), v.begin());

				//check if intersection contains elements
				//if so, this set is not conflict free
				if (int(vit - v.begin()) > 0) conflictFree = false;
			}
		}	
		
		if(conflictFree) 
		{
			result.push_back(tmpTuple);
			//cout << "Set is conflict free, added tmpTuple to result." << endl;
		}
	}
	
	return result;
}

/*
***Description***
Calculates a set of arguments attacked by the given args

INPUT:	a set of arguments
OUTPUT:	a set with all args attacked by the given args
*/
ArgumentSet ArgumentationAlgorithm::attackedBySet(const ArgumentSet *args, ArgumentationProblem *problem)
{
	ArgumentSet result = *new ArgumentSet();

	for(set<Argument>::iterator it = args->begin(); it != args->end(); ++it)
	{
		ArgumentSet *attackedElements = problem->getAttacksFromArg(*it); 		

		if (attackedElements != NULL)
		{
			for(set<Argument>::iterator it2 = attackedElements->begin(); it2 != attackedElements->end(); ++it2)
			{
				result.insert(*it2);
			}
		}
	}
	
	return result;
}

/*
***Description***
Decides if the given arg has the coloring ATT

INPUT:	a set of arguments (usually the IN-args) and an single argument
OUTPUT:	true, if the given arg attacks the given set
*/
bool ArgumentationAlgorithm::attCheck(const ArgumentSet *args, Argument arg, ArgumentationProblem *problem)
{
	ArgumentSet *attackedElements = problem->getAttacksFromArg(arg); 		

	//calculate intersection if current argument attacks elements
	if (attackedElements != NULL)
	{
		vector<int> v(attackedElements->size());
		vector<int>::iterator vit;
		
		vit = set_intersection (args->begin(), args->end(), attackedElements->begin(), attackedElements->end(), v.begin());

		//check if intersection contains elements
		//if so => given set is attacked by the given arg
		if (int(vit - v.begin()) > 0) return true;
	}
	
	return false;
}

