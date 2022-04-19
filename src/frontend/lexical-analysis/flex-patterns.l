%{

#include "flex-actions.h"

%}

/* Obliga a crear una regla explícita para manejar lexemas desconocidos. */
%option nodefault

/* Escanea un único archivo y se detiene. */
%option noyywrap

/* Dispone una variable global "yylineno" con el número de línea actual. */
%option yylineno

/* No distingue entre mayusculas y minusculas */
%option caseless

/* Estados para el procesamiento */
%x COMMENT

/* Patrones reutilizables. */
alpha			[a-zA-Z]
digit			[0-9]
whitespace		[ \f\r\t\v]
newline         \n

%%
#								{ BEGIN(COMMENT); }
<COMMENT>.						{  }
<COMMENT>\n						{ BEGIN(INITIAL); }

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
equals							{ printf("{%s}\n", yytext); return EQUALS; }
return							{ printf("{%s}\n", yytext); return RETURN; }
dump							{ printf("{%s}\n", yytext); return DUMP; }
dot							    { printf("{%s}\n", yytext); return GRAPHVIZ_DOT; }
let							    { printf("{%s}\n", yytext); return LET; }
be							    { printf("{%s}\n", yytext); return BE; }

\'.\'							{ printf("{%s}\n", yytext); return CHAR; }
{digit}+						{ printf("{%s}\n", yytext); return IntegerPatternAction(yytext); }
{digit}+.{digit}                { printf("{%s}\n", yytext); return DecimalPatternAction(yytext); }
\"(\\.|[^"\\])*\"               { printf("{%s}\n", yytext); return StringPatternAction(yytext); }

node							{ printf("{%s}\n", yytext); return NODE; }
edge							{ printf("{%s}\n", yytext); return EDGE; }
queue							{ printf("{%s}\n", yytext); return QUEUE; }
stack							{ printf("{%s}\n", yytext); return STACK; }
set								{ printf("{%s}\n", yytext); return SET; }
graph							{ printf("{%s}\n", yytext); return GRAPH; }
digraph							{ printf("{%s}\n", yytext); return DIGRAPH; }

integer							{ printf("{%s}\n", yytext); return INTEGER_TYPE; }
any								{ printf("{%s}\n", yytext); return ANY_TYPE; }
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

{whitespace}					{ printf("{%s}\n", yytext); /* IgnoredPatternAction(yytext); */ }

.								{ printf("{%s}\n", yytext); return UnknownPatternAction(yytext); }

%%
