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
	this->args = 0;
	this->attacks = 0;
}

ArgumentationProblem::~ArgumentationProblem()
{
}

void ArgumentationProblem::addArgument(string argumentId)
{
	/* TODO: adds an argument to the argument set (= vertex set) */
}

void ArgumentationProblem::addAttack(string attackerId, string attackedId)
{
	/* TODO: adds an attack relation to the attack set (= edge set) and stores the direction of the attack */
}

void ArgumentationProblem::parse()
{
	this->parser->yyparse();
}

void ArgumentationProblem::preprocess()
{
	
}

#ifdef DEBUG
static void printAF(ArgumentSet &args, AttackSet &attacks)
{
}
#endif

Hypergraph *ArgumentationProblem::buildHypergraphRepresentation()
{
#ifdef DEBUG
	printAF(args, attacks);
#endif

	return Problem::createHypergraphFromSets(args, attacks);
}
