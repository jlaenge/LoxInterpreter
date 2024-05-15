#include <vm.h>

#include <compiler.h>
#include <debug.h>
#include <memory.h>
#include <object.h>

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

VM vm;

static void resetStack() {
	vm.stackTop = vm.stack;
}
static void runtimeError(const char* format, ...) {
	va_list args;
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	fputs("\n", stderr);

	size_t instruction = (void*)vm.ip - (void*)vm.chunk->code.memory - 1;
	int line = vm.chunk->lines[instruction];
	fprintf(stderr, "[line %d] in script\n", line);

	resetStack();
}

void initVM() {
	resetStack();
	vm.objects = NULL;
	initTable(&vm.strings);
}
void freeVM() {
	freeObjects();
	freeTable(&vm.strings);
}

static void printStack() {
	printf("STACK:        ");
	for(Value* slot = vm.stack; slot < vm.stackTop; slot++) {
		printf("[ ");
		printValue(*slot);
		printf(" ]");
	}
	printf("\n");
}
static InterpretResult run() {

#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(valueType, operator) \
	do { \
		if(!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) { \
			runtimeError("Operands must be numbers."); \
			return INTERPRET_RUNTIME_ERROR; \
		} \
		double right = AS_NUMBER(pop()); \
		double left = AS_NUMBER(pop()); \
		double result = left operator right; \
		push(valueType(result)); \
	} while(false)

	while(true) {

#ifdef DEBUG_TRACE_EXECUTION
		printStack();
		disassembleInstruction(vm.chunk, (int)((void*)vm.ip - (void*)vm.chunk->code.memory));
#endif

		uint8_t instruction = READ_BYTE();
		switch(instruction) {

			// values
			case OP_CONSTANT:	push(READ_CONSTANT());		break;
			case OP_NIL:		push(NIL_VALUE);			break;
			case OP_TRUE:		push(BOOLEAN_VALUE(true));	break;
			case OP_FALSE:		push(BOOLEAN_VALUE(false));	break;

			// comparision
			case OP_EQUAL:
				Value right = pop();
				Value left = pop();
				push(BOOLEAN_VALUE(valuesEqual(left, right)));
				break;
			case OP_LESS:		BINARY_OP(BOOLEAN_VALUE, <);	break;
			case OP_GREATER:	BINARY_OP(BOOLEAN_VALUE, >);	break;

			// arithmetic
			case OP_NEGATE:
				if(!IS_NUMBER(peek(0))) {
					runtimeError("Operand must be a number.");
					return INTERPRET_RUNTIME_ERROR;
				}
				push(NUMBER_VALUE(-AS_NUMBER(pop())));
				break;
			case OP_ADD: {
				Value right = peek(0);
				Value left = peek(1);
				if(IS_STRING(left) && IS_STRING(right)) {
					concatenate();
				} else if(IS_NUMBER(left) && IS_NUMBER(right)) {
					BINARY_OP(NUMBER_VALUE, +);
				} else {
					runtimeError("Operands must both be strings or both be numbers");
					return INTERPRET_RUNTIME_ERROR;
				}

			}
			break;
			case OP_SUBTRACT:	BINARY_OP(NUMBER_VALUE, -);				break;
			case OP_MULTIPLY:	BINARY_OP(NUMBER_VALUE, *);				break;
			case OP_DIVIDE:		BINARY_OP(NUMBER_VALUE, /);				break;
			case OP_NOT:		push(BOOLEAN_VALUE(isFalsey(pop())));	break;

			// misc
			case OP_RETURN:
				printValue(pop());
				printf("\n");
				return INTERPRET_OK;

			// error
			default:
				assert(false);

		}

	}

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP

}

InterpretResult interpret(const char* source) {
	assert(source != NULL);

	Chunk chunk;
	initChunk(&chunk);

	if(!compile(source, &chunk)) {
		freeChunk(&chunk);
		return INTERPRET_COMPILE_ERROR;
	}

	vm.chunk = &chunk;
	vm.ip = (uint8_t*)chunk.code.memory;

	InterpretResult result = run();
	freeChunk(&chunk);

	return result;
}
void push(Value value) {
	assert((int)(vm.stackTop - vm.stack) < STACK_MAX);
	*vm.stackTop = value;
	vm.stackTop++;
}
Value pop() {
	assert((int)(vm.stackTop - vm.stack) > 0);
	vm.stackTop--;
	return *vm.stackTop;
}
Value peek(int distance) {
	return vm.stackTop[-distance - 1];
}
bool isFalsey(Value value) {
	return IS_NIL(value) || (IS_BOOLEAN(value) && !AS_BOOLEAN(value));
}
void concatenate() {
	ObjectString* right = AS_STRING(pop());
	ObjectString* left = AS_STRING(pop());
	int length = left->length + right->length;
	char* characters = ALLOCATE(char, length + 1);
	memcpy(characters, left->characters, left->length);
	memcpy(characters + left->length, right->characters, right->length);
	characters[length] = '\0';
	ObjectString* objectString = takeString(characters, length);
	push(OBJECT_VALUE(objectString));
}
