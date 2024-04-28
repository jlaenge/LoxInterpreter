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
	int constant = addConstant(&chunk, 1.2);
	writeChunk(&chunk, OP_CONSTANT, 123);
	writeChunk(&chunk, constant, 123);
	writeChunk(&chunk, OP_RETURN, 123);

	// disassemble and interpret program
	disassembleChunk(&chunk, "test chunk");
	interpret(&chunk);

	// terminate VM and free chunk
	freeVM();
	freeChunk(&chunk);

	printf("Terminated successfully!");
	return EXIT_SUCCESS;

}
