#include "vm.h"

#include <stdio.h>

VM vm;

void initVM() {}
void freeVM() {}

static InterpretResult run() {

#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])

	while(true) {

		uint8_t instruction = READ_BYTE();
		switch(instruction) {
			case OP_CONSTANT:
				Value constant = READ_CONSTANT();
				printValue(constant);
				printf("\n");
				break;
			case OP_RETURN:
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
