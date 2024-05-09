#include <object.h>

#include <memory.h>
#include <vm.h>

#include <stdio.h>
#include <string.h>

#define ALLOCATE_OBJECT(type, objectType) \
	(type*)allocateObject(sizeof(type), objectType)

static Object* allocateObject(size_t size, ObjectType type) {
	Object* object = (Object*)reallocate(NULL, 0, size);
	object->type = type;

	object->next = vm.objects;
	vm.objects = object;

	return object;
}
static ObjectString* allocateString(char* characters, int length) {
	ObjectString* string = ALLOCATE_OBJECT(ObjectString, OBJECT_STRING);
	string->characters = characters;
	string->length = length;
	return string;
}

ObjectString* copyString(const char* characters, int length) {
	char* heapMemory = ALLOCATE(char, length + 1);
	memcpy(heapMemory, characters, length);
	heapMemory[length] = '\0';

	return allocateString(heapMemory, length);
}
ObjectString* takeString(char* characters, int length) {
	return allocateString(characters, length);
}
