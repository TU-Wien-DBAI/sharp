%option c++
%option noyywrap
%option yylineno
%option prefix="DIMACS"

%{
#include "DIMACSParser.h"
%}

delimiter [ \t]
whitespace {delimiter}+
newline \n
number [1-9][0-9]*
program p{whitespace}cnf{whitespace}
comment c({delimiter}*|{whitespace}[^\n]*)
not -

%%

{whitespace}	;
{comment}	return DIMACSParser::COMMENT;
{newline}	return DIMACSParser::NEWLINE;
{program}	return DIMACSParser::PROGRAM;
{number}	return DIMACSParser::NUMBER;
{not}		return DIMACSParser::NOT;
"0"		return DIMACSParser::ENDOFCLAUSE;
.		return DIMACSParser::UNKNOWN;

%%

