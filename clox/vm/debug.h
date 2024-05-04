#ifndef CLOX_DEBUG_H_
#define CLOX_DEBUG_H_

#include <common.h>
#include <chunk.h>

void disassembleChunk(Chunk* chunk, const char* name);
int disassembleInstruction(Chunk* chunk, int offset);

#endif
