#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "chunk.h"

int main(int argc, char* argv[]) {

	Chunk chunk;
	initChunk(&chunk);
	writeChunk(&chunk, OP_RETURN);
	freeChunk(&chunk);

	printf("Terminated successfully!");

	return EXIT_SUCCESS;

}
