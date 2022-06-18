#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../../backend/support/shared.h"
#include "../../backend/support/logger.h"
#include "bison-actions.h"

/**
 * Implementación de "bison-grammar.h".
 */

void yyerror(const char * string) {
	LogError("Mensaje: '%s' debido a '%s' (linea %d).", string, yytext, yylineno);
	LogError("En ASCII es:");
	LogErrorRaw("\t");
	const int length = strlen(yytext);
	for (int i = 0; i < length; ++i) {
		LogErrorRaw("[%d]", yytext[i]);
	}
	LogErrorRaw("\n\n");
}

Program* newProgram(struct FunctionDefinitions* functionDefinitions, struct StartDefinition* startDefinition) {
    Program* p = malloc(sizeof(Program));
    p->functionDefinitions = functionDefinitions;
    p->startDefinition = startDefinition;
    state.succeed = true;
    return p;
}

// StartDefinition
StartDefinition* newStartDefinition(Body* body) {
    StartDefinition* sd = malloc(sizeof(StartDefinition));
    sd->body = body;
    return sd;
}

// FunctionDefinitions
FunctionDefinitions* newFunctionDefinitionsFromEmpty(void) {
    FunctionDefinitions* fd = malloc(sizeof(FunctionDefinitions));
    fd->functionDefinitionsType = FUNCTION_DEFINITIONS_EMPTY;
    fd->functionDefinitions = NULL;
    fd->functionDefinition = NULL;
    return fd;
}

FunctionDefinitions* newFunctionDefinitionsFromList(FunctionDefinitions* functionDefinitions, FunctionDefinition* functionDefinition) {
    FunctionDefinitions* fd = malloc(sizeof(FunctionDefinitions));
    fd->functionDefinitionsType = FUNCTION_DEFINITIONS_LIST;
    fd->functionDefinitions = functionDefinitions;
    fd->functionDefinition = functionDefinition;
    return fd;
}

// FunctionDefinition
FunctionDefinition* newFunctionDefinition(DataType* dataType, char* identifier, ArgumentDefinition* argumentDefinition, Body* body) {
    FunctionDefinition* fd = malloc(sizeof(FunctionDefinition));
    fd->dataType = dataType;
    fd->identifier = identifier;
    fd->argumentDefinition = argumentDefinition;
    fd->body = body;
    return fd;
}

// ArgumentDefinition
ArgumentDefinition* newArgumentDefinitionFromEmpty(void) {
    ArgumentDefinition* ad = malloc(sizeof(ArgumentDefinition));
    ad->argumentDefinitionType = ARGUMENT_DEFINITION_EMPTY;
    ad->argumentList = NULL;
    return ad;
}

ArgumentDefinition* newArgumentDefinitionFromArgumentList(ArgumentList* argumentList) {
    ArgumentDefinition* ad = malloc(sizeof(ArgumentDefinition));
    ad->argumentDefinitionType = ARGUMENT_DEFINITION_LIST;
    ad->argumentList = argumentList;
    return ad;
}

// ArgumentList
ArgumentList* newArgumentListFromVariableDefinition(VariableDefinition* variableDefinition) {
    ArgumentList* al = malloc(sizeof(ArgumentList));
    al->argumentListType = ARGUMENT_LIST_VARIABLE_DEFINITION;
    al->variableDefinition = variableDefinition;
    return al;
}

ArgumentList* newArgumentListFromArgumentAndVariable(ArgumentList* argumentList, VariableDefinition* variableDefinition) {
    ArgumentList* al = malloc(sizeof(ArgumentList));
    al->argumentListType = ARGUMENT_LIST_LIST_COMMA_VARIABLE;
    al->argumentList = argumentList;
    al->variableDefinition = variableDefinition;
    return al;
}

// VariableDefinition
VariableDefinition* newVariableDefinition(DataType* dataType, char* identifier) {
    VariableDefinition* vd = malloc(sizeof(VariableDefinition));
    vd->dataType = dataType;
    vd->identifier = identifier;
    return vd;
}

// ForBlock
ForBlock* newForBlock(ForStatement* forStatement, Body* body) {
    ForBlock* fb = malloc(sizeof(ForBlock));
    fb->forStatement = forStatement;
    fb->body = body;
    return fb;
}

// ForStatement
ForStatement* newForStatement(char* identifier, Range* range) {
    ForStatement* fs = malloc(sizeof(ForStatement));
    fs->identifier = identifier;
    fs->range = range;
    return fs;
}

