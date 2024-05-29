#ifndef CLOX_UTIL_DIRWALK_H_
#define CLOX_UTIL_DIRWALK_H_

#include <common.h>

#define MAX_FILENAME_LEN 64

typedef struct DirectoryEntry_tag DirectoryEntry;
struct DirectoryEntry_tag {
	long inodeNumber;
	char name[MAX_FILENAME_LEN];
};

typedef void(*DirwalkCallback)(DirectoryEntry*);

void dirwalk_walk(const char* path, DirwalkCallback callback);

#endif
