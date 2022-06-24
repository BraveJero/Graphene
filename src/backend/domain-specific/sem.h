#ifndef __SEM_H__
#define __SEM_H__

#include "../support/shared.h"

extern CompilerState state;

DataType* getDataType(Value* v);
DataType* getDataTypeFromDataTypeInstance(DataTypeInstance* dti);
DataType* getInnerDataTypeFromCollectionType(CollectionType* ct);
DataType* getInnerDataTypeFromExtraDataType(ExtraDataType* edt);
DataType* getDataTypeFromCollectionType(CollectionType* dt);
DataType* getDataTypeFromExtraDataType(ExtraDataType* dt);
DataType* getDataTypeFromPrimitiveDataType(PrimitiveDataType* dt);

boolean isIntegerType(DataType* dt);
boolean isCollectionType(DataType* dt);
boolean isNumericType(DataType* dt);
boolean isStringType(DataType* dt);
boolean isExtraDataType(DataType* dt);
boolean isGraphType(DataType* dt);
boolean isStackType(DataType* dt);
boolean isSetType(DataType* dt);
boolean isQueueType(DataType* dt);
boolean matchArgumentDefinitionToParamDefinition(ArgumentDefinition* ad, ParamDefinition* pd);
boolean areSameDataType(DataType* dt1, DataType* dt2);

#endif
