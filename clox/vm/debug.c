#include <debug.h>

#include <value.h>

#include <stdio.h>

void disassembleChunk(Chunk* chunk, const char* name) {
	assert(chunk != NULL);
	assert(name != NULL);

	printf("== %s ==\n", name);
	int offset = 0;
	while(dynarrayInRange(&chunk->code, offset)) {
		offset = disassembleInstruction(chunk, offset);
	}

}

static int constantInstruction(const char* name, Chunk* chunk, int offset) {
	assert(chunk != NULL);
	uint8_t constant = *((uint8_t*)dynarrayGet(&chunk->code, offset+1));
	printf("%-16s %4d '", name, constant);
	Value value = *((Value*)dynarrayGet(&chunk->constants, constant));
	printValue(value);
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

	int currentLine = *((int*)dynarrayGet(&chunk->lines, offset));
	if(offset > 0) {
		int previousLine = *((int*)dynarrayGet(&chunk->lines, offset-1));

		if(offset > 0 && currentLine == previousLine) {
			printf("   | ");
		} else {
			printf("%4d ", currentLine);
		}
	} else {
		printf("%4d ", currentLine);
	}


	uint8_t instruction = *((uint8_t*)dynarrayGet(&chunk->code, offset));
	switch(instruction) {

		// values
		case OP_CONSTANT:	return constantInstruction("OP_CONSTANT", chunk, offset);
		case OP_NIL:		return simpleInstruction("OP_NIL", offset);
		case OP_TRUE:		return simpleInstruction("OP_TRUE", offset);
		case OP_FALSE:		return simpleInstruction("OP_FALSE", offset);

		// comparison
		case OP_EQUAL:		return simpleInstruction("OP_EQUAL", offset);
		case OP_LESS:		return simpleInstruction("OP_LESS", offset);
		case OP_GREATER:	return simpleInstruction("OP_GREATER", offset);

		// arithmetic
		case OP_NEGATE:		return simpleInstruction("OP_NEGATE", offset);
		case OP_ADD:		return simpleInstruction("OP_ADD", offset);
		case OP_SUBTRACT:	return simpleInstruction("OP_SUBTRACT", offset);
		case OP_MULTIPLY:	return simpleInstruction("OP_MULTIPLY", offset);
		case OP_DIVIDE:		return simpleInstruction("OP_DIVIDE", offset);

		// boolean
		case OP_NOT:		return simpleInstruction("OP_NOT", offset);

		// return statement
		case OP_POP:		return simpleInstruction("OP_POP", offset);
		case OP_PRINT:		return simpleInstruction("OP_PRINT", offset);
		case OP_RETURN:		return simpleInstruction("OP_RETURN", offset);

		// error handling
		default:
			printf("Unknown opcode %d\n", instruction);
			return offset + 1;

	}

}
