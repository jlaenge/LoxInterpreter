#ifndef CLOX_CHUNK_H_
#define CLOX_CHUNK_H_

#include <common.h>
#include <dynarray.h>
#include <value.h>

typedef enum OpCode_tag OpCode;
enum OpCode_tag {

	// values
	OP_CONSTANT,
	OP_NIL,
	OP_TRUE,
	OP_FALSE,

	// comparison
	OP_EQUAL,
	OP_GREATER,
	OP_LESS,

	// arithmetic
	OP_NEGATE,
	OP_ADD,
	OP_SUBTRACT,
	OP_MULTIPLY,
	OP_DIVIDE,

	// boolean
	OP_NOT,

	// misc
	OP_PRINT,
	OP_RETURN

};

/**
 * Chunk defines a sequence of bytecode instructions.
 * The count, capacity and code implement a dynamic array.
 */
typedef struct Chunk_tag Chunk;
struct Chunk_tag {
	DynamicArray code;
	DynamicArray lines;
	ValueArray constants;
};

void initChunk(Chunk* chunk);
void freeChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, int line);
int addConstant(Chunk* chunk, Value value);

#endif