// Range
Range* newRange(void) {
    Range* r = malloc(sizeof(Range));
    r->rangeType = 0;
    r->value = NULL;
    r->limitValue = NULL;
    r->identifier = NULL;
    r->itType = NULL;
    return r;
}

Range* newRangeInclusive(Value* from, Value* to) {
    Range* r = newRange();
    r->rangeType = RANGE_INCLUSIVE;
    r->value = from;
    r->limitValue = to;
    return r;
}

Range* newRangeExclusive(Value* from, Value* to) {
    Range* r = newRange();
    r->rangeType = RANGE_EXCLUSIVE;
    r->value = from;
    r->limitValue = to;
    return r;
}

Range* newRangeForEach(Value* range) {
    Range* r = newRange();
    r->rangeType = RANGE_EACH;
    r->value = range;
    return r;
}

Range* newRangeFromGraph(char * identifier, ItType* itType, Value* graph) {
    Range* r = newRange();
    r->rangeType = RANGE_GRAPH;
    r->identifier = identifier;
    r->itType = itType;
    r->value = graph;
    return r;
}

// ItType
ItType* newItType(int type) {
    ItType* it = malloc(sizeof(ItType));
    it->type = type;
    return it;
}

// WhileBlock
WhileBlock* newWhileBlock(Condition* condition, Body* body) {
    WhileBlock* wb = malloc(sizeof(WhileBlock));
    wb->condition = condition;
    wb->body = body;
    return wb;
}

// IfBlock
IfBlock* newIfBlockFromIf(Condition* condition, Body* body) {
    IfBlock* ib = malloc(sizeof(IfBlock));
    ib->ifBlockType = IF_BLOCK_IF_CONDITION;
    ib->condition = condition;
    ib->body = body;
    ib->elseBody = NULL;
    ib->ifBlock = NULL;
    return ib;
}

IfBlock* newIfBlockFromIfElse(Condition* condition, Body* body, Body* elseBody) {
    IfBlock* ib = malloc(sizeof(IfBlock));
    ib->ifBlockType = IF_BLOCK_IF_ELSE;
    ib->condition = condition;
    ib->body = body;
    ib->elseBody = elseBody;
    ib->ifBlock = NULL;
    return ib;
}

IfBlock* newIfBlockFromIsElseIf(Condition* condition, Body* body, IfBlock* ifBlock) {
    IfBlock* ib = malloc(sizeof(IfBlock));
    ib->ifBlockType = IF_BLOCK_IF_ELSE_IF;
    ib->condition = condition;
    ib->body = body;
    ib->elseBody = NULL;
    ib->ifBlock = ifBlock;
    return ib;
}

// Condition
static Condition* newCondition(void) {
    Condition* c = malloc(sizeof(Condition));
    c->conditionType = 0;
    c->boolean = 0;
    c->leftV = NULL;
    c->rightV = NULL;
    c->comparator = NULL;
    c->leftC = NULL;
    c->rightC = NULL;
    c->condition = NULL;
    c->identifier = NULL;
    return c;
}

Condition* newConditionFromBoolean(boolean boolean) {
    Condition* c = newCondition();
    c->conditionType = CONDITION_BOOLEAN;
    c->boolean = boolean;
    return c;
}

Condition* newConditionFromValueComparatorValue(Value* left, Comparator* comparator, Value* right) {
    Condition* c = newCondition();
    c->conditionType = CONDITION_VALUE_COMPARATOR_VALUE;
    c->leftV = left;
    c->rightV = right;
    return c;
}

Condition* newConditionFromConditionAndCondition(Condition* left, Condition* right) {
    Condition* c = newCondition();
    c->conditionType = CONDITION_CONDITION_AND_CONDITION;
    c->leftC = left;
    c->rightC = right;
    return c;
}

Condition* newConditionFromConditionOrCondition(Condition* left, Condition* right) {
    Condition* c = newCondition();
    c->conditionType = CONDITION_CONDITION_OR_CONDITION;
    c->leftC = left;
    c->rightC = right;
    return c;
}

Condition* newConditionFromNotCondition(Condition* condition) {
    Condition* c = newCondition();
    c->conditionType = CONDITION_NOT_CONDITION;
    c->condition = condition;
    return c;
}

