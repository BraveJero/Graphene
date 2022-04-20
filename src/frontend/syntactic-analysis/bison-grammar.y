%{

#include "bison-actions.h"

%}

// IDs de los tokens generados desde Flex:
%token	ADD
%token	SUB
%token	MUL
%token	DIV

// Paréntesis
%token	OPEN_PARENTHESIS
%token	CLOSE_PARENTHESIS

// Tipos de dato
%token	INTEGER
%token	US_ASCII_TEXT
%token	START
%token	CREATE
%token	INSERT
%token	INTO
%token	FOR
%token	WHILE
%token	CHAR
%token	DECIMAL
%token	STRING
%token	BOOLEAN
%token	NODE
%token	EDGE
%token	GRAPH
%token	FROM
%token	IF
%token	ELSE
%token	TO
%token	INCLUSIVE
%token	EXCLUSIVE
%token	WITH
%token	VALUE
%token	DFS
%token	BFS
%token	IS
%token	NOT
%token	EMPTY_TYPE
%token	POP
%token	ENTRY
%token	PRINT
%token	DEF
%token	LABEL
%token	GET
%token	IN
%token	EQ
%token	LT
%token	GT
%token	GEQ
%token	LEQ
%token	AND
%token	OR
%token	RETURN
%token	DUMP
%token	GRAPHVIZ_DOT
%token	LET
%token	BE
%token	QUEUE
%token	STACK
%token	SET
%token	DIGRAPH
%token	INTEGER_TYPE
%token	ANY_TYPE
%token	BOOLEAN_TYPE
%token	DECIMAL_TYPE
%token	CHAR_TYPE
%token	STRING_TYPE
%token	OPEN_ANTILAMBDA
%token	CLOSE_ANTILAMBDA
%token	COMMA
%token	COLON
%token	DOT
%token	SEMICOLON
%token	LEFT_ARROW
%token	RIGHT_ARROW
%token	IDENTIFIER
%token	EOL
%token	INDENT
%token	DEDENT
%token	DATA
%token	EDGES
%token	NODES
%token	AS

// Reglas de asociatividad y precedencia (de menor a mayor):
%left ADD SUB
%left MUL DIV

%%

program: function_defs start_def								{ printf("GRAMMAR: program: f_defs start_def\n"); $$ = ProgramGrammarAction(0); }
	;

start_def: START COLON EOL body									{ printf("GRAMMAR: start_def: START COLON EOL body\n"); }
	;

function_defs: 													{  }
	| function_defs function_def								{  }
	;

function_def: DEF data_type IDENTIFIER argument_def COLON EOL body			{ printf("GRAMMAR: functino_def: DEF data_type IDENTIFIER argument_def COLON BODY\n"); }
	;

argument_def: OPEN_PARENTHESIS CLOSE_PARENTHESIS				{  }
	| OPEN_PARENTHESIS argument_list CLOSE_PARENTHESIS			{  }
	;

argument_list: var_def											{  }
	| argument_list COMMA var_def								{  }
	;

var_def: data_type IDENTIFIER									{  }
	;

for_block: for_stmt COLON EOL body
	;

for_stmt: FOR IDENTIFIER FROM value TO value
	| FOR IDENTIFIER FROM value TO value INCLUSIVE
	| FOR IDENTIFIER FROM value TO value EXCLUSIVE
	| FOR IDENTIFIER IN value
	| FOR IDENTIFIER IN IDENTIFIER WITH it_type ENTRY value	
	;

it_type: BFS												{ printf("GRAMMAR: it_type: BFS\n"); }
	| DFS														{ printf("GRAMMAR: it_type: DFS\n"); }
	;

while_block: WHILE condition COLON EOL body
	;

if_block: IF condition COLON EOL body
	| IF condition COLON EOL body ELSE COLON EOL body
	| IF condition COLON EOL body ELSE if_block
	;

