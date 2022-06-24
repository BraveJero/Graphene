#include <stdlib.h>

#include "ast.h"

static void freeStartDefinition(struct StartDefinition* startDefinition);
static void freeFunctionDefinitions(FunctionDefinitions* functionDefinitions);
static void freeFunctionDefinition(FunctionDefinition* functionDefinition);
static void freeArgumentDefinition(ArgumentDefinition* argumentDefinition);
static void freeArgumentList(ArgumentList* argumentList);
static void freeVariableDefinition(VariableDefinition* variableDefinition);
static void freeForBlock(ForBlock* forBlock);
static void freeForStatement(ForStatement* forStatement);
static void freeRange(Range* range);
static void freeItType(ItType* itType);
static void freeWhileBlock(WhileBlock* whileBlock);
static void freeIfBlock(IfBlock* ifBlock);
static void freeCondition(Condition* condition);
static void freeValue(Value* value);
static void freeFunctionCall(FunctionCall* functionCall);
static void freeParamDefinition(ParamDefinition* paramDefinition);
static void freeParamList(ParamList* paramList);
static void freeComparator(Comparator* comparator);
static void freeBody(Body* body);
static void freeStatements(Statements* statements);
static void freeStatement(Statement* statement);
static void freeCreateStatement(CreateStatement* createStatement);
static void freeInsertStatement(InsertStatement* insertStatement);
static void freeReturnStatement(ReturnStatement* rs);
static void freeLetBeStatement(LetBeStatement* lbs);
static void freeAssignmentStatement(AssignmentStatement* as);
static void freePopFunction(PopFunction* pf);
static void freePrintStatement(PrintStatement* ps);
static void freeDumpStatement(DumpStatement* ds);
static void freeDataType(DataType* dt);
static void freePrimitiveDataType(PrimitiveDataType* pdt);
static void freeExtraDataType(ExtraDataType* edt);
static void freeNodeType(NodeType* nt);
static void freeEdgeType(EdgeType* et);
static void freeCollectionType(CollectionType* ct);
static void freeSetType(SetType* st);
static void freeStackType(StackType* st);
static void freeQueueType(QueueType* qt);
static void freeGraphType(GraphType* gt);
static void freeDigraphType(DigraphType* dt);
static void freeDataTypeInstance(DataTypeInstance* dti);
static void freePrimitiveDataTypeInstance(PrimitiveDataTypeInstance* pdti);
static void freeExtraDataTypeInstance(ExtraDataTypeInstance* edti) ;
static void freeNodeInstance(NodeInstance* ni);
static void freeEdgeInstance(EdgeInstance* ei);

void freeProgram(struct Program* program) {
    if (program == NULL)    return;
    freeFunctionDefinitions(program->functionDefinitions);
    freeStartDefinition(program->startDefinition);
    free(program);
}

static void freeStartDefinition(struct StartDefinition* startDefinition) {
    if (startDefinition == NULL)    return;
    freeBody(startDefinition->body);
    free(startDefinition);
}

static void freeFunctionDefinitions(FunctionDefinitions* functionDefinitions) {
    if (functionDefinitions == NULL)    return;
    freeFunctionDefinitions(functionDefinitions->functionDefinitions);
    freeFunctionDefinition(functionDefinitions->functionDefinition);
    free(functionDefinitions);
}

static void freeFunctionDefinition(FunctionDefinition* functionDefinition) {
    if (functionDefinition == NULL)    return;
    freeDataType(functionDefinition->dataType);
    free(functionDefinition->identifier);
    freeArgumentDefinition(functionDefinition->argumentDefinition);
    freeBody(functionDefinition->body);
    free(functionDefinition);
}

static void freeArgumentDefinition(ArgumentDefinition* argumentDefinition) {
    if (argumentDefinition == NULL)    return;
    freeArgumentList(argumentDefinition->argumentList);
    free(argumentDefinition);
}

static void freeArgumentList(ArgumentList* argumentList) {
    if (argumentList == NULL)    return;
    freeVariableDefinition(argumentList->variableDefinition);
    freeArgumentList(argumentList->argumentList);
    free(argumentList);
}

static void freeVariableDefinition(VariableDefinition* variableDefinition) {
    if (variableDefinition == NULL)    return;
    freeDataType(variableDefinition->dataType);
    free(variableDefinition->identifier);
    free(variableDefinition);
}

