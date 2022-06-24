%{

#include "../../backend/support/shared.h"
#include "bison-actions.h"

%}

// Tipos de dato utilizados en las variables semanticas ($$, $1, $2, etc.):
%union {
	// No terminales.
	struct Program* program;
	struct StartDefinition* startDefinition;
	struct FunctionDefinitions* functionDefinitions;
	struct FunctionDefinition* functionDefinition;
	struct ArgumentDefinition* argumentDefinition;
	struct ArgumentList* argumentList;
	struct VariableDefinition* variableDefinition;
	struct ForBlock* forBlock;
	struct ForStatement* forStatement;
	struct Range* range;
	struct ItType* itType;
	struct WhileBlock* whileBlock;
	struct IfBlock* ifBlock;
	struct Condition* condition;
	struct Value* value;
	struct FunctionCall* functionCall;
	struct ParamDefinition* paramDefinition;
	struct ParamList* paramList;
	struct Comparator* comparator;
	struct Body* body;
	struct Statements* statements;
	struct Statement* statement;
	struct CreateStatement* createStatement;
	struct InsertStatement* insertStatement;
	struct ReturnStatement* returnStatement;
	struct LetBeStatement* letBeStatement;
	struct AssignmentStatement* assignmentStatement;
	struct PopFunction* popFunction;
	struct PrintStatement* printStatement;
	struct DumpStatement* dumpStatement;
	struct DataType* dataType;
	struct PrimitiveDataType* primitiveDataType;
	struct ExtraDataType* extraDataType;
	struct NodeType* nodeType;
	struct EdgeType* edgeType;
	struct CollectionType* collectionType;
	struct SetType* setType;
	struct StackType* stackType;
	struct QueueType* queueType;
	struct GraphType* graphType;
	struct DigraphType* digraphType;
	struct DataTypeInstance* dataTypeInstance;
	struct PrimitiveDataTypeInstance* primitiveDataTypeInstance;
	struct ExtraDataTypeInstance* extraDataTypeInstance;
	struct NodeInstance* nodeInstance;
	struct EdgeInstance* edgeInstance;

	// Terminales
	yytoken_kind_t token;
	int integer;
	double decimal;
	char * string;
	int bool;
	char letter;
}

// IDs de los tokens generados desde Flex:
// Operadores
%token <token> ADD SUB MUL DIV
// Paréntesis
%token <token> OPEN_PARENTHESIS CLOSE_PARENTHESIS

%token <token> START CREATE INSERT INTO FOR WHILE
%token <token> NODE EDGE GRAPH
%token <token> FROM IF ELSE TO INCLUSIVE EXCLUSIVE WITH DFS BFS IS
%token <token> NOT EMPTY_TYPE POP ENTRY PRINT DEF IN EQ LT GT GEQ LEQ AND OR RETURN
%token <token> DUMP GRAPHVIZ_DOT LET BE
%token <token> QUEUE STACK SET DIGRAPH
%token <token> INTEGER_TYPE BOOLEAN_TYPE DECIMAL_TYPE CHAR_TYPE STRING_TYPE
%token <token> OPEN_ANTILAMBDA CLOSE_ANTILAMBDA COMMA COLON DOT LEFT_ARROW RIGHT_ARROW EOL INDENT DEDENT DATA
%token <token> EDGES NODES AS
// Tipos de dato
%token <integer> INTEGER
%token <letter> CHAR
%token <decimal> DECIMAL
%token <string> STRING IDENTIFIER
%token <bool> BOOLEAN

%type <program> program
%type <startDefinition> start_definition
%type <functionDefinitions> function_definitions
%type <functionDefinition> function_definition
%type <argumentDefinition> argument_definition
%type <argumentList> argument_list
%type <variableDefinition> variable_definition
%type <forBlock> for_block
%type <forStatement> for_statement
%type <range> range
%type <itType> it_type
%type <whileBlock> while_block
%type <ifBlock> if_block
%type <condition> condition
%type <value> value
%type <functionCall> function_call
%type <paramDefinition> param_definition
%type <paramList> param_list
%type <comparator> comparator
%type <body> body
%type <statements> statements
%type <statement> statement
%type <createStatement> create_statement
%type <insertStatement> insert_statement
%type <returnStatement> return_statement
%type <letBeStatement> let_be_statement
%type <assignmentStatement> assignment_statement
%type <popFunction> pop_function
%type <printStatement> print_statement
%type <dumpStatement> dump_statement
%type <dataType> data_type
%type <primitiveDataType> primitive_data_type
%type <extraDataType> extra_data_type
%type <nodeType> node_type
%type <edgeType> edge_type
%type <collectionType> collection_type
%type <setType> set_type
%type <stackType> stack_type
%type <queueType> queue_type
%type <graphType> graph_type
%type <digraphType> digraph_type
%type <dataTypeInstance> data_type_instance
%type <primitiveDataTypeInstance> primitive_data_type_instance
%type <extraDataTypeInstance> extra_data_type_instance
%type <nodeInstance> node_instance
%type <edgeInstance> edge_instance

