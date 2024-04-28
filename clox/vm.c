#include "vm.h"

#include <stdio.h>

VM vm;

static void resetStack() {
	vm.stackTop = vm.stack;
}
void initVM() {
	resetStack();
}
void freeVM() {}

static void printStack() {
	printf("        ");
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

	while(true) {

		uint8_t instruction = READ_BYTE();
#ifdef DEBUG_TRACE_EXECUTION
		printStack();
		disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif

		switch(instruction) {
			case OP_CONSTANT:
				Value constant = READ_CONSTANT();
				push(constant);
				printf("\n");
				break;
			case OP_RETURN:
				printValue(pop());
				printf("\n");
				return INTERPRET_OK;
		}

	}

#undef READ_BYTE

}

InterpretResult interpret(Chunk* chunk) {
	assert(chunk != NULL);
	vm.chunk = chunk;
	vm.ip = vm.chunk->code;
	return run();
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