Condition* newConditionFromIdentifierIsEmpty(char * identifier) {
    Condition* c = newCondition();
    c->conditionType = CONDITION_IDENTIFIER_EMPTY;
    c->identifier = identifier;
    return c;
}

Condition* newConditionFromIdentifierIsNotEmpty(char * identifier) {
    Condition* c = newCondition();
    c->conditionType = CONDITION_IDENTIFIER_NOT_EMPTY;
    c->identifier = identifier;
    return c;
}

// Value
static Value* newValue(void) {
    Value* v = malloc(sizeof(Value));
    v->valueType = 0;
    v->identifier = NULL;
    v->functionCall = NULL;
    v->dataTypeInstance = NULL;
    v->left = NULL;
    v->right = NULL;
    v->popFunction = NULL;
    return v;
}

Value* newValueFromIdentifier(char * identifier) {
    Value* v = newValue();
    v->valueType = VALUE_IDENTIFIER;
    v->identifier = identifier;
    return v;
}

Value* newValueFromFunctionCall(FunctionCall* functionCall) {
    Value* v = newValue();
    v->valueType = VALUE_FUNCTION_CALL;
    v->functionCall = functionCall;
    return v;
}

Value* newValueFromDataTypeInstance(DataTypeInstance* dataTypeInstance) {
    Value* v = newValue();
    v->valueType = VALUE_DATA_TYPE_INSTANCE;
    v->dataTypeInstance = dataTypeInstance;
    return v;
}

Value* newValueFromValueAddValue(Value* left, Value* right) {
    Value* v = newValue();
    v->valueType = VALUE_VALUE_ADD_VALUE;
    v->left = left;
    v->right = right;
    return v;
}

Value* newValueFromValueSubValue(Value* left, Value* right) {
    Value* v = newValue();
    v->valueType = VALUE_VALUE_SUB_VALUE;
    v->left = left;
    v->right = right;
    return v;
}

Value* newValueFromValueMulValue(Value* left, Value* right) {
    Value* v = newValue();
    v->valueType = VALUE_VALUE_MUL_VALUE;
    v->left = left;
    v->right = right;
    return v;
}

Value* newValueFromValueDivValue(Value* left, Value* right) {
    Value* v = newValue();
    v->valueType = VALUE_VALUE_DIV_VALUE;
    v->left = left;
    v->right = right;
    return v;
}

Value* newValueFromPopFunction(PopFunction* popFunction) {
    Value* v = newValue();
    v->valueType = VALUE_POP_FUNCTION;
    v->popFunction = popFunction;
    return v;
}

Value* newValueFromValueDotData(Value* value) {
    Value* v = newValue();
    v->valueType = VALUE_VALUE_DOT_DATA;
    v->value = value;
    return v;
}

Value* newValueFromValueDotEdges(Value* value) {
    Value* v = newValue();
    v->valueType = VALUE_VALUE_DOT_EDGES;
    v->value = value;
    return v;
}

Value* newValueFromValueDotNodes(Value* value) {
    Value* v = newValue();
    v->valueType = VALUE_VALUE_DOT_NODES;
    v->value = value;
    return v;
}

// FunctionCall
FunctionCall* functionCall(char * identifier, ParamDefinition* paramDefinition) {
    FunctionCall* fc = malloc(sizeof(FunctionCall));
    fc->identifier = identifier;
    fc->paramDefinition = paramDefinition;
    return fc;
}

// ParamDefinition
ParamDefinition* newParamDefinitionEmpty(void) {
    ParamDefinition* pd = malloc(sizeof(ParamDefinition));
    pd->paramDefinitionType = PARAM_DEFINITION_EMPTY;
    pd->paramList = NULL;
    return pd;
}

ParamDefinition* newParamDefinitionFromParamList(ParamList* paramList) {
    ParamDefinition* pd = malloc(sizeof(ParamDefinition));
    pd->paramDefinitionType = PARAM_DEFINITION_PARAM_LIST;
    pd->paramList = paramList;
    return pd;
}

// ParamList
ParamList* newParamListFromParamListCommaValue(ParamList* paramList, Value* value) {
    ParamList* pl = malloc(sizeof(ParamList));
    pl->paramListType = PARAM_LIST_PARAM_LIST_COMMA_VALUE;
    pl->paramList = paramList;
    pl->value = value;
    return pl;
}

