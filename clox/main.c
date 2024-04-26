#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "chunk.h"
#include "debug.h"

int main(int argc, char* argv[]) {

	Chunk chunk;
	initChunk(&chunk);

	writeChunk(&chunk, OP_RETURN);
	int constant = addConstant(&chunk, 1.2);
	writeChunk(&chunk, OP_CONSTANT);
	writeChunk(&chunk, constant);

	disassembleChunk(&chunk, "test chunk");
	freeChunk(&chunk);

	printf("Terminated successfully!");

	return EXIT_SUCCESS;

}
