#include <chunk.h>

#include <memory.h>

#include <stdlib.h>

#define MIN_CHUNK_SIZE 8

void initChunk(Chunk* chunk) {
	assert(chunk != NULL);

	initDynamicArray(&chunk->code, sizeof(uint8_t));
	chunk->count = 0;
	chunk->capacity = 0;
	chunk->lines = NULL;
	initValueArray(&chunk->constants);
}
void freeChunk(Chunk* chunk) {
	assert(chunk != NULL);
	freeDynamicArray(&chunk->code);
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
		chunk->lines = GROW_ARRAY(int, chunk->lines, oldCapacity, chunk->capacity);
	}

	// write new chunk
	dynamicArrayAppend(&chunk->code, &byte);
	chunk->lines[chunk->count] = line;
	chunk->count++;
}

int addConstant(Chunk* chunk, Value value) {
	assert(chunk != NULL);
	writeValueArray(&chunk->constants, value);
	return (chunk->constants.count - 1);
}
