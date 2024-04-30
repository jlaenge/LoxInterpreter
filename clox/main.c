#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

#include "programs.h"

int main(int argc, char* argv[]) {

	// spin up VM
	initVM();

	// create program chunk
	Chunk chunk;
	initChunk(&chunk);

	// create program
	create_program1(&chunk);

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
