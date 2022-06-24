#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "../../frontend/syntactic-analysis/bison-parser.h"
#include "../../frontend/syntactic-analysis/bison-actions.h"
#include "../domain-specific/sem.h"
#include "../domain-specific/sem.h"
#include "../support/logger.h"
#include "generator.h"

/**
 * Implementación de "generator.h".
 */

static void generateProgram(Program* p, CompilerState* state, FILE* out);
static void generateStartDefinition(StartDefinition* sd, CompilerState* state, FILE* out);
static void generateFunctionDefinitions(FunctionDefinitions* fd, CompilerState* state, FILE* out);
static void generateFunctionDefinition(FunctionDefinition* fd, CompilerState* state, FILE* out);
static void generateArgumentDefinition(ArgumentDefinition* ad, CompilerState* state, FILE* out);
static void generateArgumentList(ArgumentList* al, CompilerState* state, FILE* out);
static void generateVariableDefinition(VariableDefinition* vd, CompilerState* state, FILE* out);
static void generateForBlock(ForBlock* fb, CompilerState* state, FILE* out);
static void generateForStatement(ForStatement* fs, CompilerState* state, FILE* out);
static void generateRange(Range* r, CompilerState* state, FILE* out);
static void generateItType(ItType* it, CompilerState* state, FILE* out);
static void generateWhileBlock(WhileBlock* wb, CompilerState* state, FILE* out);
static void generateIfBlock(IfBlock* ib, CompilerState* state, FILE* out);
static void generateCondition(Condition* c, CompilerState* state, FILE* out);
static void generateValue(Value* v, CompilerState* state, FILE* out);
static void generateFunctionCall(FunctionCall* fc, CompilerState* state, FILE* out);
static void generateParamDefinition(ParamDefinition* pd, CompilerState* state, FILE* out);
static void generateParamList(ParamList* pl, CompilerState* state, FILE* out);
static void generateComparator(Comparator* c, CompilerState* state, FILE* out);
static void generateBody(Body* b, CompilerState* state, FILE* out);
static void generateStatements(Statements* s, CompilerState* state, FILE* out);
static void generateStatement(Statement* s, CompilerState* state, FILE* out);
static void generateCreateStatement(CreateStatement* cs, CompilerState* state, FILE* out);
static void generateInsertStatement(InsertStatement* is, CompilerState* state, FILE* out);
static void generateReturnStatement(ReturnStatement* rs, CompilerState* state, FILE* out);
static void generateLetBeStatement(LetBeStatement* lbs, CompilerState* state, FILE* out);
static void generateAssignmentStatement(AssignmentStatement* as, CompilerState* state, FILE* out);
static void generatePopFunction(PopFunction* pf, CompilerState* state, FILE* out);
static void generatePrintStatement(PrintStatement* ps, CompilerState* state, FILE* out);
static void generateDumpStatement(DumpStatement* ds, CompilerState* state, FILE* out);
static void generateDataType(DataType* dt, CompilerState* state, FILE* out);
static void generatePrimitiveDataType(PrimitiveDataType* pdt, CompilerState* state, FILE* out);
static void generateExtraDataType(ExtraDataType* edt, CompilerState* state, FILE* out);
static void generateNodeType(NodeType* nt, CompilerState* state, FILE* out);
static void generateEdgeType(EdgeType* et, CompilerState* state, FILE* out);
static void generateCollectionType(CollectionType* ct, CompilerState* state, FILE* out);
static void generateSetType(SetType* st, CompilerState* state, FILE* out);
static void generateStackType(StackType* st, CompilerState* state, FILE* out);
static void generateQueueType(QueueType* qt, CompilerState* state, FILE* out);
static void generateGraphType(GraphType* gt, CompilerState* state, FILE* out);
static void generateDigraphType(DigraphType* dt, CompilerState* state, FILE* out);
static void generateDataTypeInstance(DataTypeInstance* dti, CompilerState* state, FILE* out);
static void generatePrimitiveDataTypeInstance(PrimitiveDataTypeInstance* pdti, CompilerState* state, FILE* out);
static void generateExtraDataTypeInstance(ExtraDataTypeInstance* edti, CompilerState* state, FILE* out);
static void generateNodeInstance(NodeInstance* ni, CompilerState* state, FILE* out);
static void generateEdgeInstance(EdgeInstance* ei, CompilerState* state, FILE* out);

typedef enum ERRNO_ENUM {
	SUCCESS,
	FAILURE,
} ERRNO_ENUM;

static ERRNO_ENUM errno;

static void invalidType(const char * type, int token) {
	LogError("Unknwon type for %s: %d\n", type, token);
	abort();
}

static void semError(int line_number) {
	LogError("Semantyc error in line %d. Aborting...\n", line_number);
	exit(errno = FAILURE);
}

static void copyToOutput(const char * temp, const char * output) {
	FILE* in = fopen(temp, "r");
	FILE* out = fopen(output, "w");
	if (in == NULL || out == NULL)
		abort();
	
	int c = fgetc(in);
    while (c != EOF)
    {
        fputc(c, out);
        c = fgetc(in);
    }

	fclose(in);
	fclose(out);
}

