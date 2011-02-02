/*
* Author: 		Clemens Nopp, Matr.Nr. 0826054
* Date: 		September 2010
* This File:	General functions for Argumentation Framework Algorithms (Header)
*/

#ifndef ABSTRACTARGUMENTATIONPROGRAM_H_
#define ABSTRACTARGUMENTATIONPROGRAM_H_

#include "AbstractAlgorithm.h"
#include "../ArgumentationProblem.h"

class AbstractArgumentationAlgorithm : public AbstractAlgorithm
{
public:
	AbstractArgumentationAlgorithm(Problem *problem);
	virtual ~AbstractArgumentationAlgorithm();

protected:
	ArgumentationProblem *problem;

protected:
	//Calculates all conflict free sets
	std::vector< ArgumentSet > conflictFreeSets(const ArgumentSet *args);

	//Calculates a set of arguments attacked by the given args
	ArgumentSet attackedBySet(const ArgumentSet *args, ArgumentationProblem *problem);

	//Decides if the given arg has the coloring ATT
	bool attCheck(const ArgumentSet *args, Argument arg, ArgumentationProblem *problem);

	//Returns the set of arguments which are colored 'IN' in the ColoringVector
	ArgumentSet getInArgs(const ArgumentSet *args, ColoringVector *colorings);

	//calculates colorings for branch node
	ColoringVector getBranchColorings(ColoringVector *leftColoring, ColoringVector *rightColoring, const ArgumentSet *arguments);

	//calculates colorings for introduction node
	ColoringVector getIntroColorings(ArgumentSet *in, ColoringVector *childColoring, Argument difference, bool *addTuple, const ArgumentSet *arguments);

};

#endif
