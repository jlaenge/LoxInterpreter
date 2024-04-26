#include "chunk.h"

#include <stdlib.h>

#define MIN_CHUNK_SIZE 8

void initChunk(Chunk* chunk) {
	assert(chunk != NULL);
	chunk->count = 0;
	chunk->capacity = 0;
	chunk->code = NULL;
}
void freeChunk(Chunk* chunk) {
	assert(chunk != NULL);
	reallocate(chunk->code, chunk->capacity * sizeof(uint8_t), 0);
	initChunk(chunk);
}

static void growChunk(Chunk* chunk) {
	assert(chunk != NULL);

	// start fresh chunk
	if(chunk->capacity == 0) {
		chunk->capacity = MIN_CHUNK_SIZE;
		chunk->code = malloc(MIN_CHUNK_SIZE * sizeof(uint8_t));
	}
	// copy old entries
	else {
		int old_capacity = chunk->capacity;
		chunk->capacity <<= 1;
		reallocate(chunk->code, old_capacity * sizeof(uint8_t), chunk->capacity * sizeof(uint8_t));
	}

}
void writeChunk(Chunk* chunk, uint8_t byte) {
	assert(chunk != NULL);

	// allocate more memory, if necessary
	if(chunk->capacity <= chunk->count) {
		growChunk(chunk);
	}

	// write new chunk
	chunk->code[chunk->count] = byte;
	chunk->count++;
}
