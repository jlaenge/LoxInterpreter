#include "compiler.h"

#include "scanner.h"

#include <stdio.h>

void compile(const char* source) {
	assert(source != NULL);

	initScanner(source);
	int line = -1;

	Token token;
	do {

		token = scanToken();
		if(token.line != line) {
			line = token.line;
			printf("%4d ", line);
		} else {
			printf("   | ");
		}
		printf("%2d '%*.s'\n", token.type, token.length, token.start);

	} while(token.type != TOKEN_EOF);

}
