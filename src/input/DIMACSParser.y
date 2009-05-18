%name DIMACSParser

%define LSP_NEEDED

%define MEMBERS \
		virtual ~DIMACSParser() { } \
		long getClauses() { return this->lClauses; } \
		long getVariables() { return this->lVariables; } \
	private: \
		yyFlexLexer lexer; \
		long lClauses; \
		long lVariables; \
		long lCurrentClause; \
		void addVariable(long number);

%define LEX_BODY { yylval.yytext = strdup(lexer.YYText()); return lexer.yylex(); }

%define ERROR_BODY { cerr << "Parser error - line: " << lexer.lineno() << ", at word: " << lexer.YYText() << endl; }

%header{
#include "../htree/Hypergraph.h"
%}

%{
#include <fstream>
#include <cstdlib>
#include <cstring>

#include <FlexLexer.h>

using namespace std;
%} 

%union{
	char *yytext;
}

%token 			EC NOT PROGRAM UNKNOWN NUMBER

%start dimacs

%%

dimacs		: program clauses		{ }
		;

program		: PROGRAM NUMBER NUMBER		{ this->lClauses = strtol($<yytext>2, NULL, 10); this->lVariables = strtol($<yytext>3, NULL, 10); }
		;

clauses		: variables EC			{ }		
		| variables EC clauses		{ ++this->lCurrentClause; }
		;

variables	: variable			{ }
		| variable variables		{ }
		;

variable	: NUMBER			{  }
		| NOT NUMBER			{ cout << "Got negative variable number;" << endl; }
		;

%%

void DIMACSParser::addVariable(long number)
{
	;
}
