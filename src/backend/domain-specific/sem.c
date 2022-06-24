#include <stdlib.h>
#include <stdio.h>

#include "../../frontend/syntactic-analysis/bison-parser.h"
#include "../../frontend/syntactic-analysis/bison-actions.h"
#include "../support/shared.h"
#include "../semantic-analysis/contextStack.h"
#include "sem.h"

// typedef struct Value {
//     VALUE_TYPE valueType;
//     char * identifier;
//     struct FunctionCall* functionCall;
//     struct DataTypeInstance* dataTypeInstance;
//     struct Value * left, * right;
//     struct PopFunction* popFunction;
//     struct Value* value;
// } Value;

DataType* getDataType(Value* v) {
    if (v == NULL)
        return NULL;
    switch(v->valueType) {
		case VALUE_IDENTIFIER:
            return retrieveData(state.stfv, v->identifier);
		case VALUE_FUNCTION_CALL:
            return retrieveData(state.stfr, v->functionCall->identifier);
		case VALUE_DATA_TYPE_INSTANCE:
            return getDataTypeFromDataTypeInstance(v->dataTypeInstance);
		case VALUE_VALUE_ADD_VALUE:
            return getDataType(v->left);
		case VALUE_VALUE_SUB_VALUE:
            return getDataType(v->left);
		case VALUE_VALUE_MUL_VALUE:
            return getDataType(v->left);
		case VALUE_VALUE_DIV_VALUE:
            return getDataType(v->left);
		case VALUE_POP_FUNCTION:
            return getInnerDataTypeFromCollectionType(((DataType*)retrieveData(state.stfv, v->popFunction->identifier))->collectionType);
		case VALUE_VALUE_DOT_DATA:
            return getInnerDataTypeFromExtraDataType(getDataType(v->value)->extraDataType);
		case VALUE_VALUE_DOT_EDGES:
            return newDataTypeFromExtraDataType(newExtraDataTypeFromNodeType(NULL)); //TODO: To check if nodes are of the same type, should get DataType of content
		case VALUE_VALUE_DOT_NODES:
            return newDataTypeFromExtraDataType(newExtraDataTypeFromEdgeType(NULL)); //TODO: To check if nodes are of the same type, should get DataType of content
	}
    return NULL;
}

DataType* getDataTypeFromDataTypeInstance(DataTypeInstance* dti) {
    switch (dti->dataTypeInstanceType) {
		case DATA_TYPE_INSTANCE_PRIMITIVE_DATA_TYPE_INSTANCE: {
            PrimitiveDataType* pdt = NULL;
            switch(dti->primitiveDataTypeInstance->primitiveDataTypeInstanceType) {
                case PRIMITIVE_DATA_TYPE_INSTACE_STRING:
                    pdt = newPrimitiveDataType(STRING_TYPE);
                    break;
                case PRIMITIVE_DATA_TYPE_INSTACE_CHAR:
                    pdt = newPrimitiveDataType(CHAR_TYPE);
                    break;
                case PRIMITIVE_DATA_TYPE_INSTACE_INTEGER:
                    pdt = newPrimitiveDataType(INTEGER_TYPE);
                    break;
                case PRIMITIVE_DATA_TYPE_INSTACE_DECIMAL:
                    pdt = newPrimitiveDataType(DECIMAL_TYPE);
                    break;
                case PRIMITIVE_DATA_TYPE_INSTACE_BOOLEAN:
                    pdt = newPrimitiveDataType(BOOLEAN_TYPE);
                    break;
            }
            return newDataTypeFromPrimitiveDataType(pdt);
        }
		case DATA_TYPE_INSTANCE_EXTRA_DATA_TYPE_INSTANCE: {
            ExtraDataType* edt = NULL;
            switch(dti->extraDataTypeInstance->extraDataTypeInstanceType) {
                case EXTRA_DATA_TYPE_INSTANCE_NODE_INSTANCE:
                    edt = newExtraDataTypeFromNodeType(NULL);
                    break;
                case EXTRA_DATA_TYPE_INSTANCE_EDGE_INSTANCE:
                    edt = newExtraDataTypeFromEdgeType(NULL);
                    break;
            }
            return newDataTypeFromExtraDataType(edt);
        }
	}
    return NULL;
}

DataType* getInnerDataTypeFromCollectionType(CollectionType* ct) {
    if (ct == NULL)
        return NULL;
	switch(ct->collectionTypeType) {
		case COLLECTION_TYPE_SET_TYPE:
            switch(ct->setType->setTypeType) {
                case SET_TYPE_PRIMITIVE_DATA_TYPE:
                    return newDataTypeFromPrimitiveDataType(ct->setType->primitiveDataType);
                case SET_TYPE_EXTRA_DATA_TYPE:
                    return newDataTypeFromExtraDataType(ct->setType->extraDataType);
            }
			break;
		case COLLECTION_TYPE_STACK_TYPE:
            switch(ct->stackType->stackTypeType) {
                case STACK_TYPE_PRIMITIVE_DATA_TYPE:
                    return newDataTypeFromPrimitiveDataType(ct->stackType->primitiveDataType);
                case STACK_TYPE_EXTRA_DATA_TYPE:
                    return newDataTypeFromExtraDataType(ct->stackType->extraDataType);
            }
			break;
		case COLLECTION_TYPE_QUEUE_TYPE:
            switch(ct->queueType->queueTypeType) {
                case QUEUE_TYPE_PRIMITIVE_DATA_TYPE:
                    return newDataTypeFromPrimitiveDataType(ct->queueType->primitiveDataType);
                case QUEUE_TYPE_EXTRA_DATA_TYPE:
                    return newDataTypeFromExtraDataType(ct->queueType->extraDataType);
            }
			break;
	}
    return NULL;
}

