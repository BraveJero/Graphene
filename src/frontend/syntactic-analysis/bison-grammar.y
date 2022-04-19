%{

#include "bison-actions.h"

%}

// IDs de los tokens generados desde Flex:
%token ADD
%token SUB
%token MUL
%token DIV

%token START CREATE INSERT INTO FOR WHILE FROM IF ELSE TO INCLUSIVE EXCLUSIVE WITH VALUE DFS BFS IS NOT POP ENTRY LET BE
%token PRINT DEF LABEL GET IN EQUALS RETURN DUMP GRAPHVIZ_DOT
%token INTEGER STRING CHAR DECIMAL NODE EDGE GRAPH DIGRAPH EMPTY_TYPE QUEUE STACK SET 
%token INTEGER_TYPE ANY_TYPE DECIMAL_TYPE CHAR_TYPE STRING_TYPE
%token GT LT GEQ LEQ EQ AND OR
%token OPEN_PARENTHESIS CLOSE_PARENTHESIS OPEN_ANTILAMBDA CLOSE_ANTILAMBDA COMMA COLON DOT RIGHT_ARROW LEFT_ARROW IDENTIFIER EOL

// Reglas de asociatividad y precedencia (de menor a mayor):
%left ADD SUB
%left MUL DIV

%%

program: function_defs start_def						{  }
	;

start_def: START COLON body								{  }
	;

function_defs: 											{  }
	| function_defs function_def						{  }
	;

function_def: DEF IDENTIFIER argument_def COLON body	{  }
	;

argument_def: OPEN_PARENTHESIS CLOSE_PARENTHESIS				{  }
	| OPEN_PARENTHESIS argument_list CLOSE_PARENTHESIS			{  }
	;

argument_list: var_def											{  }
	| argument_list COMMA var_def								{  }
	;

var_def: data_type IDENTIFIER									{  }
	;

for_block: for_stmt COLON body
	;

for_stmt: FOR IDENTIFIER FROM INTEGER TO INTEGER
	| FOR IDENTIFIER FROM INTEGER TO INTEGER INCLUSIVE
	| FOR IDENTIFIER FROM INTEGER TO INTEGER EXCLUSIVE
	| FOR IDENTIFIER IN 
	;

while_block: WHILE condition COLON body
	;

if_block: IF condition COLON body
	| IF condition COLON body ELSE COLON body
	| IF condition COLON body ELSE if_block
	;

condition: value comparator value 
	| value comparator prim_data_type_instance
	| prim_data_type_instance comparator value
	| prim_data_type_instance comparator prim_data_type_instance
	| condition AND condition
	| condition OR condition
	;

value: IDENTIFIER
	| function_call
	| prim_data_type_instance
	| expression
	;

function_call: IDENTIFIER param_def
	;

param_def: OPEN_PARENTHESIS CLOSE_PARENTHESIS				{  }
	| OPEN_PARENTHESIS param_list CLOSE_PARENTHESIS			{  }
	;

param_list: value											{  }
	| param_list COMMA value								{  }
	;

comparator: GEQ
	| LEQ
	| GT
	| LT
	| EQ

body: INDENT stmts DEDENT															{  }
	;

stmts: 																			{  }
	| stmts stmt																{  }
	;

stmt: create_stmt EOL																{  }
	| insert_stmt EOL																	{  }
	;

create_stmt: CREATE extra_data_type IDENTIFIER									{  }
	;

insert_stmt: INSERT INTO IDENTIFIER VALUE							{ /* TODO: CHECK */ }
	;

let_be_stmt: LET prim_data_type IDENTIFIER
	| LET prim_data_type IDENTIFIER BE value

data_type: prim_data_type										{  }
	| extra_data_type											{  }
	;

prim_data_type: EMPTY_TYPE										{  }
	| CHAR_TYPE													{  }
	| STRING_TYPE												{  }
	| INTEGER_TYPE												{  }
	| DECIMAL_TYPE												{  }
	;

extra_data_type: node_type										{  }
	| edge_type													{  }
	| graph_type												{  }
	| digraph_type												{  }
	| set_type													{  }
	| stack_type												{  }
	| queue_type												{  }
	;

node_type: NODE OPEN_ANTILAMBDA prim_data_type CLOSE_ANTILAMBDA							{  }
	;

edge_type: EDGE OPEN_ANTILAMBDA prim_data_type CLOSE_ANTILAMBDA							{  }

graph_type: GRAPH OPEN_ANTILAMBDA prim_data_type COMMA prim_data_type CLOSE_ANTILAMBDA	{  }
	;

digraph_type: GRAPH OPEN_ANTILAMBDA prim_data_type COMMA prim_data_type CLOSE_ANTILAMBDA	{  }
	;

set_type: SET OPEN_ANTILAMBDA prim_data_type CLOSE_ANTILAMBDA							{  }
	;

stack_type: STACK OPEN_ANTILAMBDA prim_data_type CLOSE_ANTILAMBDA						{  }
	;

queue_type: QUEUE OPEN_ANTILAMBDA prim_data_type CLOSE_ANTILAMBDA						{  }
	;

prim_data_type_instance: string									{  }
	| CHAR														{  }
	| INTEGER													{  }
	| DECIMAL													{  }
	;

extra_data_type_instance: node_instance
	| edge_instance
	;

node_instance: OPEN_PARENTHESIS string CLOSE_PARENTHESIS
	| OPEN_PARENTHESIS string COMMA prim_data_type_instance CLOSE_PARENTHESIS
	;

edge_instance: OPEN_PARENTHESIS string RIGHT_ARROW string CLOSE_PARENTHESIS
	| OPEN_PARENTHESIS string RIGHT_ARROW string COMMA prim_data_type_instance CLOSE_PARENTHESIS
	;

string: STRING													{  }
	| STRING ADD CHAR											{  }
	| CHAR ADD STRING											{  }
	| STRING ADD STRING											{  }
	| STRING ADD INTEGER										{  }
	| INTEGER ADD STRING										{  }
	| STRING ADD DECIMAL										{  }
	| DECIMAL ADD STRING										{  }
	;

integer: 
	;

expression: expression ADD expression							{ /* $$ = AdditionExpressionGrammarAction($1, $3); */ }
	| expression SUB expression									{ /* $$ = SubtractionExpressionGrammarAction($1, $3); */ }
	| expression MUL expression									{ /* $$ = MultiplicationExpressionGrammarAction($1, $3); */ }
	| expression DIV expression									{ /* $$ = DivisionExpressionGrammarAction($1, $3); */ }
	| factor													{ /* $$ = FactorExpressionGrammarAction($1); */ }
	;

factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS			{ /* $$ = ExpressionFactorGrammarAction($2); */ }
	| constant													{ /* $$ = ConstantFactorGrammarAction($1); */ }
	;

constant: INTEGER												{ /* $$ = IntegerConstantGrammarAction($1); */ }
	| DECIMAL
	;

%%
