#include <chunk.h>

#include <stdlib.h>

#define MIN_CHUNK_SIZE 8

void initChunk(Chunk* chunk) {
	assert(chunk != NULL);
	chunk->count = 0;
	chunk->capacity = 0;
	chunk->code = NULL;
	chunk->lines = NULL;
	initValueArray(&chunk->constants);
}
void freeChunk(Chunk* chunk) {
	assert(chunk != NULL);
	FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
	FREE_ARRAY(int, chunk->lines, chunk->capacity);
	freeValueArray(&chunk->constants);
	initChunk(chunk);
}

static void growChunk(Chunk* chunk) {
	assert(chunk != NULL);

	// start fresh chunk
	if(chunk->capacity == 0) {
		chunk->capacity = MIN_CHUNK_SIZE;
		chunk->code = reallocate(chunk->code, 0, chunk->capacity * sizeof(uint8_t));
		chunk->lines = reallocate(chunk->lines, 0, chunk->capacity * sizeof(int));
	}
	// grow old chunk
	else {
		int old_capacity = chunk->capacity;
		chunk->capacity <<= 1;
		chunk->code = reallocate(chunk->code, old_capacity * sizeof(uint8_t), chunk->capacity * sizeof(uint8_t));
		chunk->lines = reallocate(chunk->lines, old_capacity * sizeof(uint8_t), chunk->capacity * sizeof(uint8_t));
	}

}
void writeChunk(Chunk* chunk, uint8_t byte, int line) {
	assert(chunk != NULL);

	// allocate more memory, if necessary
	if(chunk->capacity <= chunk->count) {
		growChunk(chunk);
	}

	// write new chunk
	chunk->code[chunk->count] = byte;
	chunk->lines[chunk->count] = line;
	chunk->count++;
}

int addConstant(Chunk* chunk, Value value) {
	assert(chunk != NULL);
	writeValueArray(&chunk->constants, value);
	return (chunk->constants.count - 1);
}