condition: BOOLEAN											{ printf("GRAMMAR: condition: BOOLEAN\n"); }
	| value comparator value 								{ printf("GRAMMAR: condition: value comparator value\n"); }
	| condition AND condition								{ printf("GRAMMAR: condition: condition AND condition\n"); }
	| condition OR condition								{ printf("GRAMMAR: condition: condition OR condition\n"); }
	| NOT condition											{ printf("GRAMMAR: condition: NOT condition\n"); }
	| OPEN_PARENTHESIS condition CLOSE_PARENTHESIS			{ printf("GRAMMAR: condition: OPEN_PARENTHESIS condition CLOSE_PARENTHESIS\n"); }
	| IDENTIFIER IS EMPTY_TYPE								{ printf("GRAMMAR: condition: IDENTIFIER IS EMPTY_TYPE\n"); }
	| IDENTIFIER IS NOT EMPTY_TYPE							{ printf("GRAMMAR: condition: IDENTIFIER IS NOT EMPTY_TYPE\n"); }
	;

value: IDENTIFIER											{ printf("GRAMMAR: value: IDENTIFIER\n"); }
	| function_call											{ printf("GRAMMAR: value: function_call\n"); }
	| data_type_instance									{ printf("GRAMMAR: value: data_type_instance\n"); }
	| value ADD value										{ printf("GRAMMAR: value: value ADD value\n"); }
	| value SUB value										{ printf("GRAMMAR: value: value SUB value\n"); }
	| value MUL value										{ printf("GRAMMAR: value: value MUL value\n"); }
	| value DIV value										{ printf("GRAMMAR: value: value DIV value\n"); }
	| pop_func												{ printf("GRAMMAR: value: pop_func\n"); }
	| value DOT DATA										{ printf("GRAMMAR: value: value DOT DATA\n"); }
	| value DOT EDGES
	| value DOT NODES											
	;

function_call: IDENTIFIER param_def							{ printf("GRAMMAR: function_call IDENTIFIER param_def\n"); }
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
	;

body: INDENT stmts DEDENT														{ printf("GRAMMAR: body: INDENT stmts DEDENT\n"); }
	;

stmts: 																			{ printf("GRAMMAR: stmts: ''\n"); }
	| stmts stmt																{ printf("GRAMMAR: stmts: stmts stmt\n"); }
	;

stmt: EOL																		{ printf("GRAMMAR: stmt: EOL\n"); }
	| for_block																	{ printf("GRAMMAR: stmt: for_block\n"); }
	| if_block																	{ printf("GRAMMAR: stmt: if_block\n"); }
	| while_block																	{ printf("GRAMMAR: stmt: while_block\n"); }
	| create_stmt EOL															{  }
	| insert_stmt EOL															{  }
	| return_stmt EOL															{ printf("GRAMMAR: stmt: rtn_stmt\n"); }
	| let_be_stmt EOL															{ printf("GRAMMAR: stmt: let_be_stmt\n"); }
	| assignment_stmt EOL														{ printf("GRAMMAR: stmt: assignemnt_stmt\n"); }
	| print_stmt EOL															{ printf("GRAMMAR: stmt: print_stmt\n"); }
	| dump_stmt EOL															{ printf("GRAMMAR: stmt: dump_stmt\n"); }
	;

create_stmt: CREATE extra_data_type IDENTIFIER									{  }
	;

insert_stmt: INSERT INTO IDENTIFIER data_type value							{ printf("GRAMMAR: insert_stmt: INSERT INTO IDENTIFIER data_type value\n"); }
	;

return_stmt: RETURN value													{ printf("GRAMMAR: return_stmt: RETURN value\n"); }
	;

let_be_stmt: LET IDENTIFIER BE prim_data_type								{ printf("GRAMMAR: let_be_stmt: LET IDENTIFIER BE prim_data_type\n"); }
	;

assignment_stmt: IDENTIFIER LEFT_ARROW value				{ printf("GRAMMAR: assignment_stmt: IDENTIFIER LEFT_ARROW value\n"); }
	;

