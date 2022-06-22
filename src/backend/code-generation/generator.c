#include <stdio.h>
#include <stdbool.h>

#include "../support/logger.h"
#include "generator.h"

/**
 * Implementación de "generator.h".
 */

static bool generateProgram(CompilerState* state, Program* program, FILE* out);
static bool generateStartDefinition(CompilerState* state, StartDefinition* startDefinition, FILE* out);
static bool generateFunctionDefinitions(CompilerState* state, FunctionDefinitions* functionDefinitions, FILE* out);
static bool generateFunctionDefinition(CompilerState* state, FunctionDefinition* functionDefinition, FILE* out);
static bool generateArgumentDefinition(CompilerState* state, ArgumentDefinition* argumentDefinition, FILE* out);
static bool generateArgumentList(CompilerState* state, ArgumentList* argumentList, FILE* out);
static bool generateVariableDefinition(CompilerState* state, VariableDefinition* variableDefinition, FILE* out);
static bool generateForBlock(CompilerState* state, ForBlock* forBlock, FILE* out);
static bool generateForStatement(CompilerState* state, ForStatement* forStatement, FILE* out);
static bool generateRange(CompilerState* state, Range* range, FILE* out);
static bool generateItType(CompilerState* state, ItType* itType, FILE* out);
static bool generateWhileBlock(CompilerState* state, WhileBlock* whileBlock, FILE* out);
static bool generateIfBlock(CompilerState* state, IfBlock* ifBlock, FILE* out);
static bool generateCondition(CompilerState* state, Condition* condition, FILE* out);
static bool generateValue(CompilerState* state, Value* value, FILE* out);
static bool generateFunctionCall(CompilerState* state, FunctionCall* functionCall, FILE* out);
static bool generateParamDefinition(CompilerState* state, ParamDefinition* paramDefinition, FILE* out);
static bool generateParamList(CompilerState* state, ParamList* paramList, FILE* out);
static bool generateComparator(CompilerState* state, Comparator* comparator, FILE* out);
static bool generateBody(CompilerState* state, Body* body, FILE* out);
static bool generateStatements(CompilerState* state, Statements* statements, FILE* out);
static bool generateStatement(CompilerState* state, Statement* statement, FILE* out);
static bool generateCreateStatement(CompilerState* state, CreateStatement* createStatement, FILE* out);
static bool generateInsertStatement(CompilerState* state, InsertStatement* insertStatement, FILE* out);
static bool generateReturnStatement(CompilerState* state, ReturnStatement* rs, FILE* out);
static bool generateLetBeStatement(CompilerState* state, LetBeStatement* lbs, FILE* out);
static bool generateAssignmentStatement(CompilerState* state, AssignmentStatement* as, FILE* out);
static bool generatePopFunction(CompilerState* state, PopFunction* pf, FILE* out);
static bool generatePrintStatement(CompilerState* state, PrintStatement* ps, FILE* out);
static bool generateDumpStatement(CompilerState* state, DumpStatement* ds, FILE* out);
static bool generateDataType(CompilerState* state, DataType* dt, FILE* out);
static bool generatePrimitiveDataType(CompilerState* state, PrimitiveDataType* pdt, FILE* out);
static bool generateExtraDataType(CompilerState* state, ExtraDataType* edt, FILE* out);
static bool generateNodeType(CompilerState* state, NodeType* nt, FILE* out);
static bool generateEdgeType(CompilerState* state, EdgeType* et, FILE* out);
static bool generateCollectionType(CompilerState* state, CollectionType* ct, FILE* out);
static bool generateSetType(CompilerState* state, SetType* st, FILE* out);
static bool generateStackType(CompilerState* state, StackType* st, FILE* out);
static bool generateQueueType(CompilerState* state, QueueType* qt, FILE* out);
static bool generateGraphType(CompilerState* state, GraphType* gt, FILE* out);
static bool generateDigraphType(CompilerState* state, DigraphType* dt, FILE* out);
static bool generateDataTypeInstance(CompilerState* state, DataTypeInstance* dti, FILE* out);
static bool generatePrimitiveDataTypeInstance(CompilerState* state, PrimitiveDataTypeInstance* pdti, FILE* out);
static bool generateExtraDataTypeInstance(CompilerState* state, ExtraDataTypeInstance* edti, FILE* out);
static bool generateNodeInstance(CompilerState* state, NodeInstance* ni, FILE* out);
static bool generateEdgeInstance(CompilerState* state, EdgeInstance* ei, FILE* out);

bool Generator(CompilerState* state, const char * outputFileName) {
	FILE *out = fopen("temp", "w");
	
	bool success = generateProgram(state, state->program, out);
	fclose(out);
}

static bool generateProgram(CompilerState* state, Program* program, FILE* out) {
    pushContext(state->symbolTable); // global context
									 // no variables should be defined but maybe functions
	fprintf(out, "public class Program {\n");

    generateFunctionDefinitions(state, program->functionDefinitions, out);
    generateStartDefinition(state, program->startDefinition, out);

	fprintf(out, "}\n");
    popContext(state->symbolTable);
}

