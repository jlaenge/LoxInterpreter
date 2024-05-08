#include <compiler.h>

#include <object.h>
#include <scanner.h>
#include <value.h>

#ifdef DEBUG_PRINT_CODE
	#include <debug.h>
#endif

#include <stdio.h>
#include <stdlib.h>


typedef struct Parser_tag Parser;
struct Parser_tag {
	Token previous;
	Token current;
	bool hasError;
	bool panicMode;
};

typedef enum Precedence_tag Precedence;
enum Precedence_tag {
	PRECEDENCE_NONE,
	PRECEDENCE_ASSIGNMENT,	// =
	PRECEDENCE_OR,			// or
	PRECEDENCE_AND,			// and
	PRECEDENCE_EQUALITY,	// ==, !=
	PRECEDENCE_COMPARISON,	// <, <=, >=, >
	PRECEDENCE_TERM,		// +, -
	PRECEDENCE_FACTOR,		// *, /
	PRECEDENCE_UNARY,		// !, -
	PRECEDENCE_CALL,		// (), .
	PRECEDENCE_PRIMARY
};

typedef void (*ParseFunction)(void);
typedef struct ParseRule_tag ParseRule;
struct ParseRule_tag {
	ParseFunction prefix;
	ParseFunction infix;
	Precedence precedence;
};

Parser parser;
Chunk* compilingChunk;

static Chunk* currentChunk() {
	return compilingChunk;
}





/**************************************\
 # ERROR FUNCTIONS
\**************************************/

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





/**************************************\
 # PARSER FUNCTIONS
\**************************************/

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

static uint8_t makeConstant(Value value) {
	int constant = addConstant(currentChunk(), value);
	if(constant > UINT8_MAX) {
		error("Too many constants in current chunk.");
		return 0;
	}
	return constant;
}





/**************************************\
 # EMITTER FUNCTIONS
\**************************************/

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

static void emitConstant(Value value) {
	emitBytes(OP_CONSTANT, makeConstant(value));
}

static void endCompiler() {
	emitReturn();

#ifdef DEBUG_PRINT_CODE
	if(!parser.hasError) {
		disassembleChunk(currentChunk(), "code");
	}
#endif
}





/**************************************\
 # COMPILE FUNCTIONS
\**************************************/
static ParseRule* getRule(TokenType type);

static void parsePrecedence(Precedence precedence) {
	advance();

	ParseFunction prefixFunction = getRule(parser.previous.type)->prefix;
	if(prefixFunction == NULL) {
		error("Expected expression.");
		return;
	}
	prefixFunction();

	while(precedence <= getRule(parser.current.type)->precedence) {
		advance();
		ParseFunction infixFunction = getRule(parser.previous.type)->infix;
		infixFunction();
	}

}

static void expression() {
	parsePrecedence(PRECEDENCE_ASSIGNMENT);
}
static void grouping() {
	expression();
	consume(TOKEN_RIGHT_PARENTHESIS, "Expected ')' after expression.");
}
static void unary() {
	TokenType operator = parser.previous.type;

	parsePrecedence(PRECEDENCE_UNARY);
	switch(operator) {
		case TOKEN_BANG:	emitByte(OP_NOT);		break;
		case TOKEN_MINUS:	emitByte(OP_NEGATE);	break;
		default:			assert(false);			break;
	}
}
static void binary() {
	TokenType operator = parser.previous.type;

	ParseRule* rule = getRule(operator);
	parsePrecedence((Precedence)(rule->precedence + 1));
	switch(operator) {

		// arithmetic
		case TOKEN_PLUS:	emitByte(OP_ADD); 		break;
		case TOKEN_MINUS:	emitByte(OP_SUBTRACT); 	break;
		case TOKEN_STAR:	emitByte(OP_MULTIPLY); 	break;
		case TOKEN_SLASH:	emitByte(OP_DIVIDE); 	break;

		// comparison
		case TOKEN_BANG_EQUAL:		emitBytes(OP_EQUAL, OP_NOT);	break;
		case TOKEN_EQUAL_EQUAL:		emitByte(OP_EQUAL);				break;
		case TOKEN_LESS:			emitByte(OP_LESS);				break;
		case TOKEN_LESS_EQUAL:		emitBytes(OP_GREATER, OP_NOT);	break;
		case TOKEN_GREATER_EQUAL:	emitBytes(OP_LESS, OP_NOT);		break;
		case TOKEN_GREATER:			emitByte(OP_GREATER);			break;

		// error
		default:			assert(false);			break;

	}
}
static void literal() {
	switch(parser.previous.type) {
		case TOKEN_NIL: 	emitByte(OP_NIL); 	break;
		case TOKEN_TRUE: 	emitByte(OP_TRUE); 	break;
		case TOKEN_FALSE: 	emitByte(OP_FALSE); break;
		default: 			assert(false); 		break;
	}
}
static void number() {
	double value = strtod(parser.previous.start, NULL);
	emitConstant(NUMBER_VALUE(value));
}
static void string() {
	ObjectString* objectString = copyString(parser.previous.start + 1, parser.previous.length - 2);
	emitConstant(OBJECT_VALUE(objectString));
}

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
	return !parser.hasError;
}

