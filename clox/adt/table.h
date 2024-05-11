#ifndef CLOX_ADT_TABLE_H_
#define CLOX_ADT_TABLE_H_

#include <common.h>
#include <value.h>

typedef struct Entry_tag Entry;
struct Entry_tag {
	ObjectString* key;
	Value* value;
};

typedef struct Table_tag Table;
struct Table_tag {
	int count;
	int capacity;
	Entry* entries;
};

void initTable(Table* table);
void freeTable(Table* table);

#endif
