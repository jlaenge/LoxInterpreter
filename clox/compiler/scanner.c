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
static char advance() {
	char current = *scanner.current;
	scanner.current++;
	return current;
}
static char peek() {
	return *scanner.current;
}
static char peekNext() {
	if(isAtEnd()) return '\0';
	else return scanner.current[1];
}
static bool match(char expected) {
	if(isAtEnd()) return false;
	if(*scanner.current != expected) return false;
	scanner.current++;
	return true;
}
static void skipWhitespaces() {
	while(true) {
		char c = peek();
		switch(c) {

			// whitespaces
			case ' ':
			case '\r':
			case '\t':
				advance();
				break;

			// comments
			case '/':
				if(peekNext() == '/') {
					while(!isAtEnd() && peek() != '\n') advance();
				} else {
					return;
				}
				break;

			// newlines

			default:
				return;
		}
	}
}

Token scanToken() {
	skipWhitespaces();

	scanner.start = scanner.current;

	if(isAtEnd()) return makeToken(EOF);

	char c = advance();
	switch(c) {

		// single-character tokens
		case '(': return makeToken(TOKEN_LEFT_PARENTHESIS);
		case ')': return makeToken(TOKEN_RIGHT_PARENTHESIS);
		case '{': return makeToken(TOKEN_LEFT_BRACE);
		case '}': return makeToken(TOKEN_RIGHT_BRACE);
		case ',': return makeToken(TOKEN_COMMA);
		case '.': return makeToken(TOKEN_DOT);
		case ';': return makeToken(TOKEN_SEMICOLON);
		case '+': return makeToken(TOKEN_PLUS);
		case '-': return makeToken(TOKEN_MINUS);
		case '*': return makeToken(TOKEN_STAR);
		case '/': return makeToken(TOKEN_SLASH);

		// one or two character tokens
		case '!': return makeToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
		case '=': return makeToken(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
		case '<': return makeToken(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
		case '>': return makeToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);

	}

	return errorToken("Unexpected character.");
}
