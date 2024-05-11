#ifndef CLOX_VM_OBJECT_H_
#define CLOX_VM_OBJECT_H_

#include <common.h>
#include <value.h>

#define OBJECT_TYPE(value) (AS_OBJECT(value)->type)

#define IS_STRING(value) isObjectType(value, OBJECT_STRING)

#define AS_STRING(value) ((ObjectString*)AS_OBJECT(value))

#define AS_CSTRING(value) ( ((ObjectString*)AS_STRING(value))->characters )

typedef enum ObjectType_tag ObjectType;
enum ObjectType_tag {
	OBJECT_STRING
};

struct Object_tag {
	ObjectType type;
	Object* next;
};

struct ObjectString_tag {
	Object object;
	int length;
	char* characters;
	uint32_t hash;
};

ObjectString* copyString(const char* characters, int length);
ObjectString* takeString(char* characters, int length);

static inline bool isObjectType(Value value, ObjectType type) {
	return (IS_OBJECT(value) && OBJECT_TYPE(value) == type);
}

#endif
