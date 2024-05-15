#ifndef CLOX_ADT_DYNARRAY_INTERNAL_H_
#define CLOX_ADT_DYNARRAY_INTERNAL_H_

#define MIN_CAPACITY 8
#define RESIZE_FACTOR 2

#define RESIZE_CAPACITY(capacity) \
	((capacity < MIN_CAPACITY) ? MIN_CAPACITY : RESIZE_FACTOR*capacity)
#define RESIZE_MEMORY(entrySize, pointer, oldCount, newCount) \
	(reallocate(pointer, entrySize*oldCount, entrySize*newCount))

#endif
