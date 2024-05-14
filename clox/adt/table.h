#ifndef CLOX_ADT_TABLE_H_
#define CLOX_ADT_TABLE_H_

#include <common.h>
#include <value.h>

typedef struct Entry_tag Entry;
struct Entry_tag {
	ObjectString* key;
	Value value;
};

typedef struct Table_tag Table;
struct Table_tag {
	int count;
	int capacity;
	Entry* entries;
};

void initTable(Table* table);
void freeTable(Table* table);

bool tableGet(Table* table, ObjectString* key, Value* value);
bool tableSet(Table* table, ObjectString* key, Value value);
bool tableDelete(Table* table, ObjectString* key);
void tableAddAll(Table* from, Table* to);
ObjectString* tableFindString(Table* table, const char* characters, int length, uint32_t hash);

#endif
