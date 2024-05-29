#include <test.h>

#include <dirwalk.h>
#include <sourcefile.h>
#include <vm.h>

#include <stdio.h>

#define OUTPUT_FILE "test_output.tmp"
#define ERROR_FILE  "test_error.tmp"

static void printName(struct dirent* entry) {

	printf("Name: %s\n", entry->d_name);

}

static void iterateDirectory() {

	dirwalk_walk("/home/jakob/workspace/cpp/LoxInterpreter/clox/test/testcases", printName);


}

void test_all() {

	// open output and error files
	FILE* outputFile = fopen(OUTPUT_FILE, "w+b");
	assert(outputFile != NULL);
	FILE* errorFile = fopen(ERROR_FILE, "w+b");
	assert(errorFile != NULL);

	initVM(outputFile, errorFile);
	iterateDirectory();

}
