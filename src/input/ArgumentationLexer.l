/*
* Lexer for Argumentation Frameworks in DLV syntax
* Arguments are represented with e.g. "arg(a). arg(b)."
* Attacks are represented with e.g. "att(a,b)." 
*
* Clemens Nopp, MatrNr 0826054
*/

%option c++
%option noyywrap
%option yylineno
%option prefix="Argumentation"

%{
#include "ArgumentationParser.h"
%}

delimiter 		[ \r\t]
whitespace 		{delimiter}+
newline 		\n
comment 		(%[^\n]*{newline})
identifier		[a-z][a-z0-9]*
arg 			arg\({identifier}\)\.
att 			att\({identifier}\,[ ]*{identifier}\)\.

%%

{comment}		; /*ignore comments*/
{whitespace}	; /*ignore whitespaces*/
{newline}		; /*ignore newlines*/
{identifier}	; /*ignore (standalone) identifiers*/ 
{arg}			return ArgumentationParser::ARG;
{att}			return ArgumentationParser::ATT;
.				return ArgumentationParser::UNKNOWN;

%%