pop_func: POP FROM IDENTIFIER								{ printf("GRAMMAR: pop_func: POP FROM IDENTIFIER\n"); }
	;

print_stmt: PRINT value										{ printf("GRAMMAR: print_stmt: PRINT value\n"); }
	;

dump_stmt: DUMP value IN value										{ printf("GRAMMAR: dump_stmt: DUMP value IN value\n"); }
	| DUMP value IN value AS GRAPHVIZ_DOT							{ printf("GRAMMAR: dump_stmt: DUMP value IN value AS GRAPHVIZ_DOT\n"); }
	;

data_type: prim_data_type										{  }
	| extra_data_type											{  }
	;

prim_data_type: EMPTY_TYPE										{  }
	| CHAR_TYPE													{  }
	| STRING_TYPE												{  }
	| INTEGER_TYPE												{  }
	| DECIMAL_TYPE												{  }
	| BOOLEAN_TYPE												{  }
	;

extra_data_type: node_type										{  }
	| edge_type													{  }
	| graph_type												{  }
	| digraph_type												{  }
	| set_type													{  }
	| stack_type												{  }
	| queue_type												{  }
	;

node_type: NODE																			{ printf("GRAMMAR: node_type: NODE\n"); }
	| NODE OPEN_ANTILAMBDA prim_data_type CLOSE_ANTILAMBDA								{ printf("GRAMMAR: node_type: NODE OPEN_ANTILAMBDA prim_data_type CLOSE_ANTILAMBDA\n"); }
	;

edge_type: EDGE																			{ printf("GRAMMAR: edge_type: EDGE\n"); }
	| EDGE OPEN_ANTILAMBDA prim_data_type CLOSE_ANTILAMBDA								{ printf("GRAMMAR: edge_type: EDGE OPEN_ANTILAMBDA prim_data_type CLOSE_ANTILAMBDA\n"); }
	;

graph_type: GRAPH OPEN_ANTILAMBDA prim_data_type COMMA prim_data_type CLOSE_ANTILAMBDA	{  }
	| GRAPH																				{ printf("GRAMMAR: graph_type: GRAPH\n"); }
	;

digraph_type: DIGRAPH OPEN_ANTILAMBDA prim_data_type COMMA prim_data_type CLOSE_ANTILAMBDA	{  }
	| DIGRAPH																			{ printf("GRAMMAR: digraph_type: DIGRAPH\n"); }
	;

set_type: SET OPEN_ANTILAMBDA prim_data_type CLOSE_ANTILAMBDA							{  }
	;

stack_type: STACK OPEN_ANTILAMBDA prim_data_type CLOSE_ANTILAMBDA						{  }
	;

queue_type: QUEUE OPEN_ANTILAMBDA prim_data_type CLOSE_ANTILAMBDA						{  }
	;

data_type_instance: prim_data_type_instance												{ printf("GRAMMAR: data_type_instance: prim_data_type_instance\n"); }
	| extra_data_type_instance															{ printf("GRAMMAR: data_type_instance: extra_data_type\n"); }

prim_data_type_instance: STRING									{ printf("GRAMMAR: prim_data_type: STRING\n"); }
	| CHAR														{ printf("GRAMMAR: prim_data_type: CHAR\n"); }
	| INTEGER													{ printf("GRAMMAR: prim_data_type: INTEGER\n"); }
	| DECIMAL													{ printf("GRAMMAR: prim_data_type: DECIMAL\n"); }
	| BOOLEAN													{ printf("GRAMMAR: prim_data_type: BOOLEAN\n"); }
	;

extra_data_type_instance: node_instance
	| edge_instance
	;

node_instance: OPEN_PARENTHESIS value CLOSE_PARENTHESIS
	| OPEN_PARENTHESIS value COMMA value CLOSE_PARENTHESIS
	;

edge_instance: OPEN_PARENTHESIS value RIGHT_ARROW value CLOSE_PARENTHESIS
	| OPEN_PARENTHESIS value RIGHT_ARROW value COMMA value CLOSE_PARENTHESIS
	;
%%