ParamList* newParamListFromValue(Value* value) {
    ParamList* pl = malloc(sizeof(ParamList));
    pl->paramListType = PARAM_LIST_VALUE;
    pl->paramList = NULL;
    pl->value = value;
    return pl;
}

// Comparator
Comparator* newComparatorFromGEQ(int comparator) {
    Comparator* c = malloc(sizeof(Comparator));
    c->comparatorType = COMPARATOR_GEQ;
    c->comparator = comparator;
    return c;
}

Comparator* newComparatorFromLEQ(int comparator) {
    Comparator* c = malloc(sizeof(Comparator));
    c->comparatorType = COMPARATOR_LEQ;
    c->comparator = comparator;
    return c;
}

Comparator* newComparatorFromLT(int comparator) {
    Comparator* c = malloc(sizeof(Comparator));
    c->comparatorType = COMPARATOR_LT;
    c->comparator = comparator;
    return c;
}

Comparator* newComparatorFromGT(int comparator) {
    Comparator* c = malloc(sizeof(Comparator));
    c->comparatorType = COMPARATOR_GT;
    c->comparator = comparator;
    return c;
}

Comparator* newComparatorFromEQ(int comparator) {
    Comparator* c = malloc(sizeof(Comparator));
    c->comparatorType = COMPARATOR_EQ;
    c->comparator = comparator;
    return c;
}

// Body
Body* newBody(Statements* statements) {
    Body* b = malloc(sizeof(Body));
    b->statements = statements;
    return b;
}

// Statements
Statements* newStatementsFromStatementsStatement(Statements* statements, Statement* statement) {
    Statements* s = malloc(sizeof(Statements));
    s->statementsType = STATEMENTS_STATEMENTS_STATEMENT;
    s->statements = statements;
    s->statement = statement;
    return s;
}

Statements* newStatements(void) {
    Statements* s = malloc(sizeof(Statements));
    s->statementsType = STATEMENTS_EMPTY;
    s->statements = NULL;
    s->statement = NULL;
    return s;
}

// Statement
Statement* newStatementEmpty(void) {
    Statement* s = malloc(sizeof(Statement));
    s->statementType = STATEMENT_EMPTY;
    s->forBlock = NULL;
    s->ifBlock = NULL;
    s->whileBlock = NULL;
    s->createStatement = NULL;
    s->insertStatement = NULL;
    s->returnStatement = NULL;
    s->letBeStatement = NULL;
    s->assignmentStatement = NULL;
    s->printStatement = NULL;
    s->dumpStatement = NULL;
    return s;
}

Statement* newStatementFromForBlock(ForBlock* forBlock) {
    Statement* s = newStatementEmpty();
    s->statementType = STATEMENT_FOR_BLOCK;
    s->forBlock = forBlock;
    return s;
}

Statement* newStatementFromIfBlock(IfBlock* ifBlock) {
    Statement* s = newStatementEmpty();
    s->statementType = STATEMENT_IF_BLOCK;
    s->ifBlock = ifBlock;
    return s;
}

Statement* newStatementFromWhileBlock(WhileBlock* whileBlock) {
    Statement* s = newStatementEmpty();
    s->statementType = STATEMENT_WHILE_BLOCK;
    s->whileBlock = whileBlock;
    return s;
}

Statement* newStatementFromCreateStatement(CreateStatement* createStatement) {
    Statement* s = newStatementEmpty();
    s->statementType = STATEMENT_CREATE_STATEMENT;
    s->createStatement = createStatement;
    return s;
}

Statement* newStatementFromInsertStatement(InsertStatement* insertStatement) {
    Statement* s = newStatementEmpty();
    s->statementType = STATEMENT_INSERT_STATEMENT;
    s->insertStatement = insertStatement;
    return s;
}

Statement* newStatementFromReturnStatement(ReturnStatement* returnStatement) {
    Statement* s = newStatementEmpty();
    s->statementType = STATEMENT_RETURN_STATEMENT;
    s->returnStatement = returnStatement;
    return s;
}

Statement* newStatementFromLetBeStatement(LetBeStatement* letBeStatement) {
    Statement* s = newStatementEmpty();
    s->statementType = STATEMENT_LET_BE_STATEMENT;
    s->letBeStatement = letBeStatement;
    return s;
}

Statement* newStatementFromAssignmentStatement(AssignmentStatement* assignmentStatement) {
    Statement* s = newStatementEmpty();
    s->statementType = STATEMENT_ASSIGNMENT_STATEMENT;
    s->assignmentStatement = assignmentStatement;
    return s;
}

