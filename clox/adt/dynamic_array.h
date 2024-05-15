#ifndef CLOX_ADT_DYNAMIC_ARRAY_H_
#define CLOX_ADT_DYNAMIC_ARRAY_H_

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

void dynamicArrayAppend(DynamicArray* array, void* value);
void* dynamicArrayGet(DynamicArray* array, int index);
bool dynamicArrayIndexInRange(DynamicArray* array, int index);

#endif