boolean Generator(CompilerState* state, const char * outputFileName) {
	FILE *out = fopen(TEMP_FILENAME, "w");
	errno = SUCCESS;
	
	state->stfv = newContextStack(sizeof(struct DataType));
	state->stfr = newContextStack(sizeof(struct DataType));
	state->stfa = newContextStack(sizeof(struct ArgumentDefinition));
	generateProgram(state->program, state, out);
	freeStack(state->stfv);
	freeStack(state->stfr);
	freeStack(state->stfa);

	fclose(out);
	if (errno == SUCCESS) {
		copyToOutput(TEMP_FILENAME, outputFileName);
		remove(TEMP_FILENAME);
		return true;
	}

	remove(TEMP_FILENAME);
	return false;
}

static void generateProgram(Program* p, CompilerState* state, FILE* out) {
    LogDebug("Generating Program...");
	fprintf(out, "import java.util.HashSet;\n"
					"import java.util.Queue;\n"
					"import java.util.Set;\n"
					"import java.util.Stack;\n"
					"import java.util.concurrent.LinkedBlockingQueue;\n"
					"\n"
					"public class Program {\n");

	pushContext(state->stfv); // global context
	pushContext(state->stfr);
	pushContext(state->stfa);

    generateFunctionDefinitions(p->functionDefinitions, state, out);
    generateStartDefinition(p->startDefinition, state, out);

	fprintf(out, "}\n");

    popContext(state->stfv);
    popContext(state->stfr);
    popContext(state->stfa);
}

static void generateFunctionDefinitions(FunctionDefinitions* fd, CompilerState* state, FILE* out) {
	LogDebug("Generating FunctionDefinitions...");
	switch(fd->functionDefinitionsType) {
		case FUNCTION_DEFINITIONS_EMPTY:
			break;
		case FUNCTION_DEFINITIONS_LIST:
			generateFunctionDefinitions(fd->functionDefinitions, state, out);
			generateFunctionDefinition(fd->functionDefinition, state, out);
			break;
		default:
			invalidType("FunctionDefinitions", fd->functionDefinitionsType);
	}
}

static void generateFunctionDefinition(FunctionDefinition* fd, CompilerState* state, FILE* out) {
	LogDebug("Generating FunctionDefinition...");
	if (retrieveData(state->stfr, fd->identifier) != NULL)
		semError(__LINE__);
	addToContext(state->stfr, fd->identifier, fd->dataType);
	addToContext(state->stfa, fd->identifier, fd->argumentDefinition);	
	fprintf(out, "static ");
	generateDataType(fd->dataType, state, out);
	fprintf(out, " %s", fd->identifier);
	pushContext(state->stfv); // For the arguments
	generateArgumentDefinition(fd->argumentDefinition, state, out);
	fprintf(out, " ");
	generateBody(fd->body, state, out);
	popContext(state->stfv);
}

static void generateDataType(DataType* dt, CompilerState* state, FILE* out) {
	LogDebug("Generating DataType...");
	switch(dt->dataTypeType) {
		case DATA_TYPE_PRIMITIVE_DATA_TYPE:
		    generatePrimitiveDataType(dt->primitiveDataType, state, out);
			break;
		case DATA_TYPE_EXTRA_DATA_TYPE:
			generateExtraDataType(dt->extraDataType, state, out);
			break;
		case DATA_TYPE_COLLECTION_TYPE:
			generateCollectionType(dt->collectionType, state, out);
			break;
		default:
			invalidType("DataType", dt->dataTypeType);
	}
}

static void generatePrimitiveDataType(PrimitiveDataType* pdt, CompilerState* state, FILE* out) {
	LogDebug("Generating PrimitiveDataType...");
	switch(pdt->token) {
		case EMPTY_TYPE:
			fprintf(out, "Empty"); // TODO: Check
			break;
		case CHAR_TYPE:
			fprintf(out, "char");
			break;
		case STRING_TYPE:
			fprintf(out, "String");
			break;
		case INTEGER_TYPE:
			fprintf(out, "Integer");
			break;
		case DECIMAL_TYPE:
			fprintf(out, "Double");
			break;
		case BOOLEAN_TYPE:
			fprintf(out, "Boolean");
			break;
		default:
			invalidType("PrimitiveDataType", pdt->token);
	}
}

static void generateExtraDataType(ExtraDataType* edt, CompilerState* state, FILE* out) {
	LogDebug("Generating ExtraDataType...");
	switch(edt->extraDataTypeType) {
		case EXTRA_DATA_TYPE_NODE_TYPE:
			generateNodeType(edt->nodeType, state, out);
			break;
		case EXTRA_DATA_TYPE_EDGE_TYPE:
			generateEdgeType(edt->edgeType, state, out);
			break;
		default:
			invalidType("ExtraDataType", edt->extraDataTypeType);
	}
}

static void generateNodeType(NodeType* nt, CompilerState* state, FILE* out) {
	LogDebug("Generating NodeType...");
	fprintf(out, "Node<");
	generatePrimitiveDataType(nt->primitiveDataType, state, out);
	fprintf(out, ">");
}

static void generateEdgeType(EdgeType* et, CompilerState* state, FILE* out) {
	LogDebug("Generating EdgeType...");
	fprintf(out, "Edge<");
	generatePrimitiveDataType(et->primitiveDataType, state, out);
	fprintf(out, ">");
}

