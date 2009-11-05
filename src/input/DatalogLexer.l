%option c++
%option noyywrap
%option yylineno
%option prefix="Datalog"

%{
#include "DatalogParser.h"
%}

delimiter [ \r\t]
whitespace {delimiter}+
newline \n
comment (%[^\n]*{newline})
atom [a-zA-Z][a-zA-Z0-9]*
not not
doublenot ({not}{not})+
end \.
or v
and ,
entails :-

%%

{comment}	;
{whitespace}	;
{newline}	;
{doublenot}	;
{or}		return DatalogParser::OR;
{entails}	return DatalogParser::ENTAILS;
{and}		return DatalogParser::AND;
{not}		return DatalogParser::NOT;
{end}		return DatalogParser::END;
{atom}		return DatalogParser::ATOM;
.		return DatalogParser::UNKNOWN;

%%

