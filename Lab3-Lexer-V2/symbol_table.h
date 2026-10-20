#pragma once
#define HASH_TABLE_SIZE 211
#include <stdio.h>

typedef struct HTNode {
    char           *key;
    int             index;
    struct HTNode  *next;
} HTNode;

typedef struct SymbolTable {
    HTNode *buckets[HASH_TABLE_SIZE];
    int next_index;
} SymbolTable;

void symbol_table_init(SymbolTable *symbol_table);

int symbol_table_insert(SymbolTable *symbol_table, const char *key);

int symbol_table_search(const SymbolTable *symbol_table,const char *key);

const char *symbol_table_get_key(const SymbolTable *symbol_table, int index);

void symbol_table_print(const SymbolTable *symbol_table, FILE *out);

void symbol_table_free(SymbolTable *symbol_table);



