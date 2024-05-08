#ifndef CLOX_MEMORY_H_
#define CLOX_MEMORY_H_

#include <common.h>

#define ALLOCATE(type, count) \
	(type*)reallocate(NULL, 0, sizeof(type) * count)

/*
 * reallocates the size of the memory chunk pointed to by
 * pointer from a chunk of oldSize to one of newSize
 */
void* reallocate(void* pointer, size_t oldSize, size_t newSize);

#endif /* CLOX_MEMORY_H_ */
