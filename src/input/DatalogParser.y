%name DatalogParser

%define LSP_NEEDED

%define MEMBERS \
	public: \
		virtual ~DatalogParser(); \
	private: \
		FlexLexer *lexer; \
		int currentRule; \
	protected: \
		virtual void addVariable(int rule, const char* name, bool negative = false, bool head = false) = 0;

%define CONSTRUCTOR_PARAM FlexLexer *lexer, istream *in

%define CONSTRUCTOR_CODE \
	this->lexer = lexer; \
	this->currentRule = 0; \
	if(in != NULL) lexer->switch_streams(in, NULL);

%define LEX_BODY { int r = lexer->yylex(); yylval.yytext = strdup(lexer->YYText()); return r; }

%define ERROR_BODY { cerr << "Parser error - line: " << lexer->lineno() << ", at word: " << lexer->YYText() << endl; }

%header{
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <map>

#undef yyFlexLexer
#define yyFLexLexerOnce
#include <FlexLexer.h>

using namespace std;

#include "../htree/Hypergraph.h"
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

rule		: head ENTAILS body END			{ ++this->currentRule; }
		| body END				{ ++this->currentRule; }
		;

head		: ATOM OR head				{ this->addVariable(this->currentRule, $1, false, true); }
		| ATOM					{ this->addVariable(this->currentRule, $1, false, true); }
		;

body		: patom AND body			{ }
		| natom AND body			{ }
		| patom					{ }
		| natom					{ }
		;

patom		: ATOM					{ this->addVariable(this->currentRule, $1, true, false); }
		;

natom		: NOT ATOM				{ this->addVariable(this->currentRule, $2, false, false); }

%%

DatalogParser::~DatalogParser() { }

