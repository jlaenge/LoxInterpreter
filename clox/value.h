#ifndef CLOX_VALUE_H_
#define CLOX_VALUE_H_

#include "common.h"

typedef double Value;

typedef struct ValueArray_tag ValueArray;
struct ValueArray_tag {
	int count;
	int capacity;
	Value* values;
};

void initValueArray(ValueArray* array);
void freeValueArray(ValueArray* array);
void writeValueArray(ValueArray* array, Value value);

void printValue(Value value);

#endif
