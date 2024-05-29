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

#include <errno.h>





/**************************************\
 # STRUCTURE DEFINITIONS
\**************************************/

typedef struct Directory_tag Directory;
struct Directory_tag {
	int filedescriptor;
	DirectoryEntry* entry;
};

DirectoryEntry* createDirectoryEntry(long inodeNumber, const char* name) {

	DirectoryEntry* entry = malloc(sizeof(DirectoryEntry));
	assert(entry != NULL);

	entry->inodeNumber = inodeNumber;
	strncpy(entry->name, name, MAX_FILENAME_LEN);
	return entry;

}
Directory* createDirectory(int filedescriptor) {

	Directory* directory = malloc(sizeof(Directory));
	assert(directory != NULL);

	directory->filedescriptor = filedescriptor;
	return directory;

}





/**************************************\
 # LOW-LEVEL DIRECTORY FUNCTIONS
\**************************************/

static Directory* openDirectory(const char* path) {
	assert(path != NULL);

	int filedescriptor = open(path, O_RDONLY, 0);
	assert(filedescriptor != -1);

	struct stat statusBuffer;
	int status = fstat(filedescriptor, &statusBuffer);
	assert(status != -1);

	// check if file is a directory
	if((statusBuffer.st_mode & S_IFMT) != S_IFDIR) {
		return NULL;
	}

	Directory* directory = createDirectory(filedescriptor);
	return directory;

}
static void closeDirectory(Directory* directory) {
	assert(directory != NULL);

	close(directory->filedescriptor);
	free(directory);

}
static DirectoryEntry* readEntry(Directory* directory) {
	assert(directory != NULL);

	struct direct directoryBuffer;

	while(true) {

		int length = read(directory->filedescriptor, &directoryBuffer, sizeof(struct direct));

		int err = errno;
		assert(err != EAGAIN);

		// quit if at end of buffer
		if(length != sizeof(struct direct)) return NULL;

		// skip unused slots
		if(directoryBuffer.d_ino == 0) continue;

		// create and return entry
		DirectoryEntry* entry = createDirectoryEntry(directoryBuffer.d_ino, directoryBuffer.d_name);
		return entry;

	}

}





/**************************************\
 # HIGH-LEVEL DIRECTORY FUNCTIONS
\**************************************/

void dirwalk_walk(const char* path, DirwalkCallback callback) {
	assert(path != NULL);
	assert(callback != NULL);

	opendi

	Directory* directory = openDirectory(path);
	assert(directory != NULL);

	for(
		DirectoryEntry* entry = readEntry(directory);
		entry != NULL;
		entry = readEntry(directory)
	) {

		// skip self and parent
		if(strcmp(entry->name, ".") == 0 || strcmp(entry->name, "..") == 0) {
			continue;
		}

		// process entry
		callback(entry);

	}

	closeDirectory(directory);

}
