#include <compiler.h>

#include <scanner.h>

#include <stdio.h>


typedef struct Parser_tag Parser;
struct Parser_tag {
	Token previous;
	Token current;
	bool hasError;
	bool panicMode;
};

Parser parser;
Chunk* compilingChunk;

static Chunk* currentChunk() {
	return compilingChunk;
}

static void errorAt(Token* token, const char* message) {
	if(parser.panicMode) return;
	parser.panicMode = true;

	fprintf(stderr, "[line %d] Error", token->line);

	if(token->type == TOKEN_EOF) {
		fprintf(stderr, " at end");
	} else if(token->type != TOKEN_ERROR) {
		fprintf(stderr, " at '%*.s'", token->length, token->start);
	}

	fprintf(stderr, ": %s\n", message);
	parser.hasError = true;
}
static void error(const char* message) {
	errorAt(&parser.previous, message);
}
static void errorAtCurrent(const char* message) {
	errorAt(&parser.current, message);
}

static void advance() {
	parser.previous = parser.current;

	while(true) {
		parser.current = scanToken();
		if(parser.current.type != TOKEN_ERROR) break;

		errorAtCurrent(parser.current.start);
	}
}
static void consume(TokenType type, const char* message) {
	if(parser.current.type == type) {
		advance();
	} else {
		errorAtCurrent(message);
	}
}
static void emitByte(uint8_t byte) {
	writeChunk(currentChunk(), byte, parser.previous.line);
}
static void emitBytes(uint8_t byte1, uint8_t byte2) {
	emitByte(byte1);
	emitByte(byte2);
}
static void emitReturn() {
	emitByte(OP_RETURN);
}
static void endCompiler() {
	emitReturn();
}
static void expression() {}

static void initParser() {
	parser.hasError = false;
	parser.panicMode = false;
}
bool compile(const char* source, Chunk* chunk) {
	assert(source != NULL);
	assert(chunk != NULL);

	initParser();
	initScanner(source);
	compilingChunk = chunk;

	advance();
	expression();
	consume(TOKEN_EOF, "Expected end of expression");
	endCompiler();
	return parser.hasError;
}