ParseRule rules[] = {
	[TOKEN_LEFT_PARENTHESIS] 		= { grouping, 	NULL, 	PRECEDENCE_NONE 		},
	[TOKEN_RIGHT_PARENTHESIS]		= { NULL, 		NULL, 	PRECEDENCE_NONE 		},
	[TOKEN_LEFT_BRACE] 				= { NULL, 		NULL, 	PRECEDENCE_NONE 		},
	[TOKEN_RIGHT_BRACE]				= { NULL, 		NULL, 	PRECEDENCE_NONE 		},
	[TOKEN_COMMA] 					= { NULL, 		NULL, 	PRECEDENCE_NONE 		},
	[TOKEN_DOT] 					= { NULL, 		NULL, 	PRECEDENCE_NONE 		},
	[TOKEN_SEMICOLON]				= { NULL, 		NULL, 	PRECEDENCE_NONE 		},
	[TOKEN_MINUS] 			  		= { unary, 		binary, PRECEDENCE_TERM 		},
	[TOKEN_PLUS] 					= { NULL, 		binary, PRECEDENCE_TERM 		},
	[TOKEN_STAR] 					= { NULL, 		binary, PRECEDENCE_FACTOR		},
	[TOKEN_SLASH]					= { NULL, 		binary, PRECEDENCE_FACTOR 		},
	[TOKEN_BANG] 				    = { unary, 		NULL, 	PRECEDENCE_NONE 		},
	[TOKEN_BANG_EQUAL]				= { NULL, 		binary,	PRECEDENCE_COMPARISON	},
	[TOKEN_EQUAL] 			   		= { NULL, 		NULL, 	PRECEDENCE_NONE 		},
	[TOKEN_EQUAL_EQUAL]				= { NULL, 		binary,	PRECEDENCE_COMPARISON	},
	[TOKEN_LESS] 			   		= { NULL, 		binary,	PRECEDENCE_COMPARISON	},
	[TOKEN_LESS_EQUAL]				= { NULL, 		binary,	PRECEDENCE_COMPARISON	},
	[TOKEN_GREATER] 		   		= { NULL, 		binary,	PRECEDENCE_COMPARISON	},
	[TOKEN_GREATER_EQUAL]			= { NULL, 		binary,	PRECEDENCE_COMPARISON	},
	[TOKEN_IDENTIFIER] 	   			= { NULL, 		NULL, 	PRECEDENCE_NONE 		},
	[TOKEN_STRING] 					= { string,		NULL, 	PRECEDENCE_NONE 		},
	[TOKEN_NUMBER]					= { number, 	NULL, 	PRECEDENCE_NONE 		},
	[TOKEN_CLASS] 				   	= { NULL, 		NULL, 	PRECEDENCE_NONE 		},
	[TOKEN_SUPER] 					= { NULL, 		NULL, 	PRECEDENCE_NONE 		},
	[TOKEN_THIS]					= { NULL, 		NULL, 	PRECEDENCE_NONE 		},
	[TOKEN_VAR] 					= { NULL, 		NULL, 	PRECEDENCE_NONE 		},
	[TOKEN_FUN] 					= { NULL, 		NULL, 	PRECEDENCE_NONE 		},
	[TOKEN_FOR] 					= { NULL, 		NULL, 	PRECEDENCE_NONE 		},
	[TOKEN_WHILE] 					= { NULL, 		NULL, 	PRECEDENCE_NONE 		},
	[TOKEN_RETURN] 					= { NULL, 		NULL, 	PRECEDENCE_NONE 		},
	[TOKEN_PRINT]					= { NULL, 		NULL, 	PRECEDENCE_NONE 		},
	[TOKEN_AND] 				   	= { NULL, 		NULL, 	PRECEDENCE_NONE 		},
	[TOKEN_OR]						= { NULL, 		NULL, 	PRECEDENCE_NONE 		},
	[TOKEN_IF] 					   	= { NULL, 		NULL, 	PRECEDENCE_NONE 		},
	[TOKEN_ELSE]					= { NULL, 		NULL, 	PRECEDENCE_NONE 		},
	[TOKEN_TRUE] 				   	= { literal,	NULL, 	PRECEDENCE_NONE 		},
	[TOKEN_FALSE] 					= { literal,	NULL, 	PRECEDENCE_NONE 		},
	[TOKEN_NIL]						= { literal,	NULL, 	PRECEDENCE_NONE 		},
	[TOKEN_ERROR] 				   	= { NULL, 		NULL, 	PRECEDENCE_NONE 		},
	[TOKEN_EOF] 					= { NULL, 		NULL, 	PRECEDENCE_NONE 		}
};

static ParseRule* getRule(TokenType type) {
	return &rules[type];
}
