#include <sourcefile.h>

#include <vm.h>

#include <stdio.h>
#include <stdlib.h>

static char* readFile(const char* path) {

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

void sourcefile_run(const char* path) {

	char* source = readFile(path);
	InterpretResult result = interpret(source);
	free(source);

	if(result == INTERPRET_COMPILE_ERROR) exit(65);
	if(result == INTERPRET_RUNTIME_ERROR) exit(70);

}
