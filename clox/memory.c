#include <memory.h>

#include <stdlib.h>

void* reallocate(void* pointer, size_t oldSize, size_t newSize) {
	void* result = NULL;

	if(newSize == 0) {
		free(pointer);
	} else {
		result = realloc(pointer, newSize);
		assert(result != NULL);
	}

	return result;
}
