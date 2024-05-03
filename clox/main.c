#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <common.h>
#include <chunk.h>
#include <debug.h>
#include <vm.h>

#define LINE_BUFFER_SIZE 1024

/**
 * Starts the read, evaluate, print-loop
 */
void repl();

/**
 * Runs file at given path
 */
void runFile(const char* path);

/**
 * Reads file at given path and returns it's content in a buffer.
 * ATTENTION: Caller is responsible of freeing the returned buffer.
 */
char* readFile(const char* path);

int main(int argc, char* argv[]) {

	// spin up VM
	initVM();

	// evaluate repl or program
	if(argc == 1) {
		repl();
	} else if(argc == 2) {
		runFile(argv[1]);
	} else {
		fprintf(stderr, "Usage: clox [path]\n");
		exit(64);
	}

	// terminate VM
	freeVM();

	printf("Terminated successfully!");
	return EXIT_SUCCESS;

}

void repl() {

	char line[LINE_BUFFER_SIZE];
	while(true) {

		printf(">");

		if(!fgets(line, sizeof(line), stdin)) {
			printf("\n");
			break;
		} else {
			line[strlen(line) - 1] = '\0';
		}

		interpret(line);

	}

}


void runFile(const char* path) {

	char* source = readFile(path);
	InterpretResult result = interpret(source);
	free(source);

	if(result == INTERPRET_COMPILE_ERROR) exit(65);
	if(result == INTERPRET_RUNTIME_ERROR) exit(70);

}

char* readFile(const char* path) {

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