static void generateCollectionType(CollectionType* ct, CompilerState* state, FILE* out) {
	LogDebug("Generating CollectionType...");
	switch(ct->collectionTypeType) {
		case COLLECTION_TYPE_SET_TYPE:
			generateSetType(ct->setType, state, out);
			break;
		case COLLECTION_TYPE_STACK_TYPE:
			generateStackType(ct->stackType, state, out);
			break;
		case COLLECTION_TYPE_QUEUE_TYPE:
			generateQueueType(ct->queueType, state, out);
			break;
		case COLLECTION_TYPE_GRAPH_TYPE:
			generateGraphType(ct->graphType, state, out);
			break;
		case COLLECTION_TYPE_DIGRAPH_TYPE:
			generateDigraphType(ct->digraphType, state, out);
			break;
		default:
			invalidType("CollectionType", ct->collectionTypeType);
	}
}

static void generateSetType(SetType* st, CompilerState* state, FILE* out) {
	LogDebug("Generating SetType...");
	fprintf(out, "Set<");
	switch(st->setTypeType) {
		case SET_TYPE_PRIMITIVE_DATA_TYPE:
			generatePrimitiveDataType(st->primitiveDataType, state, out);
			break;
		case SET_TYPE_EXTRA_DATA_TYPE:
			generateExtraDataType(st->extraDataType, state, out);
			break;
		default:
			invalidType("SetType", st->setTypeType);
	}
	fprintf(out, ">");
}

static void generateStackType(StackType* st, CompilerState* state, FILE* out) {
	LogDebug("Generating StackType...");
	fprintf(out, "Stack<");
	switch(st->stackTypeType) {
		case STACK_TYPE_PRIMITIVE_DATA_TYPE:
			generatePrimitiveDataType(st->primitiveDataType, state, out);
			break;
		case STACK_TYPE_EXTRA_DATA_TYPE:
			generateExtraDataType(st->extraDataType, state, out);
			break;
		default:
			invalidType("StackType", st->stackTypeType);
	}
	fprintf(out, ">");
}

static void generateQueueType(QueueType* qt, CompilerState* state, FILE* out) {
	LogDebug("Generating QueueType...");
	fprintf(out, "Queue<");
	switch(qt->queueTypeType) {
		case QUEUE_TYPE_PRIMITIVE_DATA_TYPE:
			generatePrimitiveDataType(qt->primitiveDataType, state, out);
			break;
		case QUEUE_TYPE_EXTRA_DATA_TYPE:
			generateExtraDataType(qt->extraDataType, state, out);
			break;
		default:
			invalidType("QueueType", qt->queueTypeType);
	}
	fprintf(out, ">");
}

static void generateGraphType(GraphType* gt, CompilerState* state, FILE* out) {
	LogDebug("Generating GraphType...");
	fprintf(out, "Graph<");
	generatePrimitiveDataType(gt->nodeType, state, out);
	fprintf(out, ", ");
	generatePrimitiveDataType(gt->edgeType, state, out);
	fprintf(out, ">");
}

static void generateDigraphType(DigraphType* gt, CompilerState* state, FILE* out) {
	LogDebug("Generating Digraph...");
	fprintf(out, "Digraph<");
	generatePrimitiveDataType(gt->nodeType, state, out);
	fprintf(out, ", ");
	generatePrimitiveDataType(gt->edgeType, state, out);
	fprintf(out, ">");
}

static void generateArgumentDefinition(ArgumentDefinition* ad, CompilerState* state, FILE* out) {
	LogDebug("Generating ArgumentDefinition...");
	fprintf(out, "(");
	switch(ad->argumentDefinitionType) {
		case ARGUMENT_DEFINITION_EMPTY:
			fprintf(out, "void");
			break;
		case ARGUMENT_DEFINITION_LIST:
			generateArgumentList(ad->argumentList, state, out);
			break;
		default:
			invalidType("ArgumentDefinition", ad->argumentDefinitionType);
	}
	fprintf(out, ")");
}

static void generateArgumentList(ArgumentList* al, CompilerState* state, FILE* out) {
	LogDebug("Generating ArgumentDefinition...");
	switch(al->argumentListType) {
		case ARGUMENT_LIST_VARIABLE_DEFINITION:
			generateVariableDefinition(al->variableDefinition, state, out);
			break;
		case ARGUMENT_LIST_LIST_COMMA_VARIABLE:
			generateArgumentList(al->argumentList, state, out);
			fprintf(out, ", ");
			generateVariableDefinition(al->variableDefinition, state, out);
			break;
		default:
			invalidType("ArgumentList", al->argumentListType);
	}
}

static void generateVariableDefinition(VariableDefinition* vd, CompilerState* state, FILE* out) {
	LogDebug("Generating VariableDefinition...");
	generateDataType(vd->dataType, state, out);
	fprintf(out, " %s", vd->identifier);
	if (retrieveData(state->stfv, vd->identifier) != NULL)
		semError(__LINE__);
	addToContext(state->stfv, vd->identifier, vd->dataType);	
}

static void generateBody(Body* b, CompilerState* state, FILE* out) {
	LogDebug("Generating Body...");
	fprintf(out, "{\n");
	pushContext(state->stfv);

    generateStatements(b->statements, state, out);

	popContext(state->stfv);
	fprintf(out, "}\n");
}

