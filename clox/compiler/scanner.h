#ifndef CLOX_SCANNER_H_
#define CLOX_SCANNER_H_

#include <common.h>

typedef enum TokenType_tag TokenType;
enum TokenType_tag {

	// single-character tokens
	TOKEN_LEFT_PARENTHESIS, TOKEN_RIGHT_PARENTHESIS,
	TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
	TOKEN_COMMA, TOKEN_DOT, TOKEN_SEMICOLON,
	TOKEN_MINUS, TOKEN_PLUS, TOKEN_STAR, TOKEN_SLASH,

	// one or two character tokens
	TOKEN_BANG, TOKEN_BANG_EQUAL,
	TOKEN_EQUAL, TOKEN_EQUAL_EQUAL,
	TOKEN_LESS, TOKEN_LESS_EQUAL,
	TOKEN_GREATER, TOKEN_GREATER_EQUAL,

	// literals
	TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_NUMBER,

	// keywords
	TOKEN_CLASS, TOKEN_SUPER, TOKEN_THIS,
	TOKEN_VAR, TOKEN_FUN, TOKEN_FOR, TOKEN_WHILE, TOKEN_RETURN, TOKEN_PRINT,
	TOKEN_AND, TOKEN_OR,
	TOKEN_IF, TOKEN_ELSE,
	TOKEN_TRUE, TOKEN_FALSE, TOKEN_NIL,

	// special
	TOKEN_ERROR, TOKEN_EOF

};

typedef struct Token_tag Token;
struct Token_tag {
	TokenType type;
	const char* start;
	int length;
	int line;
};

void initScanner(const char* source);
Token scanToken();

#endif
