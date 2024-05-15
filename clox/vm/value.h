#ifndef CLOX_VALUE_H_
#define CLOX_VALUE_H_

#include <common.h>

#include <dynarray.h>

typedef enum ValueType_tag ValueType;
enum ValueType_tag {
	VALUE_BOOLEAN,
	VALUE_NIL,
	VALUE_NUMBER,
	VALUE_OBJECT
};

typedef struct Object_tag Object;
typedef struct ObjectString_tag ObjectString;

typedef struct Value_tag Value;
struct Value_tag {
	ValueType type;
	union {
		bool boolean;
		double number;
		Object* object;
	} as;
};

#define BOOLEAN_VALUE(value) ((Value){ VALUE_BOOLEAN, { .boolean = value } })
#define NIL_VALUE			 ((Value){ VALUE_NIL })
#define NUMBER_VALUE(value)	 ((Value){ VALUE_NUMBER, { .number = value } })
#define OBJECT_VALUE(value)  ((Value){ VALUE_OBJECT, { .object = (Object*)value } })

#define IS_BOOLEAN(value) ((value).type == VALUE_BOOLEAN)
#define IS_NIL(value)	  ((value).type == VALUE_NIL)
#define IS_NUMBER(value)  ((value).type == VALUE_NUMBER)
#define IS_OBJECT(value)  ((value).type == VALUE_OBJECT)

#define AS_BOOLEAN(value) ((value).as.boolean)
#define AS_NUMBER(value)  ((value).as.number)
#define AS_OBJECT(value)  ((value).as.object)

typedef DynamicArray ValueArray;

bool valuesEqual(Value a, Value b);
void initValueArray(ValueArray* array);
void freeValueArray(ValueArray* array);
void writeValueArray(ValueArray* array, Value value);

void printValue(Value value);

#endif
