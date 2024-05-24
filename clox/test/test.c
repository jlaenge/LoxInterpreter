#include <test.h>

#include <vm.h>

#define OUTPUT_FILE "test_output.tmp"
#define ERROR_FILE  "test_error.tmp"

void test_all() {

	// open output and error files
	FILE* outputFile = fopen(OUTPUT_FILE, "w+b");
	assert(outputFile != NULL);
	FILE* errorFile = fopen(ERROR_FILE, "w+b");
	assert(errorFile != NULL);

	initVM(outputFile, errorFile);
	assert(false);

}
