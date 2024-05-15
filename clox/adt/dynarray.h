#ifndef CLOX_ADT_DYNARRAY_H_
#define CLOX_ADT_DYNARRAY_H_

#include <common.h>

typedef struct DynamicArray_tag DynamicArray;
struct DynamicArray_tag {
	size_t entrySize;
	void* memory;
	int capacity;
	int count;
};

void initDynamicArray(DynamicArray* array, size_t entrySize);
void freeDynamicArray(DynamicArray* array);

void dynarrayAppend(DynamicArray* array, void* value);
void* dynarrayGet(DynamicArray* array, int index);
bool dynarrayInRange(DynamicArray* array, int index);

#endif