// Reglas de asociatividad y precedencia (de menor a mayor):
%left ADD SUB
%left MUL DIV
%left AND OR
%right NOT
%left DOT

// El simbolo inicial de la gramatica
%start program

%%

program: function_definitions start_definition	{ $$ = newProgram($1, $2); }
	;

start_definition: START COLON EOL body	{ $$ = newStartDefinition($4); }
	;

function_definitions: function_definitions function_definition	{ $$ = newFunctionDefinitionsFromList($1, $2); }
	|															{ $$ = newFunctionDefinitionsFromEmpty(); }
	;

function_definition: DEF data_type IDENTIFIER argument_definition COLON EOL body	{ $$ = newFunctionDefinition($2, $3, $4, $7); }
	;

argument_definition: OPEN_PARENTHESIS CLOSE_PARENTHESIS	{ $$ = newArgumentDefinitionFromEmpty(); }
	| OPEN_PARENTHESIS argument_list CLOSE_PARENTHESIS	{ $$ = newArgumentDefinitionFromArgumentList($2); }
	;

argument_list: variable_definition				{ $$ = newArgumentListFromVariableDefinition($1); }
	| argument_list COMMA variable_definition	{ $$ = newArgumentListFromArgumentAndVariable($1, $3); }
	;

variable_definition: data_type IDENTIFIER	{ $$ = newVariableDefinition($1, $2); }
	;

for_block: for_statement COLON EOL body	{ $$ = newForBlock($1, $4); }
	;

for_statement: FOR IDENTIFIER range	{ $$ = newForStatement($2, $3); }
	;

range: FROM value TO value INCLUSIVE			{ $$ = newRangeInclusive($2, $4); }
	| FROM value TO value EXCLUSIVE				{ $$ = newRangeExclusive($2, $4); }
	| IN value									{ $$ = newRangeForEach($2); }
	| IN IDENTIFIER WITH it_type ENTRY value	{ $$ = newRangeFromGraph($2, $4, $6); }
	;

it_type: BFS	{ $$ = newItType($1); }
	| DFS		{ $$ = newItType($1); }
	;

while_block: WHILE condition COLON EOL body	{ $$ = newWhileBlock($2, $5); }
	;

if_block: IF condition COLON EOL body					{ $$ = newIfBlockFromIf($2, $5); }
	| IF condition COLON EOL body ELSE COLON EOL body	{ $$ = newIfBlockFromIfElse($2, $5, $9); }
	| IF condition COLON EOL body ELSE if_block			{ $$ = newIfBlockFromIsElseIf($2, $5, $7); }
	;

condition: BOOLEAN					{ $$ = newConditionFromBoolean($1); }
	| value comparator value		{ $$ = newConditionFromValueComparatorValue($1, $2, $3); }
	| condition AND condition		{ $$ = newConditionFromConditionAndCondition($1, $3); }
	| condition OR condition		{ $$ = newConditionFromConditionOrCondition($1, $3); }
	| NOT condition					{ $$ = newConditionFromNotCondition($2); }
	| IDENTIFIER IS EMPTY_TYPE		{ $$ = newConditionFromIdentifierIsEmpty($1); }
	| IDENTIFIER IS NOT EMPTY_TYPE	{ $$ = newConditionFromIdentifierIsNotEmpty($1); }
	;

