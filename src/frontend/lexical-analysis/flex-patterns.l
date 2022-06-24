%{

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "flex-actions.h"

#define MAX_DEPTH 72

unsigned int indent_level = 0;
unsigned char flag_first = 1;

char process_indent(TokenID* ans, char* line, unsigned int length);
TokenID temp;

%}

/* Obliga a crear una regla explícita para manejar lexemas desconocidos. */
%option nodefault

/* Escanea un único archivo y se detiene. */
%option noyywrap

/* Dispone una variable global "yylineno" con el número de línea actual. */
%option yylineno

/* No distingue entre mayusculas y minusculas */
%option case-insensitive
%option caseless

/* Estados para el procesamiento */
%x COMMENT

/* Patrones reutilizables. */
alpha				[a-zA-Z]
digit				[0-9]
newline         	\n
whitespace			[ \f\n\r\t\v]

%%
#								{ BEGIN(COMMENT); }
<COMMENT>.*						{ CommentPatternAction(yytext); }
<COMMENT>\n						{ BEGIN(INITIAL); }

start							{ return KeywordPatternAction(yytext, START); }
create							{ return KeywordPatternAction(yytext, CREATE); }
insert							{ return KeywordPatternAction(yytext, INSERT); }
into                            { return KeywordPatternAction(yytext, INTO); }
for								{ return KeywordPatternAction(yytext, FOR); }
from							{ return KeywordPatternAction(yytext, FROM); }
while							{ return KeywordPatternAction(yytext, WHILE); }
if								{ return KeywordPatternAction(yytext, IF); }
else                            { return KeywordPatternAction(yytext, ELSE); }
to								{ return KeywordPatternAction(yytext, TO); }
inclusive						{ return KeywordPatternAction(yytext, INCLUSIVE); }
exclusive						{ return KeywordPatternAction(yytext, EXCLUSIVE); }
with							{ return KeywordPatternAction(yytext, WITH); }
dfs                             { return KeywordPatternAction(yytext, DFS); }
bfs                             { return KeywordPatternAction(yytext, BFS); }
is								{ return KeywordPatternAction(yytext, IS); }
not								{ return KeywordPatternAction(yytext, NOT); }
empty							{ return KeywordPatternAction(yytext, EMPTY_TYPE); }
pop								{ return KeywordPatternAction(yytext, POP); }
entry							{ return KeywordPatternAction(yytext, ENTRY); }
print							{ return KeywordPatternAction(yytext, PRINT); }
def								{ return KeywordPatternAction(yytext, DEF); }
in								{ return KeywordPatternAction(yytext, IN); }
return							{ return KeywordPatternAction(yytext, RETURN); }
dump							{ return KeywordPatternAction(yytext, DUMP); }
dot							    { return KeywordPatternAction(yytext, GRAPHVIZ_DOT); }
let							    { return KeywordPatternAction(yytext, LET); }
be							    { return KeywordPatternAction(yytext, BE); }
node							{ return KeywordPatternAction(yytext, NODE); }
edge							{ return KeywordPatternAction(yytext, EDGE); }
edges							{ return KeywordPatternAction(yytext, EDGES); }
nodes							{ return KeywordPatternAction(yytext, NODES); }
queue							{ return KeywordPatternAction(yytext, QUEUE); }
stack							{ return KeywordPatternAction(yytext, STACK); }
set								{ return KeywordPatternAction(yytext, SET); }
graph							{ return KeywordPatternAction(yytext, GRAPH); }
digraph							{ return KeywordPatternAction(yytext, DIGRAPH); }
integer							{ return KeywordPatternAction(yytext, INTEGER_TYPE); }
boolean                         { return KeywordPatternAction(yytext, BOOLEAN_TYPE); }
decimal							{ return KeywordPatternAction(yytext, DECIMAL_TYPE); }
char							{ return KeywordPatternAction(yytext, CHAR_TYPE); }
string							{ return KeywordPatternAction(yytext, STRING_TYPE); }
and                             { return KeywordPatternAction(yytext, AND); }
or                              { return KeywordPatternAction(yytext, OR); }
data                            { return KeywordPatternAction(yytext, DATA); }
as								{ return KeywordPatternAction(yytext, AS); }
"<<"                            { return KeywordPatternAction(yytext, LT); }
">>"                            { return KeywordPatternAction(yytext, GT); }
"<="                            { return KeywordPatternAction(yytext, LEQ); }
">="                            { return KeywordPatternAction(yytext, GEQ); }
"=="                            { return KeywordPatternAction(yytext, EQ); }
"("								{ return KeywordPatternAction(yytext, OPEN_PARENTHESIS); }
")"								{ return KeywordPatternAction(yytext, CLOSE_PARENTHESIS); }
"<"								{ return KeywordPatternAction(yytext, OPEN_ANTILAMBDA); }
">"								{ return KeywordPatternAction(yytext, CLOSE_ANTILAMBDA); }
","								{ return KeywordPatternAction(yytext, COMMA); }
":"								{ return KeywordPatternAction(yytext, COLON); }
"."								{ return KeywordPatternAction(yytext, DOT); }
"<-"						    { return KeywordPatternAction(yytext, LEFT_ARROW); }
"->"						    { return KeywordPatternAction(yytext, RIGHT_ARROW); }
"+"						    	{ return KeywordPatternAction(yytext, ADD); }
"-"								{ return KeywordPatternAction(yytext, SUB); }
"*"								{ return KeywordPatternAction(yytext, MUL); }
"/"								{ return KeywordPatternAction(yytext, DIV); }

