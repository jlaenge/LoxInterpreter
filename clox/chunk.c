#include <chunk.h>

#include <memory.h>

#include <stdlib.h>

#define MIN_CHUNK_SIZE 8

void initChunk(Chunk* chunk) {
	assert(chunk != NULL);

	initDynamicArray(&chunk->code, sizeof(uint8_t));
	initDynamicArray(&chunk->lines, sizeof(int));
	initValueArray(&chunk->constants);
}
void freeChunk(Chunk* chunk) {
	assert(chunk != NULL);
	freeDynamicArray(&chunk->code);
	freeDynamicArray(&chunk->lines);
	freeValueArray(&chunk->constants);
	initChunk(chunk);
}

void writeChunk(Chunk* chunk, uint8_t byte, int line) {
	assert(chunk != NULL);

	dynarrayAppend(&chunk->code, &byte);
	dynarrayAppend(&chunk->lines, &line);
}

int addConstant(Chunk* chunk, Value value) {
	assert(chunk != NULL);
	writeValueArray(&chunk->constants, value);
	return (chunk->constants.count - 1);
}
