#include <value.h>

#include <memory.h>
#include <object.h>

#include <stdio.h>
#include <string.h>

#define MIN_ARRAY_SIZE 8

bool valuesEqual(Value a, Value b) {
	if(a.type != b.type) return false;

	switch(a.type) {
		case VALUE_BOOLEAN:	return (AS_BOOLEAN(a) == AS_BOOLEAN(b));
		case VALUE_NIL:		return true;
		case VALUE_NUMBER:	return (AS_NUMBER(a) == AS_NUMBER(b));
		case VALUE_OBJECT: 	return (AS_OBJECT(a) == AS_OBJECT(b));
		default:			assert(false); return false;
	}
}
void initValueArray(ValueArray* array) {
	assert(array != NULL);
	initDynamicArray(array, sizeof(Value));
}
void freeValueArray(ValueArray* array) {
	assert(array != NULL);
	freeDynamicArray(array);
	initValueArray(array);
}

void writeValueArray(ValueArray* array, Value value) {
	assert(array != NULL);

	dynamicArrayAppend(array, &value);
}

static void printObject(Value value) {
	switch(OBJECT_TYPE(value)) {
		case OBJECT_STRING: printf("%s", AS_CSTRING(value)); break;
	}
}

void printValue(Value value) {
	switch(value.type) {
		case VALUE_BOOLEAN:	printf(AS_BOOLEAN(value) ? "true" : "false");	break;
		case VALUE_NIL:		printf("nil");									break;
		case VALUE_NUMBER:	printf("%g", AS_NUMBER(value));					break;
		case VALUE_OBJECT:	printObject(value);								break;
	}

}
