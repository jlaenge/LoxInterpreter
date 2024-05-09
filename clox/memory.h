#ifndef CLOX_MEMORY_H_
#define CLOX_MEMORY_H_

#include <common.h>

#define ALLOCATE(type, count) \
	(type*)reallocate(NULL, 0, sizeof(type) * count)

#define FREE(type, pointer) \
	reallocate(pointer, sizeof(type), 0)

#define FREE_ARRAY(type, pointer, count) \
	reallocate(pointer, sizeof(type)*count, 0)

/*
 * reallocates the size of the memory chunk pointed to by
 * pointer from a chunk of oldSize to one of newSize
 */
void* reallocate(void* pointer, size_t oldSize, size_t newSize);

/**
 * frees all allocated objects in the vm.
 */
void freeObjects();

#endif /* CLOX_MEMORY_H_ */