DataType* getInnerDataTypeFromExtraDataType(ExtraDataType* edt) {
	switch(edt->extraDataTypeType) {
		case EXTRA_DATA_TYPE_NODE_TYPE:
			return newDataTypeFromPrimitiveDataType(edt->nodeType->primitiveDataType);
		case EXTRA_DATA_TYPE_EDGE_TYPE:
			return newDataTypeFromPrimitiveDataType(edt->edgeType->primitiveDataType);
	}
    return NULL;
}

// Chequear si hay que upperclassear o descomprimir
DataType* getDataTypeFromCollectionType(CollectionType* ct) {
    return newDataTypeFromCollectionType(ct);
}
DataType* getDataTypeFromExtraDataType(ExtraDataType* edt) {
    return newDataTypeFromExtraDataType(edt);
}
DataType* getDataTypeFromPrimitiveDataType(PrimitiveDataType* pdt) {
    return newDataTypeFromPrimitiveDataType(pdt);
}

boolean isCollectionType(DataType* dt) {
    return dt->dataTypeType == DATA_TYPE_COLLECTION_TYPE;
}

boolean isExtraDataType(DataType* dt) {
    return dt->dataTypeType == DATA_TYPE_EXTRA_DATA_TYPE;
}

boolean isPrimitiveDataType(DataType* dt) {
    return dt->dataTypeType == DATA_TYPE_PRIMITIVE_DATA_TYPE;
}

boolean isIntegerType(DataType* dt) {
    return isPrimitiveDataType(dt) && dt->primitiveDataType->token == INTEGER_TYPE;
}

boolean isDecimalType(DataType* dt) {
    return isPrimitiveDataType(dt) && dt->primitiveDataType->token == DECIMAL_TYPE;
}

boolean isStringType(DataType* dt) {
    return isPrimitiveDataType(dt) && dt->primitiveDataType->token == STRING_TYPE;
}

boolean isNumericType(DataType* dt) {
    return isIntegerType(dt) || isDecimalType(dt);
}

boolean isGraphType(DataType* dt) {
    return isCollectionType(dt) && dt->collectionType->collectionTypeType == COLLECTION_TYPE_GRAPH_TYPE;
}

boolean isStackType(DataType* dt) {
    return isCollectionType(dt) && dt->collectionType->collectionTypeType == COLLECTION_TYPE_STACK_TYPE;
}

boolean isQueueType(DataType* dt) {
    return isCollectionType(dt) && dt->collectionType->collectionTypeType == COLLECTION_TYPE_QUEUE_TYPE;
}

static boolean areSameCollectionType(DataType* dt1, DataType* dt2) {
    return dt1->collectionType->collectionTypeType == dt2->collectionType->collectionTypeType;
}

static boolean areSameExtraDataType(DataType* dt1, DataType* dt2) {
    return dt1->extraDataType->extraDataTypeType == dt2->extraDataType->extraDataTypeType;
}

static boolean areSamePrimitiveDataType(DataType* dt1, DataType* dt2) {
    printf("{{%x %x}}", dt1->primitiveDataType, dt2->primitiveDataType);
    fflush(stdout);
    return dt1->primitiveDataType->token == dt2->primitiveDataType->token;
}

boolean areSameDataType(DataType* dt1, DataType* dt2) {
    printf("A"); fflush(stdout);
    if (dt1 == NULL || dt2 == NULL)
        return false;
    printf("A"); fflush(stdout);
    if (isCollectionType(dt1) && isCollectionType(dt2))
        return areSameCollectionType(dt1, dt2);
    printf("A"); fflush(stdout);
    if (isExtraDataType(dt1) && isExtraDataType(dt2))
        return areSameExtraDataType(dt1, dt2);
    printf("A"); fflush(stdout);
    if (isPrimitiveDataType(dt1) && isPrimitiveDataType(dt2)) {
        printf("B"); fflush(stdout);
        return areSamePrimitiveDataType(dt1, dt2);
    }
    printf("A"); fflush(stdout);
    return false;
}

boolean matchArgumentDefinitionToParamDefinition(ArgumentDefinition* ad, ParamDefinition* pd) {
    if (ad == NULL || pd == NULL)
        return false;
    if (ad->argumentDefinitionType == ARGUMENT_DEFINITION_EMPTY && pd->paramDefinitionType == PARAM_DEFINITION_EMPTY)
        return true;
    if (ad->argumentDefinitionType == ARGUMENT_DEFINITION_EMPTY || pd->paramDefinitionType == PARAM_DEFINITION_EMPTY)
        return false;
    ArgumentList* al = ad->argumentList;
    ParamList* pl = pd->paramList;
    while (al != NULL && pd != NULL) {
        if (al->argumentListType == ARGUMENT_LIST_LIST_COMMA_VARIABLE && pl->paramListType == PARAM_LIST_PARAM_LIST_COMMA_VALUE) {
            if (! areSameDataType(al->variableDefinition->dataType, getDataType(pl->value)))
                return false;
            al = al->argumentList;
            pl = pl->paramList;
        } else {
            return areSameDataType(al->variableDefinition->dataType, getDataType(pl->value));
        }
    }
    return false;
}
