/******************************************************************************\
 * dirwalk.c
 *
 * The implementation follows "The C Programming Language",
 * Chapter "8.6 Example - Listing Directories"
\******************************************************************************/

#include <dirwalk.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dir.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void dirwalk_walk(const char* path, DirwalkCallback callback) {
	assert(path != NULL);
	assert(callback != NULL);

	DIR* dir = opendir(path);
	assert(dir != NULL);

	for(struct dirent* entry=readdir(dir); entry != NULL; entry=readdir(dir)) {
		if(strcmp(entry->d_name, ".") == 0) continue;
		if(strcmp(entry->d_name, "..") == 0) continue;
		callback(entry);
	}

	closedir(dir);
}
