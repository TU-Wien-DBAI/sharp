/*
* Author: 		Clemens Nopp, Matr.Nr. 0826054
* Date: 		September 2010
* This File:	Problem processing for Argumentation Frameworks
*/ 

#include <fstream>
#include <iostream>

#include "ArgumentationProblem.h"
#include "input/ArgumentationParser.h"
#include "sharp/Argumentation.h"

#undef yyFlexLexer
#define yyFlexLexer ArgumentationFlexLexer
#include <FlexLexer.h>

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ArgumentationProblem::ArgumentationProblem(istream *stream)
	: Problem(new ArgumentationAlgorithm(this))
{
	this->parser = new ArgumentationParser(new ArgumentationFlexLexer(), stream, this);
}

ArgumentationProblem::~ArgumentationProblem()
{
}

//////////////////////////////////////////////////////////////////////
// Class methods
//////////////////////////////////////////////////////////////////////

/*
***Description***
This method is called from the parser when a new argument appears.
The new argument is stored as a vertex and in the args list.

INPUT:	argumentId: A string representation of the argument
OUTPUT:	-
*/
void ArgumentationProblem::addArgument(string argumentId)
{
	//return if argument already exists
	if(getVertexId(argumentId) != VERTEXNOTFOUND) return;
	
	//otherwise: store argument and add it to the args list
	Vertex arg = storeVertexName(argumentId);
	args.insert(arg);
}

/*
***Description***
This method is called from the parser when a new attack relation appears.
The new attack relation is stored in the attacks list.

INPUT:	attackerId: A string representation of the attacking argument
		attackedId: A string representation of the attacked argument
OUTPUT:	-
*/
void ArgumentationProblem::addAttack(string attackerId, string attackedId)
{
	attacks.insert(make_pair(attackerId, attackedId));
}

/*
***Description***
Parses the information about the argumentation framework from the
input file.

INPUT:	-	
OUTPUT:	-
*/
void ArgumentationProblem::parse()
{
	this->parser->yyparse();
}

/*
***Description***
This is called directly after parsing, allows for preliminary optimizations (i.e. redundancy elimination, etc.)
(currently not used)

INPUT:	-
OUTPUT:	-
*/
void ArgumentationProblem::preprocess()
{
	// currently not used...
	// todo: check if the elements of attack pairs appear in the vertex set; duplicate checks...
}

#ifdef DEBUG
/*
***Description***
Prints the arguments and attack relations on the screen (for debugging)

INPUT:	args: The arguments
		attacks: The attack relations
OUTPUT:	-
*/
static void printAF(ArgumentSet &args, EdgeSet &attacks)
{
	//go through ArgumentSet
	cout << "Arguments:" << endl;
	for(set<Vertex>::iterator it = args.begin(); it != args.end(); ++it)
	{
			cout << *it << endl;
	}
	
	cout << endl;
	
	//go through EdgeSet
	cout << "Attacks:" << endl;	
	for(set<pair<Vertex, Vertex> >::iterator it = attacks.begin(); it != attacks.end(); ++it)
	{
			cout << it->first << " attacks " << it->second << endl;
	}
}
#endif

/*
***Description***
Generates an EdgeSet from the attacks list by searching and storing of the related arguments and calls 
createHypergraphFromSets to create a hypergraph representation of the argumentation framework.

INPUT:	-
OUTPUT:	a hypergraph representation of the argumentation framework
*/
Hypergraph *ArgumentationProblem::buildHypergraphRepresentation()
{

	EdgeSet attackNbrSet;
	int attackerNbr, attackedNbr;
	
	//goes through the attacks set, gets the numbers to each entry and stores them in the attackNbrSet
	for(set<pair<string, string> >::iterator it = attacks.begin(); it != attacks.end(); it++)
	{
		attackerNbr = getVertexId(it->first);
		attackedNbr = getVertexId(it->second);
		attackNbrSet.insert(make_pair(attackerNbr, attackedNbr));
	}

#ifdef DEBUG
	printAF(args, attackNbrSet);
#endif
		
	return Problem::createHypergraphFromSets(args, attackNbrSet);
}