static bool generateStartDefinition(CompilerState* state, StartDefinition* startDefinition, FILE* out) {
    generateBody(state, startDefinition->body, out);
}

static bool generateFunctionDefinitions(CompilerState* state, FunctionDefinitions* functionDefinitions, FILE* out) {
    generateFunctionDefinitions(state, functionDefinitions->functionDefinitions, out);
    generateFunctionDefinition(state, functionDefinitions->functionDefinition, out);
}

static bool generateFunctionDefinition(CompilerState* state, FunctionDefinition* functionDefinition, FILE* out) {
    generateDataType(state, functionDefinition->dataType, out);
    generateArgumentDefinition(state, functionDefinition->argumentDefinition, out);
    generateBody(state, functionDefinition->body, out);
}

static bool generateArgumentDefinition(CompilerState* state, ArgumentDefinition* argumentDefinition, FILE* out) {
    generateArgumentList(state, argumentDefinition->argumentList, out);
}

static bool generateArgumentList(CompilerState* state, ArgumentList* argumentList, FILE* out) {
    generateVariableDefinition(state, argumentList->variableDefinition, out);
    generateArgumentList(state, argumentList->argumentList, out);
}

static bool generateVariableDefinition(CompilerState* state, VariableDefinition* variableDefinition, FILE* out) {
    generateDataType(state, variableDefinition->dataType, out);
}

static bool generateForBlock(CompilerState* state, ForBlock* forBlock, FILE* out) {
    generateForStatement(state, forBlock->forStatement, out);
    generateBody(state, forBlock->body, out);
}

static bool generateForStatement(CompilerState* state, ForStatement* forStatement, FILE* out) {
    generateRange(state, forStatement->range, out);
}

static bool generateRange(CompilerState* state, Range* range, FILE* out) {
    generateValue(state, range->value, out);
    generateItType(state, range->itType, out);
}

static bool generateItType(CompilerState* state, ItType* itType, FILE* out) {
}

static bool generateWhileBlock(CompilerState* state, WhileBlock* whileBlock, FILE* out) {
    generateCondition(state, whileBlock->condition, out);
    generateBody(state, whileBlock->body, out);
}

static bool generateIfBlock(CompilerState* state, IfBlock* ifBlock, FILE* out) {
    generateCondition(state, ifBlock->condition, out);
    generateBody(state, ifBlock->body, out);
    generateBody(state, ifBlock->body, out);
    generateIfBlock(state, ifBlock->ifBlock, out);
}

static bool generateCondition(CompilerState* state, Condition* condition, FILE* out) {
    generateValue(state, condition->leftV, out);
    generateValue(state, condition->rightV, out);
    generateComparator(state, condition->comparator, out);
    generateCondition(state, condition->leftC, out);
    generateCondition(state, condition->rightC, out);
    generateCondition(state, condition->condition, out);
}

static bool generateValue(CompilerState* state, Value* value, FILE* out) {
    generateFunctionCall(state, value->functionCall, out);
    generateDataTypeInstance(state, value->dataTypeInstance, out);
    generateValue(state, value->left, out);
    generateValue(state, value->right, out);
    generatePopFunction(state, value->popFunction, out);
    generateValue(state, value->value, out);
}

static bool generateFunctionCall(CompilerState* state, FunctionCall* functionCall, FILE* out) {
    generateParamDefinition(state, functionCall->paramDefinition, out);
}

static bool generateParamDefinition(CompilerState* state, ParamDefinition* paramDefinition, FILE* out) {
    generateParamList(state, paramDefinition->paramList, out);
}

static bool generateParamList(CompilerState* state, ParamList* paramList, FILE* out) {
    generateParamList(state, paramList->paramList, out);
    generateValue(state, paramList->value, out);
}

static bool generateComparator(CompilerState* state, Comparator* comparator, FILE* out) {
}

static bool generateBody(CompilerState* state, Body* body, FILE* out) {
    generateStatements(state, body->statements, out);
}

static bool generateStatements(CompilerState* state, Statements* statements, FILE* out) {
    generateStatements(state, statements->statements, out);
    generateStatement(state, statements->statement, out);
}

static bool generateStatement(CompilerState* state, Statement* statement, FILE* out) {
    generateForBlock(state, statement->forBlock, out);
    generateIfBlock(state, statement->ifBlock, out);
    generateWhileBlock(state, statement->whileBlock, out);
    generateCreateStatement(state, statement->createStatement, out);
    generateInsertStatement(state, statement->insertStatement, out);
    generateReturnStatement(state, statement->returnStatement, out);
    generateLetBeStatement(state, statement->letBeStatement, out);
    generateAssignmentStatement(state, statement->assignmentStatement, out);
    generatePrintStatement(state, statement->printStatement, out);
    generateDumpStatement(state, statement->dumpStatement, out);
}