false							{ return BooleanPatternAction(yytext, 0); }
true							{ return BooleanPatternAction(yytext, 1); }
\'([^\\]|\\.)\'					{ return CharPatternAction(yytext); }
{digit}+						{ return IntegerPatternAction(yytext); }
{digit}+\.{digit}*				{ return DecimalPatternAction(yytext); }
\"(\\.|[^\"\\])*\"				{ return StringPatternAction(yytext); }

({alpha}|_)({alpha}|_|{digit})*	{ return IdentifierPatternAction(yytext); }

^{whitespace}*\n				{ /* Ignore blank lines */ }
^[ ]*[^ \n]+					{ if ( process_indent(&temp, yytext, yyleng) ) {
									/* printf("Indenting%d...\n", temp); */
									return temp;
								} }
<<EOF>>							{ if ( process_indent(&temp, yytext, 0) ) {
									/* printf("Indenting%d...\n", temp); */
									return temp;
								}
								else {
									/* printf("GUSBAI\n"); */
									return 0;
								} }

{newline}       				{ return KeywordPatternAction(yytext, EOL); }

{whitespace}					{ IgnoredPatternAction(yytext); }

.								{ return UnknownPatternAction(yytext); }

%%

unsigned int white_count(char* line) {
	unsigned int count = 0 ;
	while (*line == ' ')
		count++, line++ ;
	return count;
}

char process_indent(TokenID* ans, char* line, unsigned int length) {
	unsigned int curr_indent = white_count(line);
	char * end_indent;
	for (end_indent = line; *end_indent == ' ' || *end_indent == '\t'; end_indent++);
	for (char * it = line + length - 1; it >= end_indent; it--) {
		unput(*it);
	}

	if (curr_indent == indent_level) {
/* 		if (!flag_first) {
			*ans = EOL;
			return 1;
		}
		flag_first = 0; */
		return 0;
	} else if (curr_indent == indent_level + 4) {
		indent_level += 4;
		*ans = IndentationPatternAction(INDENT);
		return 1;
	} else if ( (indent_level - curr_indent) % 4 == 0 ) {
		indent_level -= 4;
		for (int i = 0; i < curr_indent; i++) {
			unput(' ');
		}
		if (indent_level != curr_indent)
			unput('\n');
		*ans = IndentationPatternAction(DEDENT);
		return 1;
	}
	return 0;
}