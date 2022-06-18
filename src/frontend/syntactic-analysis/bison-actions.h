#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../backend/support/shared.h"

/**
 * Se definen las acciones a ejecutar sobre cada regla de producción de la
 * gramática. El objetivo de cada acción debe ser el de construir el nodo
 * adecuado que almacene la información requerida en el árbol de sintaxis
 * abstracta (i.e., el AST).
 */

struct Program* newProgram(struct FunctionDefinitions* functionDefinitions, struct StartDefinition* startDefinition);

struct StartDefinition* newStartDefinition(struct Body* body);

struct FunctionDefinitions* newFunctionDefinitionsFromEmpty(void);
struct FunctionDefinitions* newFunctionDefinitionsFromList(struct FunctionDefinitions* functionDefinitions, struct FunctionDefinition* functionDefinition);

struct FunctionDefinition* newFunctionDefinition(struct DataType* dataType, char* identifier, struct ArgumentDefinition* argumentDefinition, struct Body* body);

struct ArgumentDefinition* newArgumentDefinitionFromEmpty(void);
struct ArgumentDefinition* newArgumentDefinitionFromArgumentList(struct ArgumentList* argumentList);

struct ArgumentList* newArgumentListFromVariableDefinition(struct VariableDefinition* variableDefinition);
struct ArgumentList* newArgumentListFromArgumentAndVariable(struct ArgumentList* argumentList, struct VariableDefinition* variableDefinition);

struct VariableDefinition* newVariableDefinition(struct DataType* dataType, char* identifier);

struct ForBlock* newForBlock(struct ForStatement* forStatement, struct Body* body);

struct ForStatement* newForStatement(char* identifier, struct Range* range);

struct Range* newRangeInclusive(struct Value* from, struct Value* to);
struct Range* newRangeExclusive(struct Value* from, struct Value* to);
struct Range* newRangeForEach(struct Value* range);
struct Range* newRangeFromGraph(char * identifier, struct ItType* itType, struct Value* graph);

struct ItType* newItType(int type);

struct WhileBlock* newWhileBlock(struct Condition* condition, struct Body* body);

struct IfBlock* newIfBlockFromIf(struct Condition* condition, struct Body* body);
struct IfBlock* newIfBlockFromIfElse(struct Condition* condition, struct Body* body, struct Body* elseBody);
struct IfBlock* newIfBlockFromIsElseIf(struct Condition* condition, struct Body* body, struct IfBlock* ifBlock);

struct Condition* newConditionFromBoolean(boolean boolean);
struct Condition* newConditionFromValueComparatorValue(struct Value* left, struct Comparator* comparator, struct Value* right);
struct Condition* newConditionFromConditionAndCondition(struct Condition* left, struct Condition* right);
struct Condition* newConditionFromConditionOrCondition(struct Condition* left, struct Condition* right);
struct Condition* newConditionFromNotCondition(struct Condition* condition);
struct Condition* newConditionFromIdentifierIsEmpty(char * identifier);
struct Condition* newConditionFromIdentifierIsNotEmpty(char * identifier);

struct Value* newValueFromIdentifier(char * identifier);
struct Value* newValueFromFunctionCall(struct FunctionCall* functionCall);
struct Value* newValueFromDataTypeInstance(struct DataTypeInstance* dataTypeInstance);
struct Value* newValueFromValueAddValue(struct Value* left, struct Value* right);
struct Value* newValueFromValueSubValue(struct Value* left, struct Value* right);
struct Value* newValueFromValueMulValue(struct Value* left, struct Value* right);
struct Value* newValueFromValueDivValue(struct Value* left, struct Value* right);
struct Value* newValueFromPopFunction(struct PopFunction* popFunction);
struct Value* newValueFromValueDotData(struct Value* value);
struct Value* newValueFromValueDotEdges(struct Value* edges);
struct Value* newValueFromValueDotNodes(struct Value* nodes);

struct FunctionCall* functionCall(char * identifier, struct ParamDefinition* paramDefinition);

struct ParamDefinition* newParamDefinitionEmpty(void);
struct ParamDefinition* newParamDefinitionFromParamList(struct ParamList* paramList);

struct ParamList* newParamListFromParamListCommaValue(struct ParamList* paramList, struct Value* value);
struct ParamList* newParamListFromValue(struct Value* value);

struct Comparator* newComparatorFromGEQ(int comparator);
struct Comparator* newComparatorFromLEQ(int comparator);
struct Comparator* newComparatorFromLT(int comparator);
struct Comparator* newComparatorFromGT(int comparator);
struct Comparator* newComparatorFromEQ(int comparator);

struct Body* newBody(struct Statements* statements);

struct Statements* newStatementsFromStatementsStatement(struct Statements* statements, struct Statement* statement);
struct Statements* newStatements(void);