Statement* newStatementFromPrintStatement(PrintStatement* printStatement) {
    Statement* s = newStatementEmpty();
    s->statementType = STATEMENT_PRINT_STATEMENT;
    s->printStatement = printStatement;
    return s;
}

Statement* newStatementFromDumpStatement(DumpStatement* dumpStatement) {
    Statement* s = newStatementEmpty();
    s->statementType = STATEMENT_DUMP_STATAMENT;
    s->dumpStatement = dumpStatement;
    return s;
}

// CreateStatement
CreateStatement* newCreateStatementFromExtraDataType(ExtraDataType* extraDataType, char * identifier) {
    CreateStatement* cs = malloc(sizeof(CreateStatement));
    cs->createStatementType = CREATE_STATEMENT_EXTRA_DATA_TYPE;
    cs->extraDataType = extraDataType;
    cs->collectionType = NULL;
	cs->identifier = identifier;
    return cs;
}

CreateStatement* newCreateStatementFromCollectionDataType(CollectionType* collectionType, char * identifier) {
    CreateStatement* cs = malloc(sizeof(CreateStatement));
    cs->createStatementType = CREATE_STATEMENT_COLLECTION_TYPE;
    cs->extraDataType = NULL;
    cs->collectionType = collectionType;
	cs->identifier = identifier;
    return cs;
}
// InsertStatement
InsertStatement* newInsertStatement(char * identifier, DataType* dataType, Value* value) {
    InsertStatement* is = malloc(sizeof(InsertStatement));
    is->identifier = identifier;
    is->dataType = dataType;
    is->value = value;
    return is;
}

// ReturnStatement
ReturnStatement* newReturnStatementWithValue(Value* returnValue) {
    ReturnStatement* rs = malloc(sizeof(ReturnStatement));
    rs->returnStatementType = RETURN_STATEMENT_VALUE;
    rs->returnValue = returnValue;
    return rs;
}

ReturnStatement* newReturnStatement(void) {
    ReturnStatement* rs = malloc(sizeof(ReturnStatement));
    rs->returnStatementType = RETURN_STATEMENT_VOID;
    rs->returnValue = NULL;
    return rs;
}

// LetBeStatement
LetBeStatement* newLetBeStatement(char * identifier, PrimitiveDataType* primitiveDataType) {
    LetBeStatement* lbs = malloc(sizeof(LetBeStatement));
    lbs->identifier = identifier;
    lbs->primitiveDataType = primitiveDataType;
    return lbs;
}

// AssignmentStatement
AssignmentStatement* newAssignmentStatementFromIdentifier(char * identifier, Value* value) {
    AssignmentStatement* as = malloc(sizeof(AssignmentStatement));
    as->assignmentStatementType = ASSIGNMENT_STATEMENT_IDENTIFIER;
    as->identifier = identifier;
    as->value = value;
    return as;
}

AssignmentStatement* newAssignmentStatementFromIdentifierDotData(char * identifier, Value* value) {
    AssignmentStatement* as = malloc(sizeof(AssignmentStatement));
    as->assignmentStatementType = ASSIGNMENT_STATEMENT_IDENTIFIER_DOT_DATA;
    as->identifier = identifier;
    as->value = value;
    return as;
}

// PopFunction
PopFunction* newPopFunction(char * identifier) {
    PopFunction* pf = malloc(sizeof(PopFunction));
    pf->identifier = identifier;
    return pf;
}

// PrintStatement
PrintStatement* newPrintStatement(Value* graph) {
    PrintStatement* ps = malloc(sizeof(PrintStatement));
    ps->graph = graph;
    return ps;
}

// DumpStatement
DumpStatement* newDumpStatement(Value* graph, Value* file) {
    DumpStatement* ds = malloc(sizeof(DumpStatement));
    ds->dumpStatementType = DUMP_STATEMENT;
    ds->graph = graph;
    ds->file = file;
    return ds;
}

DumpStatement* newDumpStatementAsDot(Value* graph, Value* file) {
    DumpStatement* ds = malloc(sizeof(DumpStatement));
    ds->dumpStatementType = DUMP_STATEMENT_DOT;
    ds->graph = graph;
    ds->file = file;
    return ds;
}

