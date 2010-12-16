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
	definedArgs.insert(argumentId);
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
	Vertex attacker;
	Vertex attacked;
		
	if((attacker = getVertexId(attackerId)) == VERTEXNOTFOUND)
	{
		attacker = storeVertexName(attackerId);
		args.insert(attacker);
		argMap.insert(pair<Argument, string>(attacker, attackerId));
	}
			
	if((attacked = getVertexId(attackedId)) == VERTEXNOTFOUND)
	{
		attacked = storeVertexName(attackedId);
		args.insert(attacked);
		argMap.insert(pair<Argument, string>(attacked, attackedId));
	}
	
	attacks.insert(pair<Vertex, Vertex>(attacker, attacked));
}

AttackMap ArgumentationProblem::calcAttacksAdjacency(EdgeSet *attackNbrSet)
{
	AttackMap adj;
	
	//goes through the attacks set, gets the numbers to each entry and stores them in the attackNbrSet
	for(std::set<std::pair<Vertex, Vertex> >::iterator it = attackNbrSet->begin(); it != attackNbrSet->end(); it++)
	{
		adj[it->first].insert(set<Argument>::value_type(it->second));
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
	return attacks;
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
Returns a vector of arguments attacked by the given arg.

INPUT:	arg: The argument (as number)
OUTPUT:	vector with argument attacked by arg.
*/
ArgumentSet *ArgumentationProblem::getAttacksFromArg(Argument arg) 
{
	map<Argument, ArgumentSet >::iterator iter = attacksAdjacency.find(arg);

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

INPUT:	-
OUTPUT:	-
*/
void ArgumentationProblem::preprocess()
{
	if (definedArgs.size() > args.size())
	{
		cout << endl << "The following arguments do not appear in any attack relation (they will be ignored):" << endl;
		
		for(set<string>::iterator it = definedArgs.begin(); it != definedArgs.end(); ++it)
		{
			if(getVertexId(*it) == VERTEXNOTFOUND)
			{
				cout << *it << "; ";
			}
		}
		
		cout << endl;
	}
	else if (definedArgs.size() < args.size())
	{
		cout << endl << "The following arguments have not been explicitly defined as argument:" << endl;
		
		for(ArgumentSet::iterator it = args.begin(); it != args.end(); ++it)
		{
			if(definedArgs.count(getArgumentString(*it)) == 0)
			{
				cout << getArgumentString(*it) << "; ";
			}
		}
		
		cout << endl;
	}
	
	//calculate attack set with stored numbers
	attacksAdjacency = calcAttacksAdjacency(&attacks);
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
	for(map<Argument, set<Argument> >::iterator it1 = attacksAdjacency.begin(); it1 != attacksAdjacency.end(); ++it1)
	{
			cout << "Argument " << it1->first << " attacks ";
			
			for(set<Argument>::iterator it2 = (it1->second).begin(); it2 != (it1->second).end(); ++it2)
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
		
#ifdef DEBUG
	printAF(args, attacks, attacksAdjacency);
#endif
		
	return Problem::createGraphFromSets(args, attacks);
}