static bool generateCreateStatement(CompilerState* state, CreateStatement* createStatement, FILE* out) {
    generateExtraDataType(state, createStatement->extraDataType, out);
    generateCollectionType(state, createStatement->collectionType, out);
}

static bool generateInsertStatement(CompilerState* state, InsertStatement* insertStatement, FILE* out) {
    generateDataType(state, insertStatement->dataType, out);
    generateValue(state, insertStatement->value, out);
}

static bool generateReturnStatement(CompilerState* state, ReturnStatement* rs, FILE* out) {
    generateValue(state, rs->returnValue, out);
}

static bool generateLetBeStatement(CompilerState* state, LetBeStatement* lbs, FILE* out) {
    generatePrimitiveDataType(state, lbs->primitiveDataType, out);
}

static bool generateAssignmentStatement(CompilerState* state, AssignmentStatement* as, FILE* out) {
    generateValue(state, as->value, out);
}

static bool generatePopFunction(CompilerState* state, PopFunction* pf, FILE* out) {
}

static bool generatePrintStatement(CompilerState* state, PrintStatement* ps, FILE* out) {
    generateValue(state, ps->graph, out);
}

static bool generateDumpStatement(CompilerState* state, DumpStatement* ds, FILE* out) {
    generateValue(state, ds->graph, out);
    generateValue(state, ds->file, out);
}

static bool generateDataType(CompilerState* state, DataType* dt, FILE* out) {
    generatePrimitiveDataType(state, dt->primitiveDataType, out);
    generateExtraDataType(state, dt->extraDataType, out);
    generateCollectionType(state, dt->collectionType, out);
}

static bool generatePrimitiveDataType(CompilerState* state, PrimitiveDataType* pdt, FILE* out) {
}

static bool generateExtraDataType(CompilerState* state, ExtraDataType* edt, FILE* out) {
    generateNodeType(state, edt->nodeType, out);
    generateEdgeType(state, edt->edgeType, out);
}

static bool generateNodeType(CompilerState* state, NodeType* nt, FILE* out) {
    generatePrimitiveDataType(state, nt->primitiveDataType, out);
}

static bool generateEdgeType(CompilerState* state, EdgeType* et, FILE* out) {
    generatePrimitiveDataType(state, et->primitiveDataType, out);
}

static bool generateCollectionType(CompilerState* state, CollectionType* ct, FILE* out) {
    generateSetType(state, ct->setType, out);
    generateStackType(state, ct->stackType, out);
    generateQueueType(state, ct->queueType, out);
    generateGraphType(state, ct->graphType, out);
    generateDigraphType(state, ct->digraphType, out);
}

static bool generateSetType(CompilerState* state, SetType* st, FILE* out) {
    generatePrimitiveDataType(state, st->primitiveDataType, out);
    generateExtraDataType(state, st->extraDataType, out);
}

static bool generateStackType(CompilerState* state, StackType* st, FILE* out) {
    generatePrimitiveDataType(state, st->primitiveDataType, out);
    generateExtraDataType(state, st->extraDataType, out);
}

static bool generateQueueType(CompilerState* state, QueueType* qt, FILE* out) {
    generatePrimitiveDataType(state, qt->primitiveDataType, out);
    generateExtraDataType(state, qt->extraDataType, out);
}

static bool generateGraphType(CompilerState* state, GraphType* gt, FILE* out) {
    generatePrimitiveDataType(state, gt->nodeType, out);
    generatePrimitiveDataType(state, gt->edgeType, out);
}

static bool generateDigraphType(CompilerState* state, DigraphType* dt, FILE* out) {
    generatePrimitiveDataType(state, dt->nodeType, out);
    generatePrimitiveDataType(state, dt->edgeType, out);
}

static bool generateDataTypeInstance(CompilerState* state, DataTypeInstance* dti, FILE* out) {
    generatePrimitiveDataTypeInstance(state, dti->primitiveDataTypeInstance, out);
    generateExtraDataTypeInstance(state, dti->extraDataTypeInstance, out);
}

static bool generatePrimitiveDataTypeInstance(CompilerState* state, PrimitiveDataTypeInstance* pdti, FILE* out) {
}

static bool generateExtraDataTypeInstance(CompilerState* state, ExtraDataTypeInstance* edti, FILE* out) {
    generateNodeInstance(state, edti->nodeInstance, out);
    generateEdgeInstance(state, edti->edgeInstance, out);
}

static bool generateNodeInstance(CompilerState* state, NodeInstance* ni, FILE* out) {
    generateValue(state, ni->label, out);
    generateValue(state, ni->value, out);
}

static bool generateEdgeInstance(CompilerState* state, EdgeInstance* ei, FILE* out) {
    generateValue(state, ei->left, out);
    generateValue(state, ei->right, out);
    generateValue(state, ei->value, out);
}

