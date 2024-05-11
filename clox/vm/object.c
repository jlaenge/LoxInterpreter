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
static ObjectString* allocateString(char* characters, int length, uint32_t hash) {
	ObjectString* string = ALLOCATE_OBJECT(ObjectString, OBJECT_STRING);
	string->characters = characters;
	string->length = length;
	string->hash = hash;
	return string;
}
static uint32_t hashString(const char* characters, int length) {
	assert(characters != NULL);

	uint32_t hash = 2166136261u;
	for(int i=0; i<length; i++) {
		hash ^= characters[i];
		hash *= 16777619;
	}

	return hash;
}

ObjectString* copyString(const char* characters, int length) {
	char* heapMemory = ALLOCATE(char, length + 1);
	memcpy(heapMemory, characters, length);
	heapMemory[length] = '\0';
	uint32_t hash = hashString(characters, length);
	return allocateString(heapMemory, length, hash);
}
ObjectString* takeString(char* characters, int length) {
	uint32_t hash = hashString(characters, length);
	return allocateString(characters, length, hash);
}
