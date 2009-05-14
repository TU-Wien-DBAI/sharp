%name DIMACSParser

%define LSP_NEEDED

%define MEMBERS \
	virtual ~DIMACSParser() { } \
	private: \
		yyFlexLexer lexer;

%define LEX_BODY { return lexer.yylex(); }

%define ERROR_BODY { cerr << "Parser error - line: " << lexer.lineno() << ", at word:" << lexer.YYText() << endl; }

%{
#include <fstream>
#include <FlexLexer.h>

using namespace std;
%} 

%union{
	int variable;
}

%token 			EC NOT PROGRAM
%token <variable>	NUMBER

%type <variable>		variable

%start dimacs

%%

dimacs		: program clauses
		;

program		: PROGRAM NUMBER NUMBER
		;

clauses		: variables EC
		| variables EC clauses
		;

variables	: variable
		| variable variables
		;

variable	: NUMBER
		| NOT NUMBER
		;

%%
