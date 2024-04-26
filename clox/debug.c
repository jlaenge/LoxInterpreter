#include "debug.h"

#include "value.h"

#include <stdio.h>

void disassembleChunk(Chunk* chunk, const char* name) {
	assert(chunk != NULL);
	assert(name != NULL);

	printf("== %s ==\n", name);
	int offset = 0;
	while(offset < chunk->count) {
		offset = disassembleInstruction(chunk, offset);
	}

}

static int constantInstruction(const char* name, Chunk* chunk, int offset) {
	assert(chunk != NULL);
	uint8_t constant = chunk->code[offset + 1];
	printf("%-16s %4d '", name, constant);
	printValue(chunk->constants.values[constant]);
	printf("'\n");
	return offset + 2;
}
static int simpleInstruction(const char* name, int offset) {
	printf("%s\n", name);
	return offset + 1;
}

int disassembleInstruction(Chunk* chunk, int offset) {
	assert(chunk != NULL);

	printf("%04d ", offset);

	uint8_t instruction = chunk->code[offset];
	switch(instruction) {
		case OP_CONSTANT:
			return constantInstruction("OP_CONSTANT", chunk, offset);
		case OP_RETURN:
			return simpleInstruction("OP_RETURN", offset);
		default:
			printf("Unknown opcode %d\n", instruction);
			return offset + 1;
	}

}
