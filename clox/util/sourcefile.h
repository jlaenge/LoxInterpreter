#ifndef CLOX_UTIL_SOURCEFILE_H_
#define CLOX_UTIL_SOURCEFILE_H_

#include <common.h>

/**
 * Reads file at given path and returns it's content in a buffer.
 * ATTENTION: Caller is responsible of freeing the returned buffer.
 */
char* util_readFile(const char* path);

#endif /* CLOX_UTIL_SOURCEFILE_H_ */
