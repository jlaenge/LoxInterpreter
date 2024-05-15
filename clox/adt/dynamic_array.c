#include <dynamic_array.h>

#include <memory.h>

#include <string.h>

#define MIN_CAPACITY 8
#define RESIZE_FACTOR 2

#define RESIZE_CAPACITY(capacity) \
	((capacity < MIN_CAPACITY) ? MIN_CAPACITY : RESIZE_FACTOR*capacity)
#define RESIZE_MEMORY(entrySize, pointer, oldCount, newCount) \
	(reallocate(pointer, entrySize*oldCount, entrySize*newCount))

void initDynamicArray(DynamicArray* array, size_t entrySize) {
	assert(array != NULL);

	array->entrySize = entrySize;
	array->memory = NULL;
	array->capacity = 0;
	array->count = 0;
}
void freeDynamicArray(DynamicArray* array) {
	assert(array != NULL);

	FREE_ARRAY(array->entrySize, array->memory, array->capacity);
	initDynamicArray(array, array->entrySize);
}

void dynamicArrayAppend(DynamicArray* array, void* value) {
	assert(array != NULL);

	// allocate more memory, if necessary
	if(array->capacity <= array->count) {
		int oldCapacity = array->capacity;
		array->capacity = RESIZE_CAPACITY(array->capacity);
		array->memory = RESIZE_MEMORY(array->entrySize, array->memory, oldCapacity, array->capacity);
	}

	// write new chunk
	void* memory = dynamicArrayGet(array, array->count);
	memcpy(memory, value, array->entrySize);
	array->count++;
}
void* dynamicArrayGet(DynamicArray* array, int index) {
	assert(array != NULL);
	assert(0 <= index);
	assert(index < array->capacity);

	return array->memory + (array->entrySize * index);
}
