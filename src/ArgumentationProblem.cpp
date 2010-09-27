#include <fstream>
#include <iostream>

#include "ArgumentationProblem.h"

#undef yyFlexLexer
#define yyFlexLexer ArgumentationFlexLexer
#include <FlexLexer.h>

#include "input/ArgumentationParser.h"

#include "sharp/AnswerSet.h"
#include "sharp/HeadCycleFreeAnswerSet.h"

using namespace std;

ArgumentationProblem::ArgumentationProblem(istream *stream)
	: Problem(0)
{
	this->parser = new ArgumentationParser(new ArgumentationFlexLexer(), stream, this);
}

ArgumentationProblem::~ArgumentationProblem()
{
}

void ArgumentationProblem::addArgument(string argumentId)
{
	Vertex arg = storeVertexName(argumentId);
	args.insert(arg);
}

void ArgumentationProblem::addAttack(string attackerId, string attackedId)
{
	attacks.insert(make_pair(attackerId, attackedId));
}

void ArgumentationProblem::parse()
{
	this->parser->yyparse();
}

void ArgumentationProblem::preprocess()
{
	// currently not used...
	// todo: check if the elements of attack pairs appear in the vertex set; duplicate checks...
}

#ifdef DEBUG
static void printAF(ArgumentSet &args, AttackSet &attacks)
{
	//go through ArgumentSet
	cout << "Arguments:" << endl;
	for(set<Vertex>::iterator it = args.begin(); it != args.end(); ++it)
	{
			cout << *it << endl;
	}
	
	cout << endl;
	
	//go through AttackSet
	cout << "Attacks:" << endl;	
	for(set<pair<string, string> >::iterator it = attacks.begin(); it != attacks.end(); ++it)
	{
			cout << it->first << " attacks " << it->second << endl;
	}
}
#endif

Hypergraph *ArgumentationProblem::buildHypergraphRepresentation()
{
#ifdef DEBUG
	printAF(args, attacks);
#endif

	EdgeSet attackNbrSet;
	int attackerNbr, attackedNbr;
	
	//goes through the attacks set, gets the numbers to each entry and stores them in the attackNbrSet
	for(set<pair<string, string> >::iterator it = attacks.begin(); it != attacks.end(); ++it)
	{
		attackerNbr = getVertexId(it->first);
		attackedNbr = getVertexId(it->first);
		attackNbrSet.insert(make_pair(attackerNbr, attackedNbr));
	}
	
	return Problem::createHypergraphFromSets(args, attackNbrSet);
}
