#include <vm.h>

#include <compiler.h>
#include <debug.h>

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
#define BINARY_OP(operator) \
	do {\
		double right = pop();\
		double left = pop();\
		double result = left operator right;\
		push(result);\
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
				push(-pop());
				break;
			case OP_ADD:
				BINARY_OP(+);
				break;
			case OP_SUBTRACT:
				BINARY_OP(-);
				break;
			case OP_MULTIPLY:
				BINARY_OP(*);
				break;
			case OP_DIVIDE:
				BINARY_OP(/);
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
	compile(source);
	return INTERPRET_OK;
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
