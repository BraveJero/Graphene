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
unused_whitespace	[ \f\r\t\v]
whitespace			[ \f\n\r\t\v]

%%
#								{ BEGIN(COMMENT); }
<COMMENT>.						{  }
<COMMENT>\n						{ BEGIN(INITIAL); }

^{whitespace}*\n				{ /* Ignore blank lines */ }
^[ ]*[^ \n]+					{ printf("{%s}\n", yytext);
									if ( process_indent(&temp, yytext, yyleng) )
										return temp;
								}
<<EOF>>							{ printf("{%s}\n", yytext);
									if ( process_indent(&temp, yytext, yyleng) )
										return temp;
									else
										return 0;
								}

start							{ printf("{%s}\n", yytext); return START; }
create							{ printf("{%s}\n", yytext); return CREATE; }
insert							{ printf("{%s}\n", yytext); return INSERT; }
into                            { printf("{%s}\n", yytext); return INTO; }
for								{ printf("{%s}\n", yytext); return FOR; }
from							{ printf("{%s}\n", yytext); return FROM; }
while							{ printf("{%s}\n", yytext); return WHILE; }
if								{ printf("{%s}\n", yytext); return IF; }
else                            { printf("{%s}\n", yytext); return ELSE; }
to								{ printf("{%s}\n", yytext); return TO; }
inclusive						{ printf("{%s}\n", yytext); return INCLUSIVE; }
exclusive						{ printf("{%s}\n", yytext); return TO; }
with							{ printf("{%s}\n", yytext); return WITH; }
value							{ printf("{%s}\n", yytext); return VALUE; }
dfs                             { printf("{%s}\n", yytext); return DFS; }
bfs                             { printf("{%s}\n", yytext); return BFS; }
is								{ printf("{%s}\n", yytext); return IS; }
not								{ printf("{%s}\n", yytext); return NOT; }
empty							{ printf("{%s}\n", yytext); return EMPTY_TYPE; }
pop								{ printf("{%s}\n", yytext); return POP; }
entry							{ printf("{%s}\n", yytext); return ENTRY; }
print							{ printf("{%s}\n", yytext); return PRINT; }
def								{ printf("{%s}\n", yytext); return DEF; }
label							{ printf("{%s}\n", yytext); return LABEL; }
get								{ printf("{%s}\n", yytext); return GET; }
in								{ printf("{%s}\n", yytext); return IN; }
return							{ printf("{%s}\n", yytext); return RETURN; }
dump							{ printf("{%s}\n", yytext); return DUMP; }
dot							    { printf("{%s}\n", yytext); return GRAPHVIZ_DOT; }
let							    { printf("{%s}\n", yytext); return LET; }
be							    { printf("{%s}\n", yytext); return BE; }

\'([^\\]|\\.)\'					{ printf("{%s}\n", yytext); return CHAR; }
true|false                      { printf("{%s}\n", yytext); return BOOLEAN; }
{digit}+						{ printf("{%s}\n", yytext); return IntegerPatternAction(yytext); }
{digit}+.{digit}                { printf("{%s}\n", yytext); return DecimalPatternAction(yytext); }
\"(\\.|[^\"\\])*\"              { printf("{%s}\n", yytext); return StringPatternAction(yytext); }

node							{ printf("{%s}\n", yytext); return NODE; }
edge							{ printf("{%s}\n", yytext); return EDGE; }
queue							{ printf("{%s}\n", yytext); return QUEUE; }
stack							{ printf("{%s}\n", yytext); return STACK; }
set								{ printf("{%s}\n", yytext); return SET; }
graph							{ printf("{%s}\n", yytext); return GRAPH; }
digraph							{ printf("{%s}\n", yytext); return DIGRAPH; }

integer							{ printf("{%s}\n", yytext); return INTEGER_TYPE; }
any								{ printf("{%s}\n", yytext); return ANY_TYPE; }
boolean                         { printf("{%s}\n", yytext); return BOOLEAN_TYPE; }
decimal							{ printf("{%s}\n", yytext); return DECIMAL_TYPE; }
char							{ printf("{%s}\n", yytext); return CHAR_TYPE; }
string							{ printf("{%s}\n", yytext); return STRING_TYPE; }



"+"						    	{ printf("{%s}\n", yytext); return ADD; }
"-"								{ printf("{%s}\n", yytext); return SUB; }
"*"								{ printf("{%s}\n", yytext); return MUL; }
"/"								{ printf("{%s}\n", yytext); return DIV; }

"<<"                            { printf("{%s}\n", yytext); return LT; }
">>"                            { printf("{%s}\n", yytext); return GT; }
"<="                            { printf("{%s}\n", yytext); return LEQ; }
">="                            { printf("{%s}\n", yytext); return GEQ; }
"=="                            { printf("{%s}\n", yytext); return EQ; }
and                             { printf("{%s}\n", yytext); return AND; }   
or                              { printf("{%s}\n", yytext); return OR; } 

"("								{ printf("{%s}\n", yytext); return OPEN_PARENTHESIS; }
")"								{ printf("{%s}\n", yytext); return CLOSE_PARENTHESIS; }
"<"								{ printf("{%s}\n", yytext); return OPEN_ANTILAMBDA; }
">"								{ printf("{%s}\n", yytext); return CLOSE_ANTILAMBDA; }
","								{ printf("{%s}\n", yytext); return COMMA; }
":"								{ printf("{%s}\n", yytext); return COLON; }
"."								{ printf("{%s}\n", yytext); return DOT; }
";"								{ printf("{%s}\n", yytext); return SEMICOLON; }
"<-"						    { printf("{%s}\n", yytext); return LEFT_ARROW; }
"->"						    { printf("{%s}\n", yytext); return RIGHT_ARROW; }
       
({alpha}|_)({alpha}|_|{digit})*	{ printf("{%s}\n", yytext); return IDENTIFIER; }

{newline}       				{ printf("{%s}\n", yytext); return EOL; }

{whitespace}					{ printf("IGNORE{%s}\n", yytext); /* IgnoredPatternAction(yytext); */ }

.								{ printf("{%s}\n", yytext); return UnknownPatternAction(yytext); }

%%

unsigned int white_count(char* line) {
	unsigned int count = 0 ;
	while (*line == ' ')
		count++, line++ ;
	printf("White count: %d\n", count);
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
		return 0;
	} else if (curr_indent == indent_level + 4) {
		printf("--------------------------------INDENT\n");
		indent_level += 4;
		*ans = INDENT;
		return 1;
	} else if ( (indent_level - curr_indent) % 4 == 0 ) {
		indent_level -= 4;
		printf("--------------------------------DEDENT\n");
		for (int i = 0; i < curr_indent; i++) {
			unput(' ');
		}
		unput('\n');
		*ans = DEDENT;
		return 1;
	}
	return 0;
}
