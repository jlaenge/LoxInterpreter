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

void writeChunk(Chunk* chunk, uint8_t byte, int line) {
	assert(chunk != NULL);

	// allocate more memory, if necessary
	if(chunk->capacity <= chunk->count) {
		int oldCapacity = chunk->capacity;
		chunk->capacity = GROW_CAPACITY(chunk->capacity);
		chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
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