struct Statement* newStatementEmpty(void);
struct Statement* newStatementFromForBlock(struct ForBlock* forBlock);
struct Statement* newStatementFromIfBlock(struct IfBlock* ifBlock);
struct Statement* newStatementFromWhileBlock(struct WhileBlock* whileBlock);
struct Statement* newStatementFromCreateStatement(struct CreateStatement* createStatement);
struct Statement* newStatementFromInsertStatement(struct InsertStatement* insertStatement);
struct Statement* newStatementFromReturnStatement(struct ReturnStatement* returnStatement);
struct Statement* newStatementFromLetBeStatement(struct LetBeStatement* letBeStatement);
struct Statement* newStatementFromAssignmentStatement(struct AssignmentStatement* assignmentStatement);
struct Statement* newStatementFromPrintStatement(struct PrintStatement* printStatement);
struct Statement* newStatementFromDumpStatement(struct DumpStatement* dumpStatement);

struct CreateStatement* newCreateStatementFromExtraDataType(struct ExtraDataType* extraDataType, char * identifier);
struct CreateStatement* newCreateStatementFromCollectionDataType(struct CollectionType* collectionType, char * identifier);

struct InsertStatement* newInsertStatement(char * identifier, struct DataType* dataType, struct Value* value);

struct ReturnStatement* newReturnStatementWithValue(struct Value* returnValue);
struct ReturnStatement* newReturnStatement(void);

struct LetBeStatement* newLetBeStatement(char * identifier, struct PrimitiveDataType* primitiveDataType);

struct AssignmentStatement* newAssignmentStatementFromIdentifier(char * identifier, struct Value* value);
struct AssignmentStatement* newAssignmentStatementFromIdentifierDotData(char * identifier, struct Value* value);

struct PopFunction* newPopFunction(char * identifier);

struct PrintStatement* newPrintStatement(struct Value* graph);

struct DumpStatement* newDumpStatement(struct Value* graph, struct Value* file);
struct DumpStatement* newDumpStatementAsDot(struct Value* graph, struct Value* file);

struct DataType* newDataTypeFromPrimitiveDataType(struct PrimitiveDataType* primitiveDataType);
struct DataType* newDataTypeFromExtraDataType(struct ExtraDataType* extraDataType);
struct DataType* newDataTypeFromCollectionType(struct CollectionType* collectionType);

struct PrimitiveDataType* newPrimitiveDataType(int token);

struct ExtraDataType* newExtraDataTypeFromNodeType(struct NodeType* nodeType);
struct ExtraDataType* newExtraDataTypeFromEdgeType(struct EdgeType* edgeType);

struct NodeType* newNodeType(PrimitiveDataType* primitiveDataType);

struct EdgeType* newEdgeType(struct PrimitiveDataType* primitiveDataType);

struct CollectionType* newCollectionTypeFromSetType(struct SetType* setType);
struct CollectionType* newCollectionTypeFromStackType(struct StackType* stackType);
struct CollectionType* newCollectionTypeFromQueueType(struct QueueType* queueType);
struct CollectionType* newCollectionTypeFromGraphType(struct GraphType* graphType);
struct CollectionType* newCollectionTypeFromDigraphType(struct DigraphType* digraphType);

struct SetType* newSetTypeFromPrimitiveDataType(struct PrimitiveDataType* primitiveDataType);
struct SetType* newSetTypeFromExtraDataType(struct ExtraDataType* extraDataType);

struct StackType* newStackTypeFromPrimitiveDataType(struct PrimitiveDataType* primitiveDataType);
struct StackType* newStackTypeFromExtraDataType(struct ExtraDataType* extraDataType);

struct QueueType* newQueueTypeFromPrimitiveDataType(struct PrimitiveDataType* primitiveDataType);
struct QueueType* newQueueTypeFromExtraDataType(struct ExtraDataType* extraDataType);

struct GraphType* newGraphType(struct PrimitiveDataType* nodeType, struct PrimitiveDataType* edgeType);

struct DigraphType* newDigraphType(struct PrimitiveDataType* nodeType, struct PrimitiveDataType* edgeType);

struct DataTypeInstance* newDataTypeInstanceFromPrimitiveDataTypeInstance(struct PrimitiveDataTypeInstance* primitiveDataType);
struct DataTypeInstance* newDataTypeInstanceFromExtraDataTypeInstance(struct ExtraDataTypeInstance* extraDataTypeInstance);

struct PrimitiveDataTypeInstance* newPrimitiveDataTypeInstanceFromString(char * string);
struct PrimitiveDataTypeInstance* newPrimitiveDataTypeInstanceFromLetter(char letter);
struct PrimitiveDataTypeInstance* newPrimitiveDataTypeInstanceFromInteger(int integer);
struct PrimitiveDataTypeInstance* newPrimitiveDataTypeInstanceFromDecimal(double decimal);
struct PrimitiveDataTypeInstance* newPrimitiveDataTypeInstanceFromBoolean(boolean boolean);

struct ExtraDataTypeInstance* newExtraDataTypeInstanceFromNode(struct NodeInstance* node);
struct ExtraDataTypeInstance* newExtraDataTypeInstanceFromEdge(struct EdgeInstance* edge);

struct NodeInstance* newNodeInstance(struct Value* label);
struct NodeInstance* newNodeInstanceWithValue(struct Value* label, struct Value* value);

struct EdgeInstance* newEdgeInstance(struct Value* left, struct Value* right);
struct EdgeInstance* newEdgeInstanceWithValue(struct Value* left, struct Value* right, struct Value* value);

#endif
