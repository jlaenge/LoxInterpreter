#ifndef CLOX_VM_H_
#define CLOX_VM_H_

#include "common.h"

#include "chunk.h"

typedef struct VM_tag VM;
struct VM_tag {
	Chunk* chunk;
	uint8_t* ip;
};

typedef enum InterpretResult_tag InterpretResult;
enum InterpretResult_tag {
	INTERPRET_OK,
	INTERPRET_COMPILE_ERROR,
	INTERPRET_RUNTIME_ERROR
};

void initVM();
void freeVM();
InterpretResult interpret(Chunk* chunk);

#endif