// DataType
DataType* newDataTypeFromPrimitiveDataType(PrimitiveDataType* primitiveDataType) {
    DataType* dt = malloc(sizeof(DataType));
    dt->dataTypeType = DATA_TYPE_PRIMITIVE_DATA_TYPE;
    dt->primitiveDataType = primitiveDataType;
    dt->extraDataType = NULL;
    dt->collectionType = NULL;
    return dt;
}

DataType* newDataTypeFromExtraDataType(ExtraDataType* extraDataType) {
    DataType* dt = malloc(sizeof(DataType));
    dt->dataTypeType = DATA_TYPE_EXTRA_DATA_TYPE;
    dt->primitiveDataType = NULL;
    dt->extraDataType = extraDataType;
    dt->collectionType = NULL;
    return dt;
}

DataType* newDataTypeFromCollectionType(CollectionType* collectionType) {
    DataType* dt = malloc(sizeof(DataType));
    dt->dataTypeType = DATA_TYPE_COLLECTION_TYPE;
    dt->primitiveDataType = NULL;
    dt->extraDataType = NULL;
    dt->collectionType = collectionType;
    return dt;
}

// PrimitiveDataType
PrimitiveDataType* newPrimitiveDataType(int token) {
    PrimitiveDataType* pdt = malloc(sizeof(PrimitiveDataType));
    pdt->token = token;
    return pdt;
}

// ExtraDataType
ExtraDataType* newExtraDataTypeFromNodeType(NodeType* nodeType) {
    ExtraDataType* edt = malloc(sizeof(ExtraDataType));
    edt->extraDataTypeType = EXTRA_DATA_TYPE_NODE_TYPE;
    edt->nodeType = nodeType;
    edt->edgeType = NULL;
    return edt;
}

ExtraDataType* newExtraDataTypeFromEdgeType(EdgeType* edgeType) {
    ExtraDataType* edt = malloc(sizeof(ExtraDataType));
    edt->extraDataTypeType = EXTRA_DATA_TYPE_EDGE_TYPE;
    edt->nodeType = NULL;
    edt->edgeType = edgeType;
    return edt;
}

// NodeType
NodeType* newNodeType(PrimitiveDataType* primitiveDataType) {
    NodeType* nt = malloc(sizeof(NodeType));
    nt->primitiveDataType = primitiveDataType;
    return nt;
}

// EdgeType
EdgeType* newEdgeType(PrimitiveDataType* primitiveDataType) {
    EdgeType* et = malloc(sizeof(EdgeType));
    et->primitiveDataType = primitiveDataType;
    return et;
}

// Collection Type
CollectionType* newCollectionTypeFromSetType(SetType* setType) {
    CollectionType* ct = malloc(sizeof(CollectionType));
    ct->collectionTypeType = COLLECTION_TYPE_SET_TYPE;
    ct->setType = setType;
    ct->stackType = NULL;
    ct->queueType = NULL;
    ct->graphType = NULL;
    ct->digraphType = NULL;
    return ct;
}

CollectionType* newCollectionTypeFromStackType(StackType* stackType) {
    CollectionType* ct = malloc(sizeof(CollectionType));
    ct->collectionTypeType = COLLECTION_TYPE_STACK_TYPE;
    ct->setType = NULL;
    ct->stackType = stackType;
    ct->queueType = NULL;
    ct->graphType = NULL;
    ct->digraphType = NULL;
    return ct;
}

CollectionType* newCollectionTypeFromQueueType(QueueType* queueType) {
    CollectionType* ct = malloc(sizeof(CollectionType));
    ct->collectionTypeType = COLLECTION_TYPE_QUEUE_TYPE;
    ct->setType = NULL;
    ct->stackType = NULL;
    ct->queueType = queueType;
    ct->graphType = NULL;
    ct->digraphType = NULL;
    return ct;
}

CollectionType* newCollectionTypeFromGraphType(GraphType* graphType) {
    CollectionType* ct = malloc(sizeof(CollectionType));
    ct->collectionTypeType = COLLECTION_TYPE_GRAPH_TYPE;
    ct->setType = NULL;
    ct->stackType = NULL;
    ct->queueType = NULL;
    ct->graphType = graphType;
    ct->digraphType = NULL;
    return ct;
}

CollectionType* newCollectionTypeFromDigraphType(DigraphType* digraphType) {
    CollectionType* ct = malloc(sizeof(CollectionType));
    ct->collectionTypeType = COLLECTION_TYPE_DIGRAPH_TYPE;
    ct->setType = NULL;
    ct->stackType = NULL;
    ct->queueType = NULL;
    ct->graphType = NULL;
    ct->digraphType = digraphType;
    return ct;
}

