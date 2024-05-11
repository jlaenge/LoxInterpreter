#include <table.h>

#include <memory.h>

void initTable(Table* table) {
	assert(table != NULL);
	table->count = 0;
	table->capacity = 0;
	table->entries = NULL;
}
void freeTable(Table* table) {
	assert(table != NULL);
	FREE_ARRAY(Value, table->entries, table->capacity);
	initTable(table);
}



