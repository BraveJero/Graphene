#ifndef SHARED_HEADER
#define SHARED_HEADER

#include <stdio.h>

// Descriptor del archivo de entrada que utiliza Bison.
extern FILE * yyin;

// Descriptor del archivo de salida que utiliza Bison.
extern FILE * yyout;

// Variable global que contiene el número escaneado.
// extern int yylval;

// Variable global que contiene el número de la línea analizada.
extern int yylineno;

// Token actual en el tope de la pila del analizador Bison.
extern char * yytext;

// Función global de manejo de errores en Bison.
extern void yyerror(const char * string);

// Función global del analizador léxico Flex.
extern int yylex(void);

// Función global del analizador sintáctico Bison.
extern int yyparse(void);

// Emular tipo "boolean".
typedef enum {
	false = 0,
	true = 1
} boolean;

// Estado global de toda la aplicación.
typedef struct {

	// Indica si la compilación tuvo problemas hasta el momento:
	boolean succeed;

	// Indica el resultado de la compilación:
	int result;

	// Agregar una pila para almacenar tokens/nodos.
	// Agregar un nodo hacia la raíz del árbol de sintaxis abstracta.
	// Agregar una tabla de símbolos.
	// ...

} CompilerState;

// El estado se define e inicializa en el archivo "main.c":
extern CompilerState state;

// Program
typedef struct Program {
    struct FunctionDefinitions* functionDefinitions;
    struct StartDefinition* startDefinition;
} Program;

// StartDefinition
typedef struct StartDefinition {
    struct Body* body;
} StartDefinition;

// FunctionDefinitions
typedef enum FUNCTION_DEFINITIONS_TYPE {
    FUNCTION_DEFINITIONS_EMPTY,
    FUNCTION_DEFINITIONS_LIST,
} FUNCTION_DEFINITIONS_TYPE;

typedef struct FunctionDefinitions{
    FUNCTION_DEFINITIONS_TYPE functionDefinitionsType;
    struct FunctionDefinitions* functionDefinitions;
    struct FunctionDefinition* functionDefinition;
} FunctionDefinitions;

// FunctionDefinition
typedef struct FunctionDefinition {
    struct DataType* dataType;
    char* identifier;
    struct ArgumentDefinition* argumentDefinition;
    struct Body* body;
} FunctionDefinition;

// ArgumentDefinition
typedef enum ARGUMENT_DEFINITION_TYPE {
    ARGUMENT_DEFINITION_EMPTY,
    ARGUMENT_DEFINITION_LIST,
} ARGUMENT_DEFINITION_TYPE;

typedef struct ArgumentDefinition {
    ARGUMENT_DEFINITION_TYPE argumentDefinitionType;
    struct ArgumentList* argumentList;
} ArgumentDefinition;

// ArgumentList
typedef enum ARGUMENT_LIST_TYPE {
    ARGUMENT_LIST_VARIABLE_DEFINITION,
    ARGUMENT_LIST_LIST_COMMA_VARIABLE,
} ARGUMENT_LIST_TYPE;

typedef struct ArgumentList {
    ARGUMENT_LIST_TYPE argumentListType;
    struct VariableDefinition* variableDefinition;
    struct ArgumentList* argumentList;
} ArgumentList;

// VariableDefinition
typedef struct VariableDefinition {
    struct DataType* dataType;
    char* identifier;
} VariableDefinition;

// ForBlock
typedef struct ForBlock {
    struct ForStatement* forStatement;
    struct Body* body;
} ForBlock;

// ForStatement
typedef struct ForStatement {
    char * identifier;
    struct Range* range;
} ForStatement;

// Range
typedef enum RANGE_TYPE {
    RANGE_INCLUSIVE,
    RANGE_EXCLUSIVE,
    RANGE_EACH,
    RANGE_GRAPH,
} RANGE_TYPE;

typedef struct Range {
    RANGE_TYPE rangeType;
    struct Value* value, * limitValue;
    char * identifier;
    struct ItType* itType;
} Range;

// ItType
typedef struct ItType {
    int type;
} ItType;

// WhileBlock
typedef struct WhileBlock {
    struct Condition* condition;
    struct Body* body;
} WhileBlock;

// IfBlock
typedef enum IF_BLOCK_TYPE {
    IF_BLOCK_IF_CONDITION,
    IF_BLOCK_IF_ELSE,
    IF_BLOCK_IF_ELSE_IF,
} IF_BLOCK_TYPE;

typedef struct IfBlock {
    IF_BLOCK_TYPE ifBlockType;
    struct Condition* condition;
    struct Body* body, * elseBody;
    struct IfBlock* ifBlock;
} IfBlock;

