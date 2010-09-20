%name DatalogParser

%define LSP_NEEDED

%define MEMBERS \
	public: \
		~DatalogParser(); \
		DatalogProblem *getProblem(); \
	private: \
		FlexLexer *lexer; \
		int currentRule; \
		DatalogProblem *problem; \
		 \
		void nextRule(); \
		void nextVariable(const char *name, bool positive, bool head);

%define CONSTRUCTOR_PARAM FlexLexer *lexer, istream *in, DatalogProblem *problem

%define CONSTRUCTOR_CODE \
	this->lexer = lexer; \
	this->problem = problem; \
	this->currentRule = this->problem->addNewRule(); \
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

#include "../Global.h"
#include "../DatalogProblem.h"

%}

%union{
	char *yytext;
}

%token 			OR ENTAILS NOT AND END UNKNOWN
%token <yytext>		ATOM

%start datalog

%%

datalog		: program				{ }
		;

program		: rules					{ }
		;

rules		: rule					{ }
		| rule rules				{ }
		;

rule		: head ENTAILS body END			{ this->nextRule(); }
		| ENTAILS body END			{ this->nextRule(); }
		| head END				{ this->nextRule(); }
		;

head		: ATOM OR head				{ this->nextVariable($1, false, true); }
		| ATOM					{ this->nextVariable($1, false, true); }
		;

body		: patom AND body			{ }
		| natom AND body			{ }
		| patom					{ }
		| natom					{ }
		;

patom		: ATOM					{ this->nextVariable($1, true, false); }
		;

natom		: NOT ATOM				{ this->nextVariable($2, false, false); }

%%

DatalogParser::~DatalogParser() { }

inline void DatalogParser::nextRule()
{
	this->currentRule = this->problem->addNewRule();
}

inline void DatalogParser::nextVariable(const char *name, bool positive, bool head)
{
	this->problem->addEdge(this->currentRule, this->problem->addVariable(name), positive, head);
}

DatalogProblem *DatalogParser::getProblem()
{
	return this->problem;
}
