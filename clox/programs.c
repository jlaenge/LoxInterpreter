#include <programs.h>

#include <value.h>

void create_program1(Chunk* chunk) {

	int constant1 = addConstant(chunk, NUMBER_VALUE(1.2));
	writeChunk(chunk, OP_CONSTANT, 123);
	writeChunk(chunk, constant1, 123);

	int constant2 = addConstant(chunk, NUMBER_VALUE(3.4));
	writeChunk(chunk, OP_CONSTANT, 123);
	writeChunk(chunk, constant2, 123);

	writeChunk(chunk, OP_ADD, 123);

	int constant3 = addConstant(chunk, NUMBER_VALUE(5.6));
	writeChunk(chunk, OP_CONSTANT, 123);
	writeChunk(chunk, constant3, 123);

	writeChunk(chunk, OP_DIVIDE, 123);
	writeChunk(chunk, OP_NEGATE, 123);
	writeChunk(chunk, OP_RETURN, 123);

}