// Condition
typedef enum CONDITION_TYPE {
    CONDITION_BOOLEAN,
    CONDITION_VALUE_COMPARATOR_VALUE,
    CONDITION_CONDITION_AND_CONDITION,
    CONDITION_CONDITION_OR_CONDITION,
    CONDITION_NOT_CONDITION,
    CONDITION_IDENTIFIER_EMPTY,
    CONDITION_IDENTIFIER_NOT_EMPTY,
} CONDITION_TYPE;

typedef struct Condition {
    CONDITION_TYPE conditionType;
    boolean boolean;
    struct Value * leftV, * rightV;
    struct Comparator* comparator;
    struct Condition * leftC, * rightC;
    struct Condition* condition;
    char * identifier;
} Condition;

// Value
typedef enum VALUE_TYPE {
    VALUE_IDENTIFIER,
    VALUE_FUNCTION_CALL,
    VALUE_DATA_TYPE_INSTANCE,
    VALUE_VALUE_ADD_VALUE,
    VALUE_VALUE_SUB_VALUE,
    VALUE_VALUE_MUL_VALUE,
    VALUE_VALUE_DIV_VALUE,
    VALUE_POP_FUNCTION,
    VALUE_VALUE_DOT_DATA,
    VALUE_VALUE_DOT_EDGES,
    VALUE_VALUE_DOT_NODES,
} VALUE_TYPE;

typedef struct Value {
    VALUE_TYPE valueType;
    char * identifier;
    struct FunctionCall* functionCall;
    struct DataTypeInstance* dataTypeInstance;
    struct Value * left, * right;
    struct PopFunction* popFunction;
    struct Value* value;
} Value;


// FunctionCall
typedef struct FunctionCall {
    char * identifier;
    struct ParamDefinition* paramDefinition;
} FunctionCall;

// ParamDefinition
typedef enum PARAM_DEFINITION_TYPE {
    PARAM_DEFINITION_EMPTY,
    PARAM_DEFINITION_PARAM_LIST,
} PARAM_DEFINITION_TYPE;

typedef struct ParamDefinition {
    PARAM_DEFINITION_TYPE paramDefinitionType;
    struct ParamList* paramList;
} ParamDefinition;

// ParamList
typedef enum PARAM_LIST_TYPE {
    PARAM_LIST_PARAM_LIST_COMMA_VALUE,
    PARAM_LIST_VALUE,
} PARAM_LIST_TYPE;

typedef struct ParamList {
    PARAM_LIST_TYPE paramListType;
    struct ParamList* paramList;
    struct Value* value;
} ParamList;

// Comparator
typedef enum COMPARATOR_TYPE {
    COMPARATOR_GEQ,
    COMPARATOR_LEQ,
    COMPARATOR_GT,
    COMPARATOR_LT,
    COMPARATOR_EQ,
} COMPARATOR_TYPE;

typedef struct Comparator {
    COMPARATOR_TYPE comparatorType;
    int comparator;
} Comparator;

// Body
typedef struct Body {
    struct Statements* statements;
} Body;

// Statements
typedef enum STATEMENTS_TYPE {
    STATEMENTS_EMPTY,
    STATEMENTS_STATEMENTS_STATEMENT,
} STATEMENTS_TYPE;

typedef struct Statements {
    STATEMENTS_TYPE statementsType;
    struct Statements* statements;
    struct Statement* statement;
} Statements;

// Statement
typedef enum STATEMENT_TYPE {
    STATEMENT_EMPTY,
    STATEMENT_FOR_BLOCK,
    STATEMENT_IF_BLOCK,
    STATEMENT_WHILE_BLOCK,
    STATEMENT_CREATE_STATEMENT,
    STATEMENT_INSERT_STATEMENT,
    STATEMENT_RETURN_STATEMENT,
    STATEMENT_LET_BE_STATEMENT,
    STATEMENT_ASSIGNMENT_STATEMENT,
    STATEMENT_PRINT_STATEMENT,
    STATEMENT_DUMP_STATAMENT,
} STATEMENT_TYPE;

typedef struct Statement {
    STATEMENT_TYPE statementType;
    struct ForBlock* forBlock;
    struct IfBlock* ifBlock;
    struct WhileBlock* whileBlock;
    struct CreateStatement* createStatement;
    struct InsertStatement* insertStatement;
    struct ReturnStatement* returnStatement;
    struct LetBeStatement* letBeStatement;
    struct AssignmentStatement* assignmentStatement;
    struct PrintStatement* printStatement;
    struct DumpStatement* dumpStatement;
} Statement;