static void freeForBlock(ForBlock* forBlock) {
    if (forBlock == NULL)    return;
    freeForStatement(forBlock->forStatement);
    freeBody(forBlock->body);
    free(forBlock);
}

static void freeForStatement(ForStatement* forStatement) {
    if (forStatement == NULL)    return;
    free(forStatement->identifier);
    freeRange(forStatement->range);
    free(forStatement);
}

static void freeRange(Range* range) {
    if (range == NULL)    return;
    freeValue(range->value);
    free(range->identifier);
    freeItType(range->itType);
    free(range);
}

static void freeItType(ItType* itType) {
    if (itType == NULL)    return;
    free(itType);
}

static void freeWhileBlock(WhileBlock* whileBlock) {
    if (whileBlock == NULL)    return;
    freeCondition(whileBlock->condition);
    freeBody(whileBlock->body);
    free(whileBlock);
}

static void freeIfBlock(IfBlock* ifBlock) {
    if (ifBlock == NULL)    return;
    freeCondition(ifBlock->condition);
    freeBody(ifBlock->body);
    freeBody(ifBlock->body);
    freeIfBlock(ifBlock->ifBlock);
    free(ifBlock);
}

static void freeCondition(Condition* condition) {
    if (condition == NULL)    return;
    freeValue(condition->leftV);
    freeValue(condition->rightV);
    freeComparator(condition->comparator);
    freeCondition(condition->leftC);
    freeCondition(condition->rightC);
    freeCondition(condition->condition);
    free(condition->identifier);
    free(condition);
}

static void freeValue(Value* value) {
    if (value == NULL)  return;
    free(value->identifier);
    freeFunctionCall(value->functionCall);
    freeDataTypeInstance(value->dataTypeInstance);
    freeValue(value->left);
    freeValue(value->right);
    freePopFunction(value->popFunction);
    freeValue(value->value);
    free(value);
}

static void freeFunctionCall(FunctionCall* functionCall) {
    if (functionCall == NULL)  return;
    free(functionCall->identifier);
    freeParamDefinition(functionCall->paramDefinition);
    free(functionCall);
}

static void freeParamDefinition(ParamDefinition* paramDefinition) {
    if (paramDefinition == NULL)  return;
    freeParamList(paramDefinition->paramList);
    free(paramDefinition);
}

static void freeParamList(ParamList* paramList) {
    freeParamList(paramList->paramList);
    freeValue(paramList->value);
    free(paramList);
}

static void freeComparator(Comparator* comparator) {
    if (comparator == NULL)  return;
    free(comparator);
}

static void freeBody(Body* body) {
    if (body == NULL)  return;
    freeStatements(body->statements);
    free(body);
}

static void freeStatements(Statements* statements) {
    if (statements == NULL)  return;
    freeStatements(statements->statements);
    freeStatement(statements->statement);
    free(statements);
}

static void freeStatement(Statement* statement) {
    if (statement == NULL)  return;
    freeForBlock(statement->forBlock);
    freeIfBlock(statement->ifBlock);
    freeWhileBlock(statement->whileBlock);
    freeCreateStatement(statement->createStatement);
    freeInsertStatement(statement->insertStatement);
    freeReturnStatement(statement->returnStatement);
    freeLetBeStatement(statement->letBeStatement);
    freeAssignmentStatement(statement->assignmentStatement);
    freePrintStatement(statement->printStatement);
    freeDumpStatement(statement->dumpStatement);
    free(statement);
}

static void freeCreateStatement(CreateStatement* createStatement) {
    if (createStatement == NULL)  return;
    freeExtraDataType(createStatement->extraDataType);
    freeCollectionType(createStatement->collectionType);
    free(createStatement->identifier);
    free(createStatement);
}

static void freeInsertStatement(InsertStatement* insertStatement) {
    if (insertStatement == NULL)  return;
    free(insertStatement->identifier);
    freeDataType(insertStatement->dataType);
    freeValue(insertStatement->value);
    free(insertStatement);
}

static void freeReturnStatement(ReturnStatement* rs) {
    if (rs == NULL)  return;
    freeValue(rs->returnValue);
    free(rs);
}