value: IDENTIFIER			{ $$ = newValueFromIdentifier($1); }
	| function_call			{ $$ = newValueFromFunctionCall($1); }
	| data_type_instance	{ $$ = newValueFromDataTypeInstance($1); }
	| value ADD value		{ $$ = newValueFromValueAddValue($1, $3); }
	| value SUB value		{ $$ = newValueFromValueSubValue($1, $3); }
	| value MUL value		{ $$ = newValueFromValueMulValue($1, $3); }
	| value DIV value		{ $$ = newValueFromValueDivValue($1, $3); }
	| pop_function			{ $$ = newValueFromPopFunction($1); }
	| value DOT DATA		{ $$ = newValueFromValueDotData($1); }
	| value DOT EDGES		{ $$ = newValueFromValueDotEdges($1); }
	| value DOT NODES		{ $$ = newValueFromValueDotNodes($1); }
	;

function_call: IDENTIFIER param_definition	{ $$ = functionCall($1, $2); }
	;

param_definition: OPEN_PARENTHESIS CLOSE_PARENTHESIS	{ $$ = newParamDefinitionEmpty(); }
	| OPEN_PARENTHESIS param_list CLOSE_PARENTHESIS		{ $$ = newParamDefinitionFromParamList($2); }
	;

param_list: param_list COMMA value	{ $$ = newParamListFromParamListCommaValue($1, $3); }
	| value							{ $$ = newParamListFromValue($1); }
	;

comparator: GEQ	{ $$ = newComparatorFromGEQ($1); }
	| LEQ		{ $$ = newComparatorFromLEQ($1); }
	| GT		{ $$ = newComparatorFromGT($1); }
	| LT		{ $$ = newComparatorFromLT($1); }
	| EQ		{ $$ = newComparatorFromEQ($1); }
	;

body: INDENT statements DEDENT	{ $$ = newBody($2); }
	;

statements: statements statement	{ $$ = newStatementsFromStatementsStatement($1, $2); }
	|								{ $$ = newStatements(); }
	;

statement: EOL					{ $$ = newStatementEmpty(); }
	| for_block					{ $$ = newStatementFromForBlock($1); }
	| if_block					{ $$ = newStatementFromIfBlock($1); }
	| while_block				{ $$ = newStatementFromWhileBlock($1); }
	| create_statement EOL		{ $$ = newStatementFromCreateStatement($1); }
	| insert_statement EOL		{ $$ = newStatementFromInsertStatement($1); }
	| let_be_statement EOL		{ $$ = newStatementFromLetBeStatement($1); }
	| return_statement EOL		{ $$ = newStatementFromReturnStatement($1); }
	| assignment_statement EOL	{ $$ = newStatementFromAssignmentStatement($1); }
	| print_statement EOL		{ $$ = newStatementFromPrintStatement($1); }
	| dump_statement EOL		{ $$ = newStatementFromDumpStatement($1); }
	;

create_statement: CREATE extra_data_type IDENTIFIER	{ $$ = newCreateStatementFromExtraDataType($2, $3); }
	| CREATE collection_type IDENTIFIER				{ $$ = newCreateStatementFromCollectionDataType($2, $3); }
	;

insert_statement: INSERT INTO IDENTIFIER data_type value	{ $$ = newInsertStatement($3, $4, $5); }
	;

return_statement: RETURN value	{ $$ = newReturnStatementWithValue($2); }
	| RETURN					{ $$ = newReturnStatement(); }
	;

let_be_statement: LET IDENTIFIER BE primitive_data_type	{ $$ = newLetBeStatement($2, $4); }
	;

assignment_statement: IDENTIFIER LEFT_ARROW value	{ $$ = newAssignmentStatementFromIdentifier($1, $3); }
	| IDENTIFIER DOT DATA LEFT_ARROW value			{ $$ = newAssignmentStatementFromIdentifierDotData($1, $5); }
	;

pop_function: POP FROM IDENTIFIER	{ $$ = newPopFunction($3); }
	;

print_statement: PRINT value	{ $$ = newPrintStatement($2); }
	;

dump_statement: DUMP value IN value			{ $$ = newDumpStatement($2, $4); }
	| DUMP value IN value AS GRAPHVIZ_DOT	{ $$ = newDumpStatementAsDot($2, $4); }
	;

data_type: primitive_data_type	{ $$ = newDataTypeFromPrimitiveDataType($1); }
	| extra_data_type			{ $$ = newDataTypeFromExtraDataType($1); }
	| collection_type			{ $$ = newDataTypeFromCollectionType($1); }
	;