static void generateStatements(Statements* s, CompilerState* state, FILE* out) {
	LogDebug("Generating Statements...");
	switch(s->statementsType) {
		case STATEMENTS_STATEMENTS_STATEMENT:
			generateStatements(s->statements, state, out);
			generateStatement(s->statement, state, out);
			break;
		case STATEMENTS_EMPTY:
			break;
		default:
			invalidType("Statements", s->statementsType);
	}
}

static void generateStatement(Statement* s, CompilerState* state, FILE* out) {
	LogDebug("Generating Statement...");
	switch(s->statementType) {
		case STATEMENT_EMPTY:
			break;
		case STATEMENT_FOR_BLOCK:
			generateForBlock(s->forBlock, state, out);
			break;
		case STATEMENT_IF_BLOCK:
			generateIfBlock(s->ifBlock, state, out);
			break;
		case STATEMENT_WHILE_BLOCK:
			generateWhileBlock(s->whileBlock, state, out);
			break;
		case STATEMENT_CREATE_STATEMENT:
			generateCreateStatement(s->createStatement, state, out);
			break;
		case STATEMENT_INSERT_STATEMENT:
			generateInsertStatement(s->insertStatement, state, out);
			break;
		case STATEMENT_LET_BE_STATEMENT:
			generateLetBeStatement(s->letBeStatement, state, out);
			break;
		case STATEMENT_RETURN_STATEMENT:
			generateReturnStatement(s->returnStatement, state, out);
			break;
		case STATEMENT_ASSIGNMENT_STATEMENT:
			generateAssignmentStatement(s->assignmentStatement, state, out);
			break;
		case STATEMENT_PRINT_STATEMENT:
			generatePrintStatement(s->printStatement, state, out);
			break;
		case STATEMENT_DUMP_STATAMENT:
			generateDumpStatement(s->dumpStatement, state, out);
			break;
		default:
			invalidType("Statement", s->statementType);
	}
}

static void generateForBlock(ForBlock* fb, CompilerState* state, FILE* out) {
	LogDebug("Generating ForBlock...");
	pushContext(state->stfv);
	generateForStatement(fb->forStatement, state, out);
	generateBody(fb->body, state, out);
	if (fb->forStatement->range->rangeType == RANGE_EACH || fb->forStatement->range->rangeType == RANGE_GRAPH) {
		fprintf(out, ");\n");
	}
	popContext(state->stfv);
}

static void generateForStatement(ForStatement* fs, CompilerState* state, FILE* out) {
	LogDebug("Generating ForStatement...");
	switch(fs->range->rangeType) {
		case RANGE_INCLUSIVE:
			if (retrieveData(state->stfv, fs->identifier) != NULL)
				semError(__LINE__);
			addToContext(state->stfv, fs->identifier, getDataType(fs->range->value));
			if (!isIntegerType(getDataType(fs->range->value)))
				semError(__LINE__);
			if (!isIntegerType(getDataType(fs->range->limitValue)))
				semError(__LINE__);
			fprintf(out, "for(int %s = ", fs->identifier);
			generateValue(fs->range->value, state, out);
			fprintf(out, "; %s <= ", fs->identifier);
			generateValue(fs->range->limitValue, state, out);
			fprintf(out, "; %s++)", fs->identifier);
			break;
		case RANGE_EXCLUSIVE:
			if (retrieveData(state->stfv, fs->identifier) != NULL)
				semError(__LINE__);
			addToContext(state->stfv, fs->identifier, getDataType(fs->range->value));
			if (!isIntegerType(getDataType(fs->range->value)))
				semError(__LINE__);
			if (!isIntegerType(getDataType(fs->range->limitValue)))
				semError(__LINE__);
			fprintf(out, "for(int %s = ", fs->identifier);
			generateValue(fs->range->value, state, out);
			fprintf(out, "; %s < ", fs->identifier);
			generateValue(fs->range->limitValue, state, out);
			fprintf(out, "; %s++)", fs->identifier);
			break;
		case RANGE_EACH:
			LogDebug("Tipo %d, dt %x", isCollectionType(getDataType(fs->range->value)), getDataType(fs->range->value));
			if (!isCollectionType(getDataType(fs->range->value)))
				semError(__LINE__);
			if (retrieveData(state->stfv, fs->identifier) != NULL)
				semError(__LINE__);
			addToContext(state->stfv, fs->identifier, getInnerDataTypeFromCollectionType(((DataType*)getDataType(fs->range->value))->collectionType));
			generateValue(fs->range->value, state, out);
			fprintf(out, ".forEach( (%s) -> ", fs->identifier);
			break;
		case RANGE_GRAPH:
			LogDebug("%x %x", fs->range->value, getDataType(fs->range->value));
			// if (!isGraphType(getDataType(fs->range->value)));
				// semError(__LINE__);
			if (retrieveData(state->stfv, fs->identifier) != NULL)
				semError(__LINE__);
			addToContext(state->stfv, fs->identifier, newDataTypeFromExtraDataType(newExtraDataTypeFromNodeType(newNodeType(newPrimitiveDataType(STRING_TYPE))))); // TODO: We currently don't check inside the extraDataTypes
			generateValue(fs->range->value, state, out);
			fprintf(out, ".forEach( (%s) -> ", fs->identifier);
			break;
		default:
			invalidType("ForStatement->Range", fs->range->rangeType);
	}
}

