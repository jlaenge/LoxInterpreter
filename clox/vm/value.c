#include <value.h>

#include <memory.h>
#include <object.h>

#include <stdio.h>

#define MIN_ARRAY_SIZE 8

bool valuesEqual(Value a, Value b) {
	if(a.type != b.type) return false;

	switch(a.type) {
		case VALUE_BOOLEAN:	return (AS_BOOLEAN(a) == AS_BOOLEAN(b));
		case VALUE_NIL:		return true;
		case VALUE_NUMBER:	return (AS_NUMBER(a) == AS_NUMBER(b));
		default:			assert(false); return false;
	}
}
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

static void printObject(Object* object) {
	switch(object->type) {
		case OBJECT_STRING: printf("%s", ((ObjectString*)(object))->characters); break;
	}
}

void printValue(Value value) {
	switch(value.type) {
		case VALUE_BOOLEAN:	printf(AS_BOOLEAN(value) ? "true" : "false");	break;
		case VALUE_NIL:		printf("nil");									break;
		case VALUE_NUMBER:	printf("%g", AS_NUMBER(value));					break;
		case VALUE_OBJECT:	printObject(AS_OBJECT(value));					break;
	}

}