// SetType
SetType* newSetTypeFromPrimitiveDataType(PrimitiveDataType* primitiveDataType) {
    SetType* st = malloc(sizeof(SetType));
    st->setTypeType = SET_TYPE_PRIMITIVE_DATA_TYPE;
    st->primitiveDataType = primitiveDataType;
    st->extraDataType = NULL;
    return st;
}

SetType* newSetTypeFromExtraDataType(ExtraDataType* extraDataType) {
    SetType* st = malloc(sizeof(SetType));
    st->setTypeType = SET_TYPE_EXTRA_DATA_TYPE;
    st->primitiveDataType = NULL;
    st->extraDataType = extraDataType;
    return st;
}

// StackType
StackType* newStackTypeFromPrimitiveDataType(PrimitiveDataType* primitiveDataType) {
    StackType* st = malloc(sizeof(StackType));
    st->stackTypeType = STACK_TYPE_PRIMITIVE_DATA_TYPE;
    st->primitiveDataType = primitiveDataType;
    st->extraDataType = NULL;
    return st;
}

StackType* newStackTypeFromExtraDataType(ExtraDataType* extraDataType) {
    StackType* st = malloc(sizeof(StackType));
    st->stackTypeType = STACK_TYPE_EXTRA_DATA_TYPE;
    st->primitiveDataType = NULL;
    st->extraDataType = extraDataType;
    return st;
}

// QueueType
QueueType* newQueueTypeFromPrimitiveDataType(PrimitiveDataType* primitiveDataType) {
    QueueType* qt = malloc(sizeof(QueueType));
    qt->queueTypeType = QUEUE_TYPE_PRIMITIVE_DATA_TYPE;
    qt->primitiveDataType = primitiveDataType;
    qt->extraDataType = NULL;
    return qt;
}

QueueType* newQueueTypeFromExtraDataType(ExtraDataType* extraDataType) {
    QueueType* qt = malloc(sizeof(QueueType));
    qt->queueTypeType = QUEUE_TYPE_EXTRA_DATA_TYPE;
    qt->primitiveDataType = NULL;
    qt->extraDataType = extraDataType;
    return qt;
}

// GraphType
GraphType* newGraphType(PrimitiveDataType* nodeType, PrimitiveDataType* edgeType) {
    GraphType* gt = malloc(sizeof(GraphType));
    gt->nodeType = nodeType;
    gt->edgeType = edgeType;
    return gt;
}

// DigraphType
DigraphType* newDigraphType(PrimitiveDataType* nodeType, PrimitiveDataType* edgeType) {
    DigraphType* dt = malloc(sizeof(DigraphType));
    dt->nodeType = nodeType;
    dt->edgeType = edgeType;
    return dt;
}

// DataTypeInstance
DataTypeInstance* newDataTypeInstanceFromPrimitiveDataTypeInstance(PrimitiveDataTypeInstance* primitiveDataTypeInstance) {
    DataTypeInstance* dti = malloc(sizeof(DataTypeInstance));
    dti->dataTypeInstanceType = DATA_TYPE_INSTANCE_PRIMITIVE_DATA_TYPE_INSTANCE;
    dti->primitiveDataTypeInstance = primitiveDataTypeInstance;
    dti->extraDataTypeInstance = NULL;
    return dti;
}

DataTypeInstance* newDataTypeInstanceFromExtraDataTypeInstance(ExtraDataTypeInstance* extraDataTypeInstance) {
    DataTypeInstance* dti = malloc(sizeof(DataTypeInstance));
    dti->dataTypeInstanceType = DATA_TYPE_INSTANCE_EXTRA_DATA_TYPE_INSTANCE;
    dti->primitiveDataTypeInstance = NULL;
    dti->extraDataTypeInstance = extraDataTypeInstance;
    return dti;

}

// PrimitiveDataTypeInstance
PrimitiveDataTypeInstance* newPrimitiveDataTypeInstanceFromString(char * string) {
    PrimitiveDataTypeInstance* pdti = malloc(sizeof(PrimitiveDataTypeInstance));
    pdti->primitiveDataTypeInstanceType = PRIMITIVE_DATA_TYPE_INSTACE_STRING;
    pdti->string = string;
    pdti->letter = '\0';
    pdti->integer = 0;
    pdti->decimal = 0;
    pdti->boolean = false;
    return pdti;
}

