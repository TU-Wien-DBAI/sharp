/*
* Author: 		Clemens Nopp, Matr.Nr. 0826054
* Date: 		September 2010
* This File:	Problem processing for Argumentation Frameworks (Header)
*/ 

#ifndef ARGUMENTATIONPROBLEM_H_
#define ARGUMENTATIONPROBLEM_H_

#include <string>
#include <fstream>

#include "Global.h"
#include "Problem.h"

typedef Vertex Argument;
typedef VertexSet ArgumentSet;
typedef std::map<Argument, std::string> ArgumentMap;
typedef std::set<std::pair<std::string, std::string> > AttackSet;
typedef std::map<Argument, std::set<Argument> > AttackMap;

class ArgumentationParser;

class ArgumentationProblem : public Problem
{
public:
	//Constructor/Destructor
	ArgumentationProblem(std::istream *stream, char *credulousAcc);
	virtual ~ArgumentationProblem();
	
	//This method is called from the parser when a new argument appears.
	//The new argument is stored as a vertex and in the args list.
	void addArgument(std::string argumentId);

	//This method is called from the parser when a new attack relation appears.
	//The new attack relation is stored in the attacks list.
	void addAttack(std::string attackerId, std::string attackedId);
	
	//Get attack set (representation in numbers)
	EdgeSet getAttacksAsNbr();
	
	//Get string representation of arg
	std::string getArgumentString(Argument arg);
	
	//Get set of arguments attacked by the given arg.
	set<Argument> *getAttacksFromArg(Argument arg); 

protected:
	//Parses the information about the argumentation framework from the input file.
	virtual void parse();
	
	//This is called directly after parsing, allows for preliminary optimizations (i.e. redundancy elimination, etc.)
	//(currently not used)
	virtual void preprocess();

	//Generates an EdgeSet from the attacks list by searching and storing of the related arguments and calls 
	//createHypergraphFromSets to create a hypergraph representation of the argumentation framework.
	virtual Hypergraph *buildHypergraphRepresentation();

private:
	EdgeSet getAttackNbrSet(AttackSet *attacks);
	AttackMap calcAttacksAdjacency(EdgeSet *attackNbrSet);

	//the attack relations
	AttackSet attacks;
	
	//Attack relations stored in an ajdacency list
	AttackMap attacksAdjacency;
	
	//the arguments
	ArgumentSet args;
	ArgumentMap argMap;
	
	ArgumentationParser *parser;
};

#endif //ARGUMENTATIONPROBLEM_H_
