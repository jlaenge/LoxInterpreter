#ifndef CLOX_UTIL_DIRWALK_H_
#define CLOX_UTIL_DIRWALK_H_

#include <common.h>

#include <dirent.h>

typedef void(*DirwalkCallback)(struct dirent*);

void dirwalk_walk(const char* path, DirwalkCallback callback);

#endif