static void generateRange(Range* r, CompilerState* state, FILE* out) {
	LogDebug("Generating Range...");
	// Is handled in ForStatement
}

static void generateValue(Value* v, CompilerState* state, FILE* out) {
	LogDebug("Generating Value...");
	switch(v->valueType) {
		case VALUE_IDENTIFIER:
			fprintf(out, "%s", v->identifier);
			break;
		case VALUE_FUNCTION_CALL:
			generateFunctionCall(v->functionCall, state, out);
			break;
		case VALUE_DATA_TYPE_INSTANCE:
			generateDataTypeInstance(v->dataTypeInstance, state, out);
			break;
		case VALUE_VALUE_ADD_VALUE:
			if (!(isNumericType(getDataType(v->left)) && isNumericType(getDataType(v->right))))
			if (!isStringType(getDataType(v->left)) && !isStringType(getDataType(v->left)))
				semError(__LINE__);
			fprintf(out, "(");
			generateValue(v->left, state, out);
			fprintf(out, "+");
			generateValue(v->right, state, out);
			fprintf(out, ")");
			break;
		case VALUE_VALUE_SUB_VALUE:
			if (!(isNumericType(getDataType(v->left)) && isNumericType(getDataType(v->right))))
				semError(__LINE__);
			fprintf(out, "(");
			generateValue(v->left, state, out);
			fprintf(out, "-");
			generateValue(v->right, state, out);
			fprintf(out, ")");
			break;
		case VALUE_VALUE_MUL_VALUE:
			if (!(isNumericType(getDataType(v->left)) && isNumericType(getDataType(v->right))))
				semError(__LINE__);
			fprintf(out, "(");
			generateValue(v->left, state, out);
			fprintf(out, "*");
			generateValue(v->right, state, out);
			fprintf(out, ")");
			break;
		case VALUE_VALUE_DIV_VALUE:
			if (!(isNumericType(getDataType(v->left)) && isNumericType(getDataType(v->right))))
				semError(__LINE__);
			fprintf(out, "(");
			generateValue(v->left, state, out);
			fprintf(out, "/");
			generateValue(v->right, state, out);
			fprintf(out, ")");
			break;
		case VALUE_POP_FUNCTION:
			generatePopFunction(v->popFunction, state, out);
			break;
		case VALUE_VALUE_DOT_DATA:
			LogDebug("%s %x %x %x", v->value->identifier, v->value, getDataType(v->value)->dataTypeType, isExtraDataType(getDataType(v->value)));
			if (! isExtraDataType(getDataType(v->value)))
				semError(__LINE__);
			generateValue(v->value, state, out);
			fprintf(out, ".getData()");
			break;
		case VALUE_VALUE_DOT_EDGES:
			if (! isGraphType(getDataType(v->value)))
				semError(__LINE__);
			generateValue(v->value, state, out);
			fprintf(out, ".getEdges()");
			break;
		case VALUE_VALUE_DOT_NODES:
			if (! isGraphType(getDataType(v->value)))
				semError(__LINE__);
			generateValue(v->value, state, out);
			fprintf(out, ".getNodes()");
			break;
		default:
			invalidType("Value", v->valueType);
	}
}

static void generateFunctionCall(FunctionCall* fc, CompilerState* state, FILE* out) {
	LogDebug("Generating FunctionCall...");
	if (!matchArgumentDefinitionToParamDefinition(retrieveData(state->stfa, fc->identifier), fc->paramDefinition))
		semError(__LINE__);
	fprintf(out, "%s", fc->identifier);
	generateParamDefinition(fc->paramDefinition, state, out);
}

static void generateParamDefinition(ParamDefinition* pd, CompilerState* state, FILE* out) {
	LogDebug("Generating ParamDefinition...");
	fprintf(out, "(");
	switch (pd->paramDefinitionType) {
		case PARAM_DEFINITION_EMPTY:
			break;
		case PARAM_DEFINITION_PARAM_LIST:
			generateParamList(pd->paramList, state, out);
			break;
		default:
			invalidType("ParamDefinition", pd->paramDefinitionType);
	}
	fprintf(out, ")");
}

static void generateParamList(ParamList* pl, CompilerState* state, FILE* out) {
	LogDebug("Generating ParamList...");
	switch(pl->paramListType) {
		case PARAM_LIST_PARAM_LIST_COMMA_VALUE:
			generateParamList(pl->paramList, state, out);
			fprintf(out, ", ");
			generateValue(pl->value, state, out);
			break;
		case PARAM_LIST_VALUE:
			generateValue(pl->value, state, out);
			break;
		default:
			invalidType("ParamList", pl->paramListType);
	}
}

static void generateDataTypeInstance(DataTypeInstance* dti, CompilerState* state, FILE* out) {
	LogDebug("Generating DataTypeInstance...");
	switch (dti->dataTypeInstanceType) {
		case DATA_TYPE_INSTANCE_PRIMITIVE_DATA_TYPE_INSTANCE:
			generatePrimitiveDataTypeInstance(dti->primitiveDataTypeInstance, state, out);
			break;
		case DATA_TYPE_INSTANCE_EXTRA_DATA_TYPE_INSTANCE:
			generateExtraDataTypeInstance(dti->extraDataTypeInstance, state, out);
			break;
		default:
			invalidType("DataTypeInstance", dti->dataTypeInstanceType);
	}
}