PrimitiveDataTypeInstance* newPrimitiveDataTypeInstanceFromLetter(char letter) {
    PrimitiveDataTypeInstance* pdti = malloc(sizeof(PrimitiveDataTypeInstance));
    pdti->primitiveDataTypeInstanceType = PRIMITIVE_DATA_TYPE_INSTACE_CHAR;
    pdti->string = NULL;
    pdti->letter = letter;
    pdti->integer = 0;
    pdti->decimal = 0;
    pdti->boolean = false;
    return pdti;
}

PrimitiveDataTypeInstance* newPrimitiveDataTypeInstanceFromInteger(int integer) {
    PrimitiveDataTypeInstance* pdti = malloc(sizeof(PrimitiveDataTypeInstance));
    pdti->primitiveDataTypeInstanceType = PRIMITIVE_DATA_TYPE_INSTACE_INTEGER;
    pdti->string = NULL;
    pdti->letter = '\0';
    pdti->integer = integer;
    pdti->decimal = 0;
    pdti->boolean = false;
    return pdti;
}

PrimitiveDataTypeInstance* newPrimitiveDataTypeInstanceFromDecimal(double decimal) {
    PrimitiveDataTypeInstance* pdti = malloc(sizeof(PrimitiveDataTypeInstance));
    pdti->primitiveDataTypeInstanceType = PRIMITIVE_DATA_TYPE_INSTACE_DECIMAL;
    pdti->string = NULL;
    pdti->letter = '\0';
    pdti->integer = 0;
    pdti->decimal = decimal;
    pdti->boolean = false;
    return pdti;
}

PrimitiveDataTypeInstance* newPrimitiveDataTypeInstanceFromBoolean(boolean boolean) {
    PrimitiveDataTypeInstance* pdti = malloc(sizeof(PrimitiveDataTypeInstance));
    pdti->primitiveDataTypeInstanceType = PRIMITIVE_DATA_TYPE_INSTACE_BOOLEAN;
    pdti->string = NULL;
    pdti->letter = '\0';
    pdti->integer = 0;
    pdti->decimal = 0;
    pdti->boolean = boolean;
    return pdti;
}

// ExtraDataTypeInstance
ExtraDataTypeInstance* newExtraDataTypeInstanceFromNode(NodeInstance* node) {
    ExtraDataTypeInstance* edti = malloc(sizeof(ExtraDataTypeInstance));
    edti->extraDataTypeInstanceType = EXTRA_DATA_TYPE_INSTANCE_NODE_INSTANCE;
    edti->nodeInstance = node;
    edti->edgeInstance = NULL;
    return edti;
}
ExtraDataTypeInstance* newExtraDataTypeInstanceFromEdge(EdgeInstance* edge) {
    ExtraDataTypeInstance* edti = malloc(sizeof(ExtraDataTypeInstance));
    edti->extraDataTypeInstanceType = EXTRA_DATA_TYPE_INSTANCE_EDGE_INSTANCE;
    edti->nodeInstance = NULL;
    edti->edgeInstance = edge;
    return edti;
}

// NodeInstance
NodeInstance* newNodeInstance(Value* label) {
    NodeInstance* ni = newNodeInstanceWithValue(label, NULL);
    ni->nodeInstaceType = NODE_INSTANCE;
    return ni;
}

NodeInstance* newNodeInstanceWithValue(Value* label, Value* value) {
    NodeInstance* ni = malloc(sizeof(NodeInstance));
    ni->nodeInstaceType = NODE_INSTANCE_WITH_VALUE;
    ni->label = label;
    ni->value = value;
    return ni;
}

// EdgeInstance
EdgeInstance* newEdgeInstance(Value* left, Value* right) {
    EdgeInstance* ei = newEdgeInstanceWithValue(left, right, NULL);
    ei->edgeInstanceType = EDGE_INSTANCE;
    return ei;
}

EdgeInstance* newEdgeInstanceWithValue(Value* left, Value* right, Value* value) {
    EdgeInstance* ei = malloc(sizeof(EdgeInstance));
    ei->edgeInstanceType = EDGE_INSTANCE_WITH_VALUE;
    ei->left = left;
    ei->right = right;
    ei->value = value;
    return ei;
}