static void freeLetBeStatement(LetBeStatement* lbs) {
    if (lbs == NULL)  return;
    free(lbs->identifier);
    freePrimitiveDataType(lbs->primitiveDataType);
    free(lbs);
}

static void freeAssignmentStatement(AssignmentStatement* as) {
    if (as == NULL)  return;
    free(as->identifier);
    freeValue(as->value);
    free(as);
}

static void freePopFunction(PopFunction* pf) {
    if (pf == NULL)  return;
    free(pf->identifier);
    free(pf);
}

static void freePrintStatement(PrintStatement* ps) {
    if (ps == NULL)  return;
    freeValue(ps->graph);
    free(ps);
}

static void freeDumpStatement(DumpStatement* ds) {
    if (ds == NULL)  return;
    freeValue(ds->graph);
    freeValue(ds->file);
    free(ds);
}

static void freeDataType(DataType* dt) {
    if (dt == NULL)  return;
    freePrimitiveDataType(dt->primitiveDataType);
    freeExtraDataType(dt->extraDataType);
    freeCollectionType(dt->collectionType);
    free(dt);
}

static void freePrimitiveDataType(PrimitiveDataType* pdt) {
    if (pdt == NULL)  return;
    free(pdt);
}

static void freeExtraDataType(ExtraDataType* edt) {
    if (edt == NULL)  return;
    freeNodeType(edt->nodeType);
    freeEdgeType(edt->edgeType);
    free(edt);
}

static void freeNodeType(NodeType* nt) {
    if (nt == NULL)  return;
    freePrimitiveDataType(nt->primitiveDataType);
    free(nt);
}

static void freeEdgeType(EdgeType* et) {
    if (et  == NULL)  return;
    freePrimitiveDataType(et->primitiveDataType);
    free(et);
}

static void freeCollectionType(CollectionType* ct) {
    if (ct == NULL)  return;
    freeSetType(ct->setType);
    freeStackType(ct->stackType);
    freeQueueType(ct->queueType);
    freeGraphType(ct->graphType);
    freeDigraphType(ct->digraphType);
    free(ct);
}

static void freeSetType(SetType* st) {
    if (st == NULL)  return;
    freePrimitiveDataType(st->primitiveDataType);
    freeExtraDataType(st->extraDataType);
    free(st);
}

static void freeStackType(StackType* st) {
    if (st == NULL)  return;
    freePrimitiveDataType(st->primitiveDataType);
    freeExtraDataType(st->extraDataType);
    free(st);
}

static void freeQueueType(QueueType* qt) {
    if (qt == NULL)  return;
    freePrimitiveDataType(qt->primitiveDataType);
    freeExtraDataType(qt->extraDataType);
    free(qt);
}

static void freeGraphType(GraphType* gt) {
    if (gt == NULL)  return;
    freePrimitiveDataType(gt->nodeType);
    freePrimitiveDataType(gt->edgeType);
    free(gt);
}

static void freeDigraphType(DigraphType* dt) {
    if (dt == NULL)  return;
    freePrimitiveDataType(dt->nodeType);
    freePrimitiveDataType(dt->edgeType);
    free(dt);
}

static void freeDataTypeInstance(DataTypeInstance* dti) {
    if (dti == NULL)  return;
    freePrimitiveDataTypeInstance(dti->primitiveDataTypeInstance);
    freeExtraDataTypeInstance(dti->extraDataTypeInstance);
    free(dti);
}

static void freePrimitiveDataTypeInstance(PrimitiveDataTypeInstance* pdti) {
    if (pdti == NULL)  return;
    free(pdti->string);
    free(pdti);
}

static void freeExtraDataTypeInstance(ExtraDataTypeInstance* edti) {
    if (edti == NULL)  return;
    freeNodeInstance(edti->nodeInstance);
    freeEdgeInstance(edti->edgeInstance);
    free(edti);
}

static void freeNodeInstance(NodeInstance* ni) {
    if (ni == NULL)  return;
    freeValue(ni->label);
    freeValue(ni->value);
    free(ni);
}

static void freeEdgeInstance(EdgeInstance* ei) {
    if (ei == NULL)  return;
    freeValue(ei->left);
    freeValue(ei->right);
    freeValue(ei->value);
    free(ei);
}