static void generatePrimitiveDataTypeInstance(PrimitiveDataTypeInstance* pdti, CompilerState* state, FILE* out) {
	LogDebug("Generating PrimitiveDataTypeInstance...");
	switch(pdti->primitiveDataTypeInstanceType) {
		case PRIMITIVE_DATA_TYPE_INSTACE_STRING:
			fprintf(out, "%s", pdti->string);
			break;
		case PRIMITIVE_DATA_TYPE_INSTACE_CHAR:
			fprintf(out, "'%c'", pdti->letter);
			break;
		case PRIMITIVE_DATA_TYPE_INSTACE_INTEGER:
			fprintf(out, "%d", pdti->integer);
			break;
		case PRIMITIVE_DATA_TYPE_INSTACE_DECIMAL:
			fprintf(out, "%g", pdti->decimal);
			break;
		case PRIMITIVE_DATA_TYPE_INSTACE_BOOLEAN:
			fprintf(out, "%s", (pdti->bool) ? "true" : "false");
			break;
		default:
			invalidType("PrimitiveDataTypeInstance", pdti->primitiveDataTypeInstanceType);
	}
}

static void generateExtraDataTypeInstance(ExtraDataTypeInstance* edti, CompilerState* state, FILE* out) {
	LogDebug("Generating ExtraDataTypeInstance...");
	switch(edti->extraDataTypeInstanceType) {
		case EXTRA_DATA_TYPE_INSTANCE_NODE_INSTANCE:
			generateNodeInstance(edti->nodeInstance, state, out);
			break;
		case EXTRA_DATA_TYPE_INSTANCE_EDGE_INSTANCE:
			generateEdgeInstance(edti->edgeInstance, state, out);
			break;
		default:
			invalidType("ExtraDataTypeInstance", edti->extraDataTypeInstanceType);
	}
}

static void generateNodeInstance(NodeInstance* ni, CompilerState* state, FILE* out) {
	LogDebug("Generating NodeInstance...");
	switch (ni->nodeInstaceType) {
		case NODE_INSTANCE:
			if (!isStringType(getDataType(ni->label)))
				semError(__LINE__);
			fprintf(out, "new Node<Empty>(");
			generateValue(ni->label, state, out);
			fprintf(out, ")");
			break;
		case NODE_INSTANCE_WITH_VALUE:
			if (!isStringType(getDataType(ni->label)))
				semError(__LINE__);
			fprintf(out, "new Node<>(");
			generateValue(ni->label, state, out);
			fprintf(out, ",");
			generateValue(ni->value, state, out);
			fprintf(out, ")");
			break;
		default:
			invalidType("NodeInstance", ni->nodeInstaceType);
	}
}

static void generateEdgeInstance(EdgeInstance* ei, CompilerState* state, FILE* out) {
	LogDebug("Generating EdgeInstance...");
	switch (ei->edgeInstanceType) {
		case EDGE_INSTANCE:
			if (!isStringType(getDataType(ei->left)))
				semError(__LINE__);
			if (!isStringType(getDataType(ei->right)))
				semError(__LINE__);
			fprintf(out, "new Edge<Empty>(");
			generateValue(ei->left, state, out);
			fprintf(out, ",");
			generateValue(ei->right, state, out);
			fprintf(out, ")");
			break;
		case EDGE_INSTANCE_WITH_VALUE:
			if (!isStringType(getDataType(ei->left)))
				semError(__LINE__);
			if (!isStringType(getDataType(ei->right)))
				semError(__LINE__);
			fprintf(out, "new Edge<>(");
			generateValue(ei->left, state, out);
			fprintf(out, ",");
			generateValue(ei->right, state, out);
			fprintf(out, ",");
			generateValue(ei->value, state, out);
			fprintf(out, ")");
			break;
		default:
			invalidType("EdgeInstance", ei->edgeInstanceType);
	}
}

static void generatePopFunction(PopFunction* pf, CompilerState* state, FILE* out) {
	LogDebug("Generating PopFunction...");
	if (!(isStackType(retrieveData(state->stfv, pf->identifier)) ||
		isQueueType(retrieveData(state->stfv, pf->identifier))))
			semError(__LINE__);
	LogDebug("Generating PopFunction...");
	if (isStackType(retrieveData(state->stfv, pf->identifier)))
		fprintf(out, "%s.pop()", pf->identifier);
		LogDebug("Generating PopFunction...");
	if (isQueueType(retrieveData(state->stfv, pf->identifier)))
		fprintf(out, "%s.remove()", pf->identifier);
}

static void generateItType(ItType* it, CompilerState* state, FILE* out) {
	// TODO: Depends on for statements
}

