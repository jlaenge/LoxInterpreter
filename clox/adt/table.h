#ifndef CLOX_ADT_TABLE_H_
#define CLOX_ADT_TABLE_H_

#include <common.h>
#include <dynarray.h>
#include <value.h>

typedef struct Entry_tag Entry;
struct Entry_tag {
	ObjectString* key;
	Value value;
};

// TODO: probably we want to wrap the DynamicArray
// inside of a struct in case we want to add additional
// properties to our table. But for now we use the
// datastructure that is easiest.
typedef DynamicArray Table;

void initTable(Table* table);
void freeTable(Table* table);

bool tableGet(Table* table, ObjectString* key, Value* value);
bool tableSet(Table* table, ObjectString* key, Value value);
bool tableDelete(Table* table, ObjectString* key);
void tableAddAll(Table* from, Table* to);
ObjectString* tableFindString(Table* table, const char* characters, int length, uint32_t hash);

#endif
