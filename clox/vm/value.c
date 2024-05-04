#include <value.h>

#include <memory.h>

#include <stdio.h>

#define MIN_ARRAY_SIZE 8

void initValueArray(ValueArray* array) {
	assert(array != NULL);
	array->count = 0;
	array->capacity = 0;
	array->values = NULL;
}
void freeValueArray(ValueArray* array) {
	assert(array != NULL);
	reallocate(array->values, array->capacity * sizeof(Value), 0);
	initValueArray(array);
}

static void growArray(ValueArray* array) {
	assert(array != NULL);

	// start fresh array
	if(array->capacity == 0) {
		array->capacity = MIN_ARRAY_SIZE;
		array->values = reallocate(array->values, 0, array->capacity * sizeof(Value));
	}
	// grow old array
	else {
		int old_capacity = array->capacity;
		array->capacity <<= 1;
		array->values = reallocate(array->values, old_capacity * sizeof(Value), array->capacity * sizeof(Value));
	}

}
void writeValueArray(ValueArray* array, Value value) {
	assert(array != NULL);

	if(array->capacity <= array->count) {
		growArray(array);
	}

	array->values[array->count] = value;
	array->count++;
}

void printValue(Value value) {
	printf("%g", AS_NUMBER(value));
}