static void generateIfBlock(IfBlock* ib, CompilerState* state, FILE* out) {
	LogDebug("Generating IfBlock...");
	switch(ib->ifBlockType) {
		case IF_BLOCK_IF_CONDITION:
			fprintf(out, "if ");
			generateCondition(ib->condition, state, out);
			fprintf(out, " ");
			generateBody(ib->body, state, out);
			break;
		case IF_BLOCK_IF_ELSE:
			fprintf(out, "if ");
			generateCondition(ib->condition, state, out);
			fprintf(out, " ");
			generateBody(ib->body, state, out);
			fprintf(out, "else ");
			generateBody(ib->elseBody, state, out);
			break;
		case IF_BLOCK_IF_ELSE_IF:
			fprintf(out, "if ");
			generateCondition(ib->condition, state, out);
			fprintf(out, " ");
			generateBody(ib->body, state, out);
			fprintf(out, "else ");
			generateIfBlock(ib->ifBlock, state, out);
			break;
		default:
			invalidType("IfBlock", ib->ifBlockType);
	}
}

static void generateCondition(Condition* c, CompilerState* state, FILE* out) {
	LogDebug("Generating Condition...");
	fprintf(out, "(");
	switch(c->conditionType) {
		case CONDITION_BOOLEAN:
			fprintf(out, "%s", (c->bool) ? "true" : "false");
			break;
		case CONDITION_VALUE_COMPARATOR_VALUE:
			LogDebug("%d", __LINE__);
			getDataType(c->leftV);
			LogDebug("%d", __LINE__);
			isNumericType(getDataType(c->leftV));
			LogDebug("%d", __LINE__);
			isNumericType(getDataType(c->rightV));
			LogDebug("%d", __LINE__);
			if (!(isNumericType(getDataType(c->leftV)) && isNumericType(getDataType(c->rightV))) && 
				!isStringType(getDataType(c->leftV)))
				semError(__LINE__);
			LogDebug("%d", __LINE__);
			generateValue(c->leftV, state, out);
			LogDebug("%d", __LINE__);
			generateComparator(c->comparator, state, out);
			LogDebug("%d", __LINE__);
			generateValue(c->rightV, state, out);
			LogDebug("%d", __LINE__);
			break;
		case CONDITION_CONDITION_AND_CONDITION:
			generateCondition(c->leftC, state, out);
			fprintf(out, " && ");
			generateCondition(c->rightC, state, out);
			break;
		case CONDITION_CONDITION_OR_CONDITION:
			generateCondition(c->leftC, state, out);
			fprintf(out, " || ");
			generateCondition(c->rightC, state, out);
			break;
		case CONDITION_NOT_CONDITION:
			fprintf(out, "!");
			generateCondition(c->condition, state, out);
			break;
		case CONDITION_IDENTIFIER_EMPTY:
			if (! isCollectionType(retrieveData(state->stfv, c->identifier)))
				semError(__LINE__);
			fprintf(out, "%s.isEmpty()", c->identifier);
			break;
		case CONDITION_IDENTIFIER_NOT_EMPTY:
			if (! isCollectionType(retrieveData(state->stfv, c->identifier)))
				semError(__LINE__);
			fprintf(out, "! %s.isEmpty()", c->identifier);
			break;
		default:
			invalidType("Condition", c->conditionType);
	}
	fprintf(out, ")");
}

static void generateComparator(Comparator* c, CompilerState* state, FILE* out) {
	LogDebug("Generating Comparator...");
	switch(c->comparatorType) {
		case COMPARATOR_GEQ:
			fprintf(out, "<=");
			break;
		case COMPARATOR_LEQ:
			fprintf(out, ">=");
			break;
		case COMPARATOR_GT:
			fprintf(out, ">");
			break;
		case COMPARATOR_LT:
			fprintf(out, "<");
			break;
		case COMPARATOR_EQ:
			fprintf(out, "==");
			break;
		default:
			invalidType("Comparator", c->comparatorType);
	}
}

static void generateWhileBlock(WhileBlock* wb, CompilerState* state, FILE* out) {
	LogDebug("Generating WhileBlock...");
	fprintf(out, "while");
	generateCondition(wb->condition, state, out);
	generateBody(wb->body, state, out);
}

static void generateInstanceFromCollectionType(CollectionType* ct, CompilerState* state, FILE* out) {
	LogDebug("Instantiating CollectionType...");
	switch(ct->collectionTypeType) {
		case COLLECTION_TYPE_SET_TYPE:
			fprintf(out, "new HashSet<>()");
			break;
		case COLLECTION_TYPE_STACK_TYPE:
			fprintf(out, "new Stack<>()");
			break;
		case COLLECTION_TYPE_QUEUE_TYPE:
			fprintf(out, "new LinkedBlockingQueue<>()");
			break;
		case COLLECTION_TYPE_GRAPH_TYPE:
			fprintf(out, "new Bigraph<>()");
			break;
		case COLLECTION_TYPE_DIGRAPH_TYPE:
			fprintf(out, "new Digraph<>()");
			break;
		default:
			invalidType("CollectionType", ct->collectionTypeType);
	}
}

