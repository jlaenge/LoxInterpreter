#ifndef CLOX_VM_H_
#define CLOX_VM_H_

#include <common.h>
#include <chunk.h>
#include <value.h>

#define STACK_MAX 256

typedef struct VM_tag VM;
struct VM_tag {
	Chunk* chunk;
	uint8_t* ip;
	Value stack[STACK_MAX];
	Value* stackTop;
};

typedef enum InterpretResult_tag InterpretResult;
enum InterpretResult_tag {
	INTERPRET_OK,
	INTERPRET_COMPILE_ERROR,
	INTERPRET_RUNTIME_ERROR
};

void initVM();
void freeVM();
InterpretResult interpret(const char* source);
void push(Value value);
Value pop();
Value peek(int distance);

#endif
