/*
* Author: 		Clemens Nopp, Matr.Nr. 0826054
* Date: 		September 2010
* This File:	General functions for Argumentation Framework Algorithms
*/

#include <iostream>
#include <algorithm>

#include "AbstractArgumentation.h"

using namespace std;

AbstractArgumentationAlgorithm::AbstractArgumentationAlgorithm(Problem *problem)
	: AbstractAlgorithm(problem)
{
	this->problem = (ArgumentationProblem *)problem;
}

AbstractArgumentationAlgorithm::~AbstractArgumentationAlgorithm() { }

/*
***Description***
Calculates all conflict free sets

INPUT:	a set of arguments
OUTPUT:	a vector with sets of conflict free argument combinations
*/
vector< ArgumentSet > AbstractArgumentationAlgorithm::conflictFreeSets(const ArgumentSet *args)
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
ArgumentSet AbstractArgumentationAlgorithm::attackedBySet(const ArgumentSet *args, ArgumentationProblem *problem)
{
	ArgumentSet result; // = *new ArgumentSet();

	for(ArgumentSet::iterator it = args->begin(); it != args->end(); ++it)
	{
		ArgumentSet *attackedElements = problem->getAttacksFromArg(*it);

		if (attackedElements != NULL)
		{
			for(ArgumentSet::iterator it2 = attackedElements->begin(); it2 != attackedElements->end(); ++it2)
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
bool AbstractArgumentationAlgorithm::attCheck(const ArgumentSet *args, Argument arg, ArgumentationProblem *problem)
{
	ArgumentSet *attackedElements = problem->getAttacksFromArg(arg);

	//calculate intersection if current argument attacks elements
	if (attackedElements != NULL)
	{
		for (ArgumentSet::iterator it = attackedElements->begin(); it != attackedElements->end(); ++it)
		{
			if (args->count(*it) > 0) return true;
		}
	}

	return false;
}

/*
***Description***
Returns the set of arguments which are colored 'IN' in the ColoringVector

INPUT: args: the arguments to be checked
	   colorings: the corresponding ColoringVector
*/
ArgumentSet AbstractArgumentationAlgorithm::getInArgs(const ArgumentSet *args, ColoringVector *colorings)
{
	ArgumentSet in;
	int index = 0;

	for(set<Argument>::iterator it2 = args->begin(); it2 != args->end(); ++it2)
	{
		if ((*colorings)[index] == IN)
		{
			in.insert(*it2);
		}
		index++;
	}

	return in;
}

/*
***Description***
Tries to insert a new element in the tupleset

INPUT: t: the tuple
	   s: the solution
	   ts: the tupleset where t and s should be inserted
	   op: the operation (CrossJoin, Union)
*/
void AbstractArgumentationAlgorithm::addToTupleSet(Tuple *t, Solution *s, TupleSet *ts, Operation op)
{
	// try to insert the tuple into the tuple set
	pair<TupleSet::iterator, bool> result = ts->insert(TupleSet::value_type(t, s));

	// if the tuple was already in the set
	if(!result.second)
	{
		// delete it and insert it again with combined solution
		Solution *orig = result.first->second;
		ts->erase(result.first);
		ts->insert(TupleSet::value_type(t, this->instantiator->combine(op, orig, s)));
	}
}

/*
***Description***
calculates colorings for branch node

INPUT: leftColoring: the left child's coloring vector
	   rightColoring: the right child's coloring vector
*/
ColoringVector AbstractArgumentationAlgorithm::getBranchColorings(ColoringVector leftColoring, ColoringVector rightColoring, ArgumentSet arguments)
{
	int index = 0;
	ColoringVector result;

	for(set<Argument>::iterator it = arguments.begin(); it != arguments.end(); ++it)
	{
		//IN if both colorings are IN
		if ((leftColoring[index] == IN) && (rightColoring[index] == IN))
		{
			result.push_back(IN);
			//cout << "Calculated IN for tupel " << i << ", Argument " << *it << endl;
		}

		//OUT if both colorings are OUT
		else if ((leftColoring[index] == OUT) && (rightColoring[index] == OUT))
		{
			result.push_back(OUT);
			//cout << "Calculated OUT for tupel " << i << ", Argument " << *it << endl;
		}

		//DEF if left or right coloring is DEF
		else if ((leftColoring[index] == DEF) || (rightColoring[index] == DEF))
		{
			result.push_back(DEF);
			//cout << "Calculated DEF for tupel " << i << ", Argument " << *it << endl;
		}

		//ATT otherwise
		else
		{
			result.push_back(ATT);
			//cout << "Calculated ATT for tupel " << i << ", Argument " << *it << endl;
		}

		index++;
	}

	return result;
}

/*
***Description***
calculates colorings for introduction node

INPUT: leftColoring: the left child's coloring vector
	   rightColoring: the right child's coloring vector
*/
ColoringVector AbstractArgumentationAlgorithm::getIntroColorings(ArgumentSet *in, ColoringVector *childColoring, int difference, bool *addTuple, ArgumentSet arguments)
{

	int index = 0;
	ColoringVector result;

	for(set<Argument>::iterator it2 = arguments.begin(); it2 != arguments.end(); ++it2)
	{
		//current arg is not the new one
		if (*it2 != difference)
		{
			result.push_back((*childColoring)[index]);
			index++;
		}
		//current arg is the new one
		else
		{
			//calculate coloring of new arg
			set<Argument> attByIn = this->attackedBySet(in, problem);

			if (attByIn.count((Argument)*it2) > 0)
			{
				result.push_back(DEF);
				//cout << "Calculated DEF for tupel " << i << ", Argument " << *it2 << endl;
			}

			//call attCheck to decide if IN-args are attacked by current argument
			else if (this->attCheck(in, (Argument) *it2, problem))
			{
				result.push_back(ATT);
				//cout << "Calculated ATT for tupel " << i << ", Argument " << *it2 << endl;
			}

			//otherwise OUT
			else
			{
				result.push_back(OUT);
				//cout << "Calculated OUT for Argument " << *it2 << endl;

				ArgumentSet curArgAsSet;
				curArgAsSet.insert(*it2);

				//add additional tuple if arg does not attack itself
				if (!(this->attCheck(&curArgAsSet, (Argument) *it2, problem)));
				{
					*addTuple = true;
				}
			}
		}
	}

	return result;
}

