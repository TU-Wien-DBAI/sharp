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

ArgumentationProblem::ArgumentationProblem(istream *stream, char *credulousAcc)
	: Problem(new ArgumentationAlgorithm(this, credulousAcc))
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
	argMap.insert(pair<Argument, string>(arg, argumentId));
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
Converts the AttackSet (with <String, String>) to a set with the 
corresponding numbers (<int, int>) and returns it

INPUT:	attacks: The attack set
OUTPUT:	attackNbrSet: The attack set converted to numbers
*/
EdgeSet ArgumentationProblem::getAttackNbrSet(AttackSet *attacks)
{
	EdgeSet attackNbrSet;
	int attackerNbr, attackedNbr;
	
	//goes through the attacks set, gets the numbers to each entry and stores them in the attackNbrSet
	for(set<pair<string, string> >::iterator it = attacks->begin(); it != attacks->end(); it++)
	{
		if((attackerNbr = getVertexId(it->first)) == VERTEXNOTFOUND) 
		{
			cout << "Argument '" << it->first << "' was not found." << endl;
			continue;
		}

		if((attackedNbr = getVertexId(it->second)) == VERTEXNOTFOUND) 
		{
			cout << "Argument '" << it->second << "' was not found." << endl;
			continue;
		}	
		
		attackNbrSet.insert(make_pair(attackerNbr, attackedNbr));
	}
	
	return attackNbrSet;	
}

AttackMap ArgumentationProblem::calcAttacksAdjacency(EdgeSet *attackNbrSet)
{
	AttackMap adj;
	
	//goes through the attacks set, gets the numbers to each entry and stores them in the attackNbrSet
	for(std::set<std::pair<Vertex, Vertex> >::iterator it = attackNbrSet->begin(); it != attackNbrSet->end(); it++)
	{
		if (adj.count(it->first) == 0)
		{
			adj[it->first].insert(set<Argument>::value_type(it->second));
		}
		else
		{
			adj[it->first].insert(it->second);
		}
	}
	
	return adj;
}

/*
***Description***
Returns the AttackSet 'attacks'

INPUT:	
OUTPUT:	attacks
*/
EdgeSet ArgumentationProblem::getAttacksAsNbr()
{
	return ArgumentationProblem::getAttackNbrSet(&attacks);
}

/*
***Description***
Returns an argument's string representation

INPUT:	arg: The argument (as number)
OUTPUT:	the argument as string or NULL if the argument does not exist
*/
string ArgumentationProblem::getArgumentString(Argument arg) 
{
	map<Argument, string>::iterator iter = argMap.find(arg);

	if( iter != argMap.end() ) {
      return iter->second;
    }
	
	return NULL;
}

/*
***Description***
Returns a set of arguments attacked by the given arg.

INPUT:	arg: The argument (as number)
OUTPUT:	set with argument attacked by arg.
*/
set<Argument> *ArgumentationProblem::getAttacksFromArg(Argument arg) 
{
	map<Argument, set<Argument> >::iterator iter = attacksAdjacency.find(arg);

	if( iter != attacksAdjacency.end() ) {
      return &(iter->second);
    }
	
	return NULL;
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
}

#ifdef DEBUG
/*
***Description***
Prints the arguments and attack relations on the screen (for debugging)

INPUT:	args: The arguments
		attacks: The attack relations
OUTPUT:	-
*/
static void printAF(ArgumentSet &args, EdgeSet &attacks, AttackMap &attacksAdjacency)
{
	//go through ArgumentSet
	cout << endl << "Arguments:" << endl;
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
	
	cout << endl;

	//go through AttackMap
	cout << "Attacks as adjacency list:" << endl;	
	for(std::map<Argument, std::set<Argument> >::iterator it1 = attacksAdjacency.begin(); it1 != attacksAdjacency.end(); ++it1)
	{
			cout << "Argument " << it1->first << " attacks ";
			
			for(std::set<Argument>::iterator it2 = (it1->second).begin(); it2 != (it1->second).end(); ++it2)
			{
				cout << *it2 << " ";
		    }
			
			cout << endl;
	}
	
	cout << endl;

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
	//calculate attack set with stored numbers
	EdgeSet attackNbrSet = getAttackNbrSet(&attacks);
		
	attacksAdjacency = calcAttacksAdjacency(&attackNbrSet);
		
#ifdef DEBUG
	printAF(args, attackNbrSet, attacksAdjacency);
#endif
		
	return Problem::createHypergraphFromSets(args, attackNbrSet);
}