primitive_data_type: EMPTY_TYPE	{ $$ = newPrimitiveDataType($1); }
	| CHAR_TYPE					{ $$ = newPrimitiveDataType($1); }
	| STRING_TYPE				{ $$ = newPrimitiveDataType($1); }
	| INTEGER_TYPE				{ $$ = newPrimitiveDataType($1); }
	| DECIMAL_TYPE				{ $$ = newPrimitiveDataType($1); }
	| BOOLEAN_TYPE				{ $$ = newPrimitiveDataType($1); }
	;

extra_data_type: node_type	{ $$ = newExtraDataTypeFromNodeType($1); }
	| edge_type				{ $$ = newExtraDataTypeFromEdgeType($1); }
	;

node_type: NODE OPEN_ANTILAMBDA primitive_data_type CLOSE_ANTILAMBDA	{ $$ = newNodeType($3); }
	;

edge_type: EDGE OPEN_ANTILAMBDA primitive_data_type CLOSE_ANTILAMBDA	{ $$ = newEdgeType($3); }
	;

collection_type: set_type	{ $$ = newCollectionTypeFromSetType($1); }
	| stack_type			{ $$ = newCollectionTypeFromStackType($1); }
	| queue_type			{ $$ = newCollectionTypeFromQueueType($1); }
	| graph_type			{ $$ = newCollectionTypeFromGraphType($1); }
	| digraph_type			{ $$ = newCollectionTypeFromDigraphType($1); }
	;

set_type: SET OPEN_ANTILAMBDA primitive_data_type CLOSE_ANTILAMBDA	{ $$ = newSetTypeFromPrimitiveDataType($3); }
	| SET OPEN_ANTILAMBDA extra_data_type CLOSE_ANTILAMBDA			{ $$ = newSetTypeFromExtraDataType($3); }
	;

stack_type: STACK OPEN_ANTILAMBDA primitive_data_type CLOSE_ANTILAMBDA	{ $$ = newStackTypeFromPrimitiveDataType($3); }
	| STACK OPEN_ANTILAMBDA extra_data_type CLOSE_ANTILAMBDA			{ $$ = newStackTypeFromExtraDataType($3); }
	;

queue_type: QUEUE OPEN_ANTILAMBDA primitive_data_type CLOSE_ANTILAMBDA	{ $$ = newQueueTypeFromPrimitiveDataType($3); }
	| QUEUE OPEN_ANTILAMBDA extra_data_type CLOSE_ANTILAMBDA			{ $$ = newQueueTypeFromExtraDataType($3); }
	;

graph_type: GRAPH OPEN_ANTILAMBDA primitive_data_type COMMA primitive_data_type CLOSE_ANTILAMBDA	{ $$ = newGraphType($3, $5); }
	;

digraph_type: DIGRAPH OPEN_ANTILAMBDA primitive_data_type COMMA primitive_data_type CLOSE_ANTILAMBDA	{ $$ = newDigraphType($3, $5); }
	;

data_type_instance: primitive_data_type_instance	{ $$ = newDataTypeInstanceFromPrimitiveDataTypeInstance($1); }
	| extra_data_type_instance						{ $$ = newDataTypeInstanceFromExtraDataTypeInstance($1); }

primitive_data_type_instance: STRING	{ $$ = newPrimitiveDataTypeInstanceFromString($1); }
	| CHAR								{ $$ = newPrimitiveDataTypeInstanceFromLetter($1); }
	| INTEGER							{ $$ = newPrimitiveDataTypeInstanceFromInteger($1); }
	| DECIMAL							{ $$ = newPrimitiveDataTypeInstanceFromDecimal($1); }
	| BOOLEAN							{ $$ = newPrimitiveDataTypeInstanceFromBoolean($1); }
	;

extra_data_type_instance: node_instance	{ $$ = newExtraDataTypeInstanceFromNode($1); }
	| edge_instance						{ $$ = newExtraDataTypeInstanceFromEdge($1); }
	;

node_instance: OPEN_PARENTHESIS value CLOSE_PARENTHESIS		{ $$ = newNodeInstance($2); }
	| OPEN_PARENTHESIS value COMMA value CLOSE_PARENTHESIS	{ $$ = newNodeInstanceWithValue($2, $4); }
	;

edge_instance: OPEN_PARENTHESIS value RIGHT_ARROW value CLOSE_PARENTHESIS		{ $$ = newEdgeInstance($2, $4); }
	| OPEN_PARENTHESIS value RIGHT_ARROW value COMMA value CLOSE_PARENTHESIS	{ $$ = newEdgeInstanceWithValue($2, $4, $6); }
	;

%%
