#include <table.h>

#include <memory.h>
#include <value.h>
#include <object.h>

#define TABLE_MAX_LOAD 0.75

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

static Entry* findEntry(Entry* entries, int capacity, ObjectString* key) {
	assert(entries != NULL);
	assert(key != NULL);

	uint32_t index = key->hash % capacity;
	Entry* tombstone = NULL;
	while(true) {
		Entry* entry = &entries[index];

		if(entry->key == NULL) {
			if(IS_NIL(entry->value)) {
				return tombstone != NULL ? tombstone : entry;
			} else {
				if(tombstone == NULL) tombstone = entry;
			}
		} else if(entry->key == key) {
			return entry;
		}

		index = (index + 1) % capacity;
	}
}
static void adjustCapacity(Table* table, int capacity) {

	Entry* entries = ALLOCATE(Entry, capacity);

	// initialize empty entries
	for(int i=0; i<capacity; i++) {
		entries[i].key = NULL;
		entries[i].value = NIL_VALUE;
	}

	// copy existing entries
	table->count = 0;
	for(int i=0; i<table->capacity; i++) {
		Entry* entry = &table->entries[i];
		if(entry->key == NULL) continue;

		Entry* destination = findEntry(entries, capacity, entry->key);
		destination->key = entry->key;
		destination->value = entry->value;
		table->count++;
	}

	// delete old entries
	FREE_ARRAY(Entry, table->entries, table->capacity);

	// set new entries and capacity for table
	table->entries = entries;
	table->capacity = capacity;

}

bool tableGet(Table* table, ObjectString* key, Value* value) {
	assert(table != NULL);
	assert(key != NULL);
	assert(value != NULL);

	if(table->capacity == 0) return false;

	Entry* entry = findEntry(table->entries, table->capacity, key);
	if(entry->key == NULL) return false;

	*value = entry->value;
	return true;
}
bool tableSet(Table* table, ObjectString* key, Value value) {
	assert(table != NULL);
	assert(key != NULL);
	// assert(value != NULL); // the value may be NULL

	if(table->count + 1  > table->capacity * TABLE_MAX_LOAD) {
		table->capacity = GROW_CAPACITY(table->capacity);
		adjustCapacity(table, table->capacity);
	}

	Entry* entry = findEntry(table->entries, table->capacity, key);
	bool isNewKey = (entry->key == NULL);
	if(isNewKey && IS_NIL(entry->value)) table->count++;

	entry->key = key;
	entry->value = value;

	return isNewKey;
}
bool tableDelete(Table* table, ObjectString* key) {
	assert(table != NULL);
	assert(key != NULL);

	if(table->count == 0) return false;

	Entry* entry = findEntry(table->entries, table->capacity, key);
	if(entry->key != key) return false;

	entry->key = NULL;
	entry->value = BOOLEAN_VALUE(true);	// any value other than NIL will do

	return true;
}
void tableAddAll(Table* from, Table* to) {

	assert(from != NULL);
	assert(to != NULL);

	for(int i=0; i<from->capacity; i++) {
		Entry* entry = &from->entries[i];
		if(entry->key != NULL) {
			tableSet(to, entry->key, entry->value);
		}
	}

}
