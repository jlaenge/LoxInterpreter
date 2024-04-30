#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

int main(int argc, char* argv[]) {

	// spin up VM
	initVM();

	// create program chunk
	Chunk chunk;
	initChunk(&chunk);

	// create program
	int constant1 = addConstant(&chunk, 1.2);
	writeChunk(&chunk, OP_CONSTANT, 123);
	writeChunk(&chunk, constant1, 123);

	int constant2 = addConstant(&chunk, 3.4);
	writeChunk(&chunk, OP_CONSTANT, 123);
	writeChunk(&chunk, constant2, 123);

	writeChunk(&chunk, OP_ADD, 123);

	int constant3 = addConstant(&chunk, 5.6);
	writeChunk(&chunk, OP_CONSTANT, 123);
	writeChunk(&chunk, constant3, 123);

	writeChunk(&chunk, OP_DIVIDE, 123);
	writeChunk(&chunk, OP_NEGATE, 123);
	writeChunk(&chunk, OP_RETURN, 123);

	// disassemble and interpret program
	printf("Dissassembly:\n");
	disassembleChunk(&chunk, "test chunk");
	printf("Interpretation:\n");
	interpret(&chunk);

	// terminate VM and free chunk
	freeVM();
	freeChunk(&chunk);

	printf("Terminated successfully!");
	return EXIT_SUCCESS;

}
