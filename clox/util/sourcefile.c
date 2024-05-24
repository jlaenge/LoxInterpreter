#include <sourcefile.h>

#include <stdio.h>
#include <stdlib.h>

char* util_readFile(const char* path) {

	// open file
	FILE* file = fopen(path, "rb");
	if(file == NULL) {
		fprintf(stderr, "Could not open file \"%s\".\n", path);
		exit(74);
	}

	// determin file size
	fseek(file, 0L, SEEK_END);
	size_t fileSize = ftell(file);
	rewind(file);

	// allocate memory
	char* buffer = (char*)malloc(fileSize + 1);
	if(buffer == NULL) {
		fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
		exit(74);
	}

	// read file
	size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
	if(bytesRead < fileSize) {
		fprintf(stderr, "Failed to read \"%s\".\n", path);
	}
	buffer[bytesRead] = '\0';

	// close file & return buffer
	fclose(file);
	return buffer;

}