// CreateStatement
typedef enum CREATE_STATEMENT_TYPE {
    CREATE_STATEMENT_EXTRA_DATA_TYPE,
    CREATE_STATEMENT_COLLECTION_TYPE,
} CREATE_STATEMENT_TYPE;

typedef struct CreateStatement {
    CREATE_STATEMENT_TYPE createStatementType;
    struct ExtraDataType* extraDataType;
    struct CollectionType* collectionType;
    char * identifier;
} CreateStatement;

// InsertStatement
typedef struct InsertStatement {
    char * identifier;
    struct DataType* dataType;
    struct Value* value;
} InsertStatement;

// ReturnStatement
typedef enum RETURN_STATEMENT_TYPE {
    RETURN_STATEMENT_VALUE,
    RETURN_STATEMENT_VOID,
} RETURN_STATEMENT_TYPE;

typedef struct ReturnStatement {
    RETURN_STATEMENT_TYPE returnStatementType;
    struct Value* returnValue;
} ReturnStatement;

// LetBeStatement
typedef struct LetBeStatement {
    char * identifier;
    struct PrimitiveDataType* primitiveDataType;
} LetBeStatement;

// AssignmentStatement
typedef enum ASSIGNMENT_STATEMENT_TYPE {
    ASSIGNMENT_STATEMENT_IDENTIFIER,
    ASSIGNMENT_STATEMENT_IDENTIFIER_DOT_DATA,
} ASSIGNMENT_STATEMENT_TYPE;

typedef struct AssignmentStatement {
    ASSIGNMENT_STATEMENT_TYPE assignmentStatementType;
    char * identifier;
    struct Value* value;
} AssignmentStatement;

// PopFunction
typedef struct PopFunction {
    char * identifier;
} PopFunction;

// PrintStatement
typedef struct PrintStatement {
    struct Value* graph;
} PrintStatement;

// DumpStatement
typedef enum DUMP_STATEMENT_TYPE {
    DUMP_STATEMENT,
    DUMP_STATEMENT_DOT,
} DUMP_STATEMENT_TYPE;

typedef struct DumpStatement {
    DUMP_STATEMENT_TYPE dumpStatementType;
    struct Value * graph, * file;
} DumpStatement;

// DataType
typedef enum DATA_TYPE_TYPE {
    DATA_TYPE_PRIMITIVE_DATA_TYPE,
    DATA_TYPE_EXTRA_DATA_TYPE,
    DATA_TYPE_COLLECTION_TYPE,
} DATA_TYPE_TYPE;

typedef struct DataType {
    DATA_TYPE_TYPE dataTypeType;
    struct PrimitiveDataType* primitiveDataType;
    struct ExtraDataType* extraDataType;
    struct CollectionType* collectionType;
} DataType;

// PrimitiveDataType
typedef enum PRIMITIVE_DATA_TYPE_TYPE {
    PRIMITIVE_DATA_TYPE_EMPTY_TYPE,
    PRIMITIVE_DATA_TYPE_CHAR_TYPE,
    PRIMITIVE_DATA_TYPE_STRING_TYPE,
    PRIMITIVE_DATA_TYPE_INTEGER_TYPE,
    PRIMITIVE_DATA_TYPE_DECIMAL_TYPE,
    PRIMITIVE_DATA_TYPE_BOOLEAN_TYPE,
} PRIMITIVE_DATA_TYPE_TYPE;

typedef struct PrimitiveDataType {
    int token;
} PrimitiveDataType;

// ExtraDataType
typedef enum EXTRA_DATA_TYPE_TYPE {
    EXTRA_DATA_TYPE_NODE_TYPE,
    EXTRA_DATA_TYPE_EDGE_TYPE,
} EXTRA_DATA_TYPE_TYPE;

typedef struct ExtraDataType {
    EXTRA_DATA_TYPE_TYPE extraDataTypeType;
    struct NodeType* nodeType;
    struct EdgeType* edgeType;
} ExtraDataType;

// NodeType
typedef struct NodeType {
    struct PrimitiveDataType* primitiveDataType;
} NodeType;

// EdgeType
typedef struct EdgeType {
    struct PrimitiveDataType* primitiveDataType;
} EdgeType;

// CollectionType
typedef enum COLLECTION_TYPE_TYPE {
    COLLECTION_TYPE_SET_TYPE,
    COLLECTION_TYPE_STACK_TYPE,
    COLLECTION_TYPE_QUEUE_TYPE,
    COLLECTION_TYPE_GRAPH_TYPE,
    COLLECTION_TYPE_DIGRAPH_TYPE,
} COLLECTION_TYPE_TYPE;

typedef struct CollectionType {
    COLLECTION_TYPE_TYPE collectionTypeType;
    struct SetType* setType;
    struct StackType* stackType;
    struct QueueType* queueType;
    struct GraphType* graphType;
    struct DigraphType* digraphType;
} CollectionType;

