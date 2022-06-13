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
%token	DFS
%token	BFS
%token	IS
%token	NOT
%token	EMPTY_TYPE
%token	POP
%token	ENTRY
%token	PRINT
%token	DEF
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
%token	BOOLEAN_TYPE
%token	DECIMAL_TYPE
%token	CHAR_TYPE
%token	STRING_TYPE
%token	OPEN_ANTILAMBDA
%token	CLOSE_ANTILAMBDA
%token	COMMA
%token	COLON
%token	DOT
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
%left AND OR
%right NOT
%left DOT

// El simbolo inicial de la gramatica
%start program

%%

program: function_definitions start_definition								{ printf("GRAMMAR: program: f_definitions start_definition\n"); $$ = ProgramGrammarAction(0); }
	;

start_definition: START COLON EOL body									{ printf("GRAMMAR: start_definition: START COLON EOL body\n"); }
	;

function_definitions: 													{  }
	| function_definitions function_definition								{  }
	;

function_definition: DEF data_type IDENTIFIER argument_definition COLON EOL body			{ printf("GRAMMAR: functino_definition: DEF data_type IDENTIFIER argument_definition COLON BODY\n"); }
	;

argument_definition: OPEN_PARENTHESIS CLOSE_PARENTHESIS				{  }
	| OPEN_PARENTHESIS argument_list CLOSE_PARENTHESIS			{  }
	;

argument_list: variable_definition											{  }
	| argument_list COMMA variable_definition								{  }
	;

variable_definition: data_type IDENTIFIER									{  }
	;

for_block: for_statement COLON EOL body
	;

for_statement: FOR IDENTIFIER FROM value TO value
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

function_call: IDENTIFIER param_definition							{ printf("GRAMMAR: function_call IDENTIFIER param_definition\n"); }
	;

param_definition: OPEN_PARENTHESIS CLOSE_PARENTHESIS				{  }
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

body: INDENT statements DEDENT														{ printf("GRAMMAR: body: INDENT statements DEDENT\n"); }
	;

statements: 																			{ printf("GRAMMAR: statements: ''\n"); }
	| statements statement																{ printf("GRAMMAR: statements: statements statement\n"); }
	;

statement: EOL																		{ printf("GRAMMAR: statement: EOL\n"); }
	| for_block																	{ printf("GRAMMAR: statement: for_block\n"); }
	| if_block																	{ printf("GRAMMAR: statement: if_block\n"); }
	| while_block																	{ printf("GRAMMAR: statement: while_block\n"); }
	| create_statement EOL															{  }
	| insert_statement EOL															{  }
	| return_statement EOL															{ printf("GRAMMAR: statement: rtn_statement\n"); }
	| let_be_statement EOL															{ printf("GRAMMAR: statement: let_be_statement\n"); }
	| assignment_statement EOL														{ printf("GRAMMAR: statement: assignemnt_statement\n"); }
	| print_statement EOL															{ printf("GRAMMAR: statement: print_statement\n"); }
	| dump_statement EOL															{ printf("GRAMMAR: statement: dump_statement\n"); }
	;

create_statement: CREATE extra_data_type IDENTIFIER									{  }
	| CREATE collection_type IDENTIFIER
	;

insert_statement: INSERT INTO IDENTIFIER data_type value							{ printf("GRAMMAR: insert_statement: INSERT INTO IDENTIFIER data_type value\n"); }
	;

return_statement: RETURN value													{ printf("GRAMMAR: return_statement: RETURN value\n"); }
	| RETURN													{ printf("GRAMMAR: return_statement: RETURN\n"); }
	;

let_be_statement: LET IDENTIFIER BE primitive_data_type								{ printf("GRAMMAR: let_be_statement: LET IDENTIFIER BE prim_data_type\n"); }
	;

assignment_statement: IDENTIFIER LEFT_ARROW value				{ printf("GRAMMAR: assignment_statement: IDENTIFIER LEFT_ARROW value\n"); }
	| IDENTIFIER DOT DATA LEFT_ARROW value					{ printf("GRAMMAR: assignment_statement: IDENTIFIER DOT DATA LEFT_ARROW value\n"); }
	;

pop_func: POP FROM IDENTIFIER								{ printf("GRAMMAR: pop_func: POP FROM IDENTIFIER\n"); }
	;

print_statement: PRINT value										{ printf("GRAMMAR: print_statement: PRINT value\n"); }
	;

dump_statement: DUMP value IN value										{ printf("GRAMMAR: dump_statement: DUMP value IN value\n"); }
	| DUMP value IN value AS GRAPHVIZ_DOT							{ printf("GRAMMAR: dump_statement: DUMP value IN value AS GRAPHVIZ_DOT\n"); }
	;

data_type: primitive_data_type									{  }
	| extra_data_type											{  }
	| collection_type											{  }
	;

primitive_data_type: EMPTY_TYPE									{  }
	| CHAR_TYPE													{  }
	| STRING_TYPE												{  }
	| INTEGER_TYPE												{  }
	| DECIMAL_TYPE												{  }
	| BOOLEAN_TYPE												{  }
	;

extra_data_type: node_type										{  }
	| edge_type													{  }
	;

node_type: NODE																			{ printf("GRAMMAR: node_type: NODE\n"); }
	| NODE OPEN_ANTILAMBDA primitive_data_type CLOSE_ANTILAMBDA								{ printf("GRAMMAR: node_type: NODE OPEN_ANTILAMBDA prim_data_type CLOSE_ANTILAMBDA\n"); }
	;

edge_type: EDGE																			{ printf("GRAMMAR: edge_type: EDGE\n"); }
	| EDGE OPEN_ANTILAMBDA primitive_data_type CLOSE_ANTILAMBDA								{ printf("GRAMMAR: edge_type: EDGE OPEN_ANTILAMBDA prim_data_type CLOSE_ANTILAMBDA\n"); }
	;

collection_type: set_type
	| stack_type
	| queue_type
	| graph_type
	| digraph_type
	;

set_type: SET OPEN_ANTILAMBDA primitive_data_type CLOSE_ANTILAMBDA							{  }
	| SET OPEN_ANTILAMBDA extra_data_type CLOSE_ANTILAMBDA							{  }
	;

stack_type: STACK OPEN_ANTILAMBDA primitive_data_type CLOSE_ANTILAMBDA						{  }
	| STACK OPEN_ANTILAMBDA extra_data_type CLOSE_ANTILAMBDA						{  }
	;

queue_type: QUEUE OPEN_ANTILAMBDA primitive_data_type CLOSE_ANTILAMBDA						{  }
	| QUEUE OPEN_ANTILAMBDA extra_data_type CLOSE_ANTILAMBDA
	;

graph_type: GRAPH OPEN_ANTILAMBDA primitive_data_type COMMA primitive_data_type CLOSE_ANTILAMBDA	{  }
	| GRAPH																				{ printf("GRAMMAR: graph_type: GRAPH\n"); }
	;

digraph_type: DIGRAPH OPEN_ANTILAMBDA primitive_data_type COMMA primitive_data_type CLOSE_ANTILAMBDA	{  }
	| DIGRAPH																			{ printf("GRAMMAR: digraph_type: DIGRAPH\n"); }
	;

data_type_instance: primitive_data_type_instance												{ printf("GRAMMAR: data_type_instance: prim_data_type_instance\n"); }
	| extra_data_type_instance															{ printf("GRAMMAR: data_type_instance: extra_data_type\n"); }

primitive_data_type_instance: STRING									{ printf("GRAMMAR: prim_data_type: STRING\n"); }
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