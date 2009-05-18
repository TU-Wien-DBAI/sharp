%option c++
%option noyywrap
%option yylineno

%{
#include <fstream>

using namespace std;

#include "DIMACSParser.h"
%}

whitespace [ \t\r]*
newline \n
number [1-9][0-9]*
program p{whitespace}cnf
comment c[^\n]*
not -

%%

{comment}	;
{whitespace}	;
{newline}	;
{program}	return DIMACSParser::PROGRAM;
{number}	return DIMACSParser::NUMBER;
{not}		return DIMACSParser::NOT;
"0"		return DIMACSParser::EC;
.		return DIMACSParser::UNKNOWN;

%%