// SetType
typedef enum SET_TYPE_TYPE {
    SET_TYPE_PRIMITIVE_DATA_TYPE,
    SET_TYPE_EXTRA_DATA_TYPE,
} SET_TYPE_TYPE;

typedef struct SetType {
    SET_TYPE_TYPE setTypeType;
    struct PrimitiveDataType* primitiveDataType;
    struct ExtraDataType* extraDataType;
} SetType;

// StackType
typedef enum STACK_TYPE_TYPE {
    STACK_TYPE_PRIMITIVE_DATA_TYPE,
    STACK_TYPE_EXTRA_DATA_TYPE,
} STACK_TYPE_TYPE;

typedef struct StackType {
    STACK_TYPE_TYPE stackTypeType;
    struct PrimitiveDataType* primitiveDataType;
    struct ExtraDataType* extraDataType;
} StackType;

// QueueType
typedef enum QUEUE_TYPE_TYPE {
    QUEUE_TYPE_PRIMITIVE_DATA_TYPE,
    QUEUE_TYPE_EXTRA_DATA_TYPE,
} QUEUE_TYPE_TYPE;

typedef struct QueueType {
    QUEUE_TYPE_TYPE queueTypeType;
    struct PrimitiveDataType* primitiveDataType;
    struct ExtraDataType* extraDataType;
} QueueType;

// GraphType
typedef struct GraphType {
    struct PrimitiveDataType * nodeType, * edgeType;
} GraphType;

// DigraphType
typedef struct DigraphType {
    struct PrimitiveDataType * nodeType, * edgeType;
} DigraphType;

// DataTypeInstance
typedef enum DATA_TYPE_INSTANCE_TYPE {
    DATA_TYPE_INSTANCE_PRIMITIVE_DATA_TYPE_INSTANCE,
    DATA_TYPE_INSTANCE_EXTRA_DATA_TYPE_INSTANCE,
} DATA_TYPE_INSTANCE_TYPE;

typedef struct DataTypeInstance {
    DATA_TYPE_INSTANCE_TYPE dataTypeInstanceType;
    struct PrimitiveDataTypeInstance* primitiveDataTypeInstance;
    struct ExtraDataTypeInstance* extraDataTypeInstance;
} DataTypeInstance;

// PrimitiveDataTypeInstance
typedef enum PRIMITIVE_DATA_TYPE_INSTACE_TYPE {
    PRIMITIVE_DATA_TYPE_INSTACE_STRING,
    PRIMITIVE_DATA_TYPE_INSTACE_CHAR,
    PRIMITIVE_DATA_TYPE_INSTACE_INTEGER,
    PRIMITIVE_DATA_TYPE_INSTACE_DECIMAL,
    PRIMITIVE_DATA_TYPE_INSTACE_BOOLEAN,
} PRIMITIVE_DATA_TYPE_INSTACE_TYPE;

typedef struct PrimitiveDataTypeInstance {
    PRIMITIVE_DATA_TYPE_INSTACE_TYPE primitiveDataTypeInstanceType;
    char * string;
    char letter;
    int integer;
    double decimal;
    boolean boolean;
} PrimitiveDataTypeInstance;

// ExtraDataTypeInstance
typedef enum EXTRA_DATA_TYPE_INSTANCE_TYPE {
    EXTRA_DATA_TYPE_INSTANCE_NODE_INSTANCE,
    EXTRA_DATA_TYPE_INSTANCE_EDGE_INSTANCE,
} EXTRA_DATA_TYPE_INSTANCE_TYPE;

typedef struct ExtraDataTypeInstance {
    EXTRA_DATA_TYPE_INSTANCE_TYPE extraDataTypeInstanceType;
    struct NodeInstance* nodeInstance;
    struct EdgeInstance* edgeInstance;
} ExtraDataTypeInstance;

// NodeInstance
typedef enum NODE_INSTANCE_TYPE {
    NODE_INSTANCE,
    NODE_INSTANCE_WITH_VALUE,
} NODE_INSTANCE_TYPE;

typedef struct NodeInstance {
    NODE_INSTANCE_TYPE nodeInstaceType;
    struct Value * label, * value;
} NodeInstance;

// EdgeInstance
typedef enum EDGE_INSTANCE_TYPE {
    EDGE_INSTANCE,
    EDGE_INSTANCE_WITH_VALUE,
} EDGE_INSTANCE_TYPE;

typedef struct EdgeInstance {
    EDGE_INSTANCE_TYPE edgeInstanceType;
    struct Value * left, * right, * value;
} EdgeInstance;

#endif
