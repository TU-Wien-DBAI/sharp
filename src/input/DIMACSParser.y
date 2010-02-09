%name DIMACSParser

%define LSP_NEEDED

%define MEMBERS \
	public: \
		virtual ~DIMACSParser(); \
		int getClauseCount(); \
		int getVariableCount(); \
	private: \
		FlexLexer *lexer; \
		int clauseCount; \
		int variableCount; \
		int currentClause; \
	protected: \
		virtual void addVariable(int clause, int variable, bool negative = false) = 0;

%define CONSTRUCTOR_PARAM FlexLexer *lexer, istream *in

%define CONSTRUCTOR_CODE \
	this->lexer = lexer; \
	this->clauseCount = this->variableCount = this->currentClause = 0; \
	if(in != NULL) lexer->switch_streams(in, NULL);

%define LEX_BODY { int r = lexer->yylex(); yylval.yytext = strdup(lexer->YYText()); return r; }

%define ERROR_BODY { cerr << endl << "Parser error - line: " << lexer->lineno() << ", at word: " << lexer->YYText() << endl; }

%header{
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <map>

#undef yyFlexLexer
#define yyFlexLexerOnce
#include <FlexLexer.h>

using namespace std;

#include "../htree/Hypergraph.h"
%}

%union{
	char *yytext;
}

%token 			ENDOFCLAUSE NOT PROGRAM COMMENT NEWLINE UNKNOWN
%token <yytext>		NUMBER

%start dimacs

%%

dimacs		: comments program clauses		{ }
		;

comments	: 					{ }
		| COMMENT NEWLINE comments		{ }

program		: PROGRAM NUMBER NUMBER NEWLINE		{ this->variableCount = (int)strtol($2, NULL, 10); this->clauseCount = (int)strtol($3, NULL, 10); }
		;

clauses		: variables ENDOFCLAUSE NEWLINE		{ }		
		| variables ENDOFCLAUSE NEWLINE clauses	{ }
		;

variables	: variable				{ ++this->currentClause; }
		| variable variables			{ }
		;

variable	: NUMBER				{ addVariable(this->currentClause, (int)strtol($1, NULL, 10)); }
		| NOT NUMBER				{ addVariable(this->currentClause, (int)strtol($2, NULL, 10), true); }
		;

%%

DIMACSParser::~DIMACSParser() { }

int DIMACSParser::getClauseCount() { return this->clauseCount; }

int DIMACSParser::getVariableCount() { return this->variableCount; }
