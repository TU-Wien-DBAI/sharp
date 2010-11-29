/*
* Author: 		Clemens Nopp, Matr.Nr. 0826054
* Date: 		September 2010
* This File:	Parser for Argumentation Frameworks
*/

%name ArgumentationParser

%define LSP_NEEDED

%define MEMBERS \
	public: \
		~ArgumentationParser(); \
		ArgumentationProblem *getProblem(); \
	private: \
		FlexLexer *lexer; \
		ArgumentationProblem *problem; \
		std::string trim(const std::string &str); \
		void addArgumentId(std::string arg); \
		void addAttackId(std::string att); \

%define CONSTRUCTOR_PARAM FlexLexer *lexer, istream *in, ArgumentationProblem *problem

%define CONSTRUCTOR_CODE \
	this->lexer = lexer; \
	this->problem = problem; \
	if(in != NULL) lexer->switch_streams(in, NULL);

%define LEX_BODY { int r = lexer->yylex(); yylval.yytext = strdup(lexer->YYText()); return r; }

%define ERROR_BODY { cerr << endl << "Parser error - line: " << lexer->lineno() << ", at word: " << lexer->YYText() << endl; }


%header{
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <map>

#undef yyFlexLexer
#define yyFLexLexerOnce
#include <FlexLexer.h>

using namespace std;

void yyerror(const char *error_text);
int yylex();

#include "../Global.h"
#include "../ArgumentationProblem.h"

%}

%union{
	char *yytext;
}


%token 				UNKNOWN
%token <yytext>		ARG ATT

%start argumentation

%%

argumentation: 		ARG argumentation 				{ this->addArgumentId($1); }
					| ATT argumentation				{ this->addAttackId($1); }
					|
		;

%%

ArgumentationParser::~ArgumentationParser() { }

std::string ArgumentationParser::trim(const std::string &str)
{
    size_t s = str.find_first_not_of(" \n\r\t");
    size_t e = str.find_last_not_of (" \n\r\t");

    if(( string::npos == s) || ( string::npos == e))
        return "";
    else
        return str.substr(s, e-s+1);
}

void ArgumentationParser::addArgumentId(string arg)
{
	/* Arguments are represented with e.g. "arg(a).". This example would return "a" */
	string argumentId = this->trim(arg.substr(4, arg.length()-6));
	
	this->problem->addArgument(argumentId);
}

void ArgumentationParser::addAttackId(string att)
{
	/* Attacks are represented with e.g. "att(a,b)." This example would return "a" and "b"*/ 
	int commaPos = att.find(",",0);
	string attackerId = att.substr(4, commaPos-4);
	string attackedId = this->trim(att.substr(commaPos+1, att.length()-commaPos-3));

	this->problem->addAttack(attackerId, attackedId);	
}

ArgumentationProblem *ArgumentationParser::getProblem()
{
	return this->problem;
}
