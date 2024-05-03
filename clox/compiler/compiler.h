#ifndef CLOX_COMPILER_H_
#define CLOX_COMPILER_H_

#include <common.h>

#include <chunk.h>
#include <scanner.h>

bool compile(const char* source, Chunk* chunk);

#endif