static void generateCreateStatement(CreateStatement* cs, CompilerState* state, FILE* out) {
	LogDebug("Generating CreateStatement...");
	switch(cs->createStatementType) {
		case CREATE_STATEMENT_EXTRA_DATA_TYPE:
			if (retrieveData(state->stfv, cs->identifier) != NULL)
				semError(__LINE__);
			addToContext(state->stfv, cs->identifier, getDataTypeFromExtraDataType(cs->extraDataType));
			generateExtraDataType(cs->extraDataType, state, out);
			fprintf(out, " %s;", cs->identifier);
			break;
		case CREATE_STATEMENT_COLLECTION_TYPE:
			if (retrieveData(state->stfv, cs->identifier) != NULL)
				semError(__LINE__);
			addToContext(state->stfv, cs->identifier, getDataTypeFromCollectionType(cs->collectionType));
			generateCollectionType(cs->collectionType, state, out);
			fprintf(out, " %s = ", cs->identifier);
			generateInstanceFromCollectionType(cs->collectionType, state, out);
			fprintf(out, ";");
			break;
		default:
			invalidType("CreateStatement", cs->createStatementType);
	}
	fprintf(out, "\n");
}

static void generateInsertStatement(InsertStatement* is, CompilerState* state, FILE* out) {
	LogDebug("Generating InsertStatement...");
	if (! isCollectionType(retrieveData(state->stfv, is->identifier)))
		semError(__LINE__);
	// if (! areSameDataType(
	// 		getInnerDataTypeFromCollectionType(((DataType*)retrieveData(state->stfv, is->identifier))->collectionType),
	// 		is->dataType)
	// 	)
	// 	semError(__LINE__);
	fprintf(out, "%s.add(", is->identifier);
	generateValue(is->value, state, out);
	fprintf(out, ");\n");
}

static void generateLetBeStatement(LetBeStatement* lbs, CompilerState* state, FILE* out) {
	LogDebug("Generating LetBeStatement...");
	if (retrieveData(state->stfv, lbs->identifier) != NULL)
		semError(__LINE__);
	DataType* dt = newDataTypeFromPrimitiveDataType(lbs->primitiveDataType);
	addToContext(state->stfv, lbs->identifier, dt);
	generatePrimitiveDataType(lbs->primitiveDataType, state, out);
	fprintf(out, " %s;\n", lbs->identifier);
}

static void generateReturnStatement(ReturnStatement* rs, CompilerState* state, FILE* out) {
	LogDebug("Generating ReturnStatement...");
	switch (rs->returnStatementType) {
		case RETURN_STATEMENT_VOID:
			fprintf(out, "return;\n");
			break;
		case RETURN_STATEMENT_VALUE:
			fprintf(out, "return ");
			generateValue(rs->returnValue, state, out);
			fprintf(out, ";\n");
			break;
		default:
			invalidType("ReturnStatement", rs->returnStatementType);
	}
}

static void generateAssignmentStatement(AssignmentStatement* as, CompilerState* state, FILE* out) {
	LogDebug("Generating AssignmentStatement...");
	switch(as->assignmentStatementType) {
		case ASSIGNMENT_STATEMENT_IDENTIFIER:
			if (! areSameDataType(
				retrieveData(state->stfv, as->identifier),
				getDataType(as->value)
			))
				semError(__LINE__);
			fprintf(out, "%s = ", as->identifier);
			generateValue(as->value, state, out);
			fprintf(out, ";\n");
			break;
		case ASSIGNMENT_STATEMENT_IDENTIFIER_DOT_DATA:
			if (! isExtraDataType(retrieveData(state->stfv, as->identifier)))
				semError(__LINE__);
			if (! areSameDataType(
				retrieveData(state->stfv, as->identifier),
				getDataType(as->value)
			))
				semError(__LINE__);
			fprintf(out, "%s.setData(", as->identifier);
			generateValue(as->value, state, out);
			fprintf(out, ");\n");
			break;
		default:
			invalidType("AssignmentType", as->assignmentStatementType);
	}
}

static void generatePrintStatement(PrintStatement* ps, CompilerState* state, FILE* out) {
	LogDebug("Generating PrintStatement...");
	if (isStackType(getDataType(ps->graph)) || isSetType(getDataType(ps->graph)))
		semError(__LINE__);
	fprintf(out, "System.out.print(");
	generateValue(ps->graph, state, out);
	fprintf(out, ");\n");
}

static void generateDumpStatement(DumpStatement* ds, CompilerState* state, FILE* out) {
	LogDebug("Generating DumpStatement...");
	switch(ds->dumpStatementType) {
		case DUMP_STATEMENT:
			if (! isGraphType(getDataType(ds->graph)))
				semError(__LINE__);
			if (! isStringType(getDataType(ds->file)))
				semError(__LINE__);
			generateValue(ds->graph, state, out); // Todo> Check is graph
			fprintf(out, ".dump(");
			generateValue(ds->file, state, out); // TODO: Check is String
			fprintf(out, ");\n");
			break;
		case DUMP_STATEMENT_DOT:
			if (! isGraphType(getDataType(ds->graph)))
				semError(__LINE__);
			if (! isStringType(getDataType(ds->file)))
				semError(__LINE__);
			generateValue(ds->graph, state, out); // Todo> Check is graph
			fprintf(out, ".dumpAsDot(");
			generateValue(ds->file, state, out); // TODO: Check is String
			fprintf(out, ");\n");
			break;
		default:
			invalidType("DumpStatement", ds->dumpStatementType);
	}
}

static void generateStartDefinition(StartDefinition* sd, CompilerState* state, FILE* out) {
	LogDebug("Generating StartDefinition...");
	fprintf(out, "public static void main(String[] args)");
	generateBody(sd->body, state, out);
}
