#ifndef CLOX_CHUNK_H_
#define CLOX_CHUNK_H_

#include "common.h"
#include "memory.h"

typedef enum OpCode_tag OpCode;
enum OpCode_tag {
	OP_RETURN
};

/**
 * Chunk defines a sequence of bytecode instructions.
 * The count, capacity and code implement a dynamic array.
 */
typedef struct Chunk_tag Chunk;
struct Chunk_tag {
	int count;
	int capacity;
	uint8_t* code;
};

void initChunk(Chunk* chunk);
void freeChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte);

#endif
