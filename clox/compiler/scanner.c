#include <scanner.h>

#include <stdio.h>
#include <string.h>

typedef struct Scanner_tag Scanner;
struct Scanner_tag {
	const char* start;
	const char* current;
	int line;
};

Scanner scanner;

void initScanner(const char* source) {
	assert(source != NULL);
	scanner.start = source;
	scanner.current = scanner.start;
	scanner.line = 1;
}

static bool isAtEnd() {
	return (*scanner.current == '\0');
}
static Token makeToken(TokenType type) {
	Token token;
	token.type = type;
	token.start = scanner.start;
	token.length = (int)(scanner.current - scanner.start);
	token.line = scanner.line;

	return token;
}
static Token errorToken(const char* message) {
	assert(message != NULL);
	Token token;
	token.type = TOKEN_ERROR;
	token.start = message;
	token.length = (int)strlen(message);
	token.line = scanner.line;
	return token;
}

Token scanToken() {
	scanner.start = scanner.current;

	if(isAtEnd()) return makeToken(EOF);

	return errorToken("Unexpected character.");
}
