#include <memory.h>

#include <object.h>
#include <vm.h>

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

static void freeObject(Object* object) {
	assert(object != NULL);
	switch(object->type) {
		case OBJECT_STRING: {
			ObjectString* objectString = (ObjectString*)object;
			FREE_ARRAY(char, objectString->characters, objectString->length + 1);
			FREE(ObjectString, object);
			break;
		}
	}
}
void freeObjects() {
	Object* object = vm.objects;
	while(object != NULL) {
		Object* next = object->next;
		freeObject(object);
		object = next;
	}
}
