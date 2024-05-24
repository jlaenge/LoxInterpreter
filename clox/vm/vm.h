#ifndef CLOX_VM_H_
#define CLOX_VM_H_

#include <common.h>
#include <chunk.h>
#include <table.h>
#include <value.h>

#include <stdio.h>

#define STACK_MAX 256

typedef struct VM_tag VM;
struct VM_tag {
	FILE* output;
	FILE* error;

	Chunk* chunk;
	uint8_t* ip;
	Value stack[STACK_MAX];
	Value* stackTop;
	Table strings;

	Object* objects;
};

extern VM vm;

typedef enum InterpretResult_tag InterpretResult;
enum InterpretResult_tag {
	INTERPRET_OK,
	INTERPRET_COMPILE_ERROR,
	INTERPRET_RUNTIME_ERROR
};

void initVM(FILE* output, FILE* error);
void freeVM();
InterpretResult interpret(const char* source);
void push(Value value);
Value pop();
Value peek(int distance);
bool isFalsey(Value value);
void concatenate();

#endif
