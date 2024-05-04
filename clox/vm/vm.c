#include <vm.h>

#include <compiler.h>
#include <debug.h>

#include <stdarg.h>
#include <stdio.h>

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

	size_t instruction = vm.ip - vm.chunk->code - 1;
	int line = vm.chunk->lines[instruction];
	fprintf(stderr, "[line %d] in script\n", line);

	resetStack();
}

void initVM() {
	resetStack();
}
void freeVM() {}

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
	do {\
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
		disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif

		uint8_t instruction = READ_BYTE();
		switch(instruction) {
			case OP_CONSTANT:
				Value constant = READ_CONSTANT();
				push(constant);
				break;
			case OP_NEGATE:
				if(IS_NUMBER(peek(0))) {
					runtimeError("Operand must be a number.");
					return INTERPRET_RUNTIME_ERROR;
				}
				push(NUMBER_VALUE(-AS_NUMBER(pop())));
				break;
			case OP_ADD:
				BINARY_OP(NUMBER_VALUE, +);
				break;
			case OP_SUBTRACT:
				BINARY_OP(NUMBER_VALUE, -);
				break;
			case OP_MULTIPLY:
				BINARY_OP(NUMBER_VALUE, *);
				break;
			case OP_DIVIDE:
				BINARY_OP(NUMBER_VALUE, /);
				break;
			case OP_RETURN:
				printValue(pop());
				printf("\n");
				return INTERPRET_OK;
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
	vm.ip = chunk.code;

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
