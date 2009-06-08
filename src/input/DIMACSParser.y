%name DIMACSParser

%define LSP_NEEDED

%define MEMBERS \
	public: \
		virtual ~DIMACSParser(); \
		int getClauseCount(); \
		int getVariableCount(); \
	private: \
		yyFlexLexer lexer; \
		int iClauseCount; \
		int iVariableCount; \
		int iCurrentClause; \
	protected: \
		virtual void addVariable(int iClause, int iVariable, bool bNegative = false) = 0;

%define CONSTRUCTOR_PARAM istream *in

%define CONSTRUCTOR_CODE \
	this->iClauseCount = this->iVariableCount = this->iCurrentClause = 0; \
	if(in != NULL) lexer.switch_streams(in, NULL);

%define LEX_BODY { int r = lexer.yylex(); yylval.yytext = strdup(lexer.YYText()); return r; }

%define ERROR_BODY { cerr << "Parser error - line: " << lexer.lineno() << ", at word: " << lexer.YYText() << endl; }

%header{
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <map>

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

program		: PROGRAM NUMBER NUMBER NEWLINE		{ this->iVariableCount = (int)strtol($2, NULL, 10); this->iClauseCount = (int)strtol($3, NULL, 10); }
		;

clauses		: variables ENDOFCLAUSE NEWLINE		{ }		
		| variables ENDOFCLAUSE NEWLINE clauses	{ }
		;

variables	: variable				{ ++this->iCurrentClause; }
		| variable variables			{ }
		;

variable	: NUMBER				{ addVariable(this->iCurrentClause, (int)strtol($1, NULL, 10)); }
		| NOT NUMBER				{ addVariable(this->iCurrentClause, (int)strtol($2, NULL, 10), true); }
		;

%%

DIMACSParser::~DIMACSParser() { }

int DIMACSParser::getClauseCount() { return this->iClauseCount; }

int DIMACSParser::getVariableCount() { return this->iVariableCount; }
