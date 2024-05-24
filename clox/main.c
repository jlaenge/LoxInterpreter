#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <common.h>
#include <chunk.h>
#include <debug.h>
#include <sourcefile.h>
#include <test.h>
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

int main(int argc, char* argv[]) {

	// run testsuite
	test_all();

	// spin up VM
	initVM(stdout, stderr);

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

	char* source = util_readFile(path);
	InterpretResult result = interpret(source);
	free(source);

	if(result == INTERPRET_COMPILE_ERROR) exit(65);
	if(result == INTERPRET_RUNTIME_ERROR) exit(70);

}

