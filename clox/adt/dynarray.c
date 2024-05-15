#include <dynarray.h>
#include <dynarray_internal.h>

#include <memory.h>

#include <string.h>

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

void dynarrayAppend(DynamicArray* array, void* value) {
	assert(array != NULL);

	// allocate more memory, if necessary
	if(array->capacity <= array->count) {
		int oldCapacity = array->capacity;
		array->capacity = RESIZE_CAPACITY(array->capacity);
		array->memory = RESIZE_MEMORY(array->entrySize, array->memory, oldCapacity, array->capacity);
	}

	// write new chunk
	void* memory = array->memory + (array->entrySize * array->count);
	memcpy(memory, value, array->entrySize);
	array->count++;
}
void* dynarrayGet(DynamicArray* array, int index) {
	assert(array != NULL);
	assert(0 <= index);
	assert(index < array->capacity);

	return array->memory + (array->entrySize * index);
}
bool dynarrayInRange(DynamicArray* array, int index) {
	assert(array != NULL);
	return (0 <= index && index < array->count);
}
