#ifndef CLOX_MEMORY_H_
#define CLOX_MEMORY_H_

#include <common.h>

/*
 * reallocates the size of the memory chunk pointed to by
 * pointer from a chunk of oldSize to one of newSize
 */
void* reallocate(void* pointer, size_t oldSize, size_t newSize);

#endif /* CLOX_MEMORY_H_ */
