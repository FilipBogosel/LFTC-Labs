#include "symbol_table.h"
#include <stdlib.h>
#include <string.h>

static unsigned int hash(const char *key) {
    unsigned int hash_value = 5381;
    while (*key) {
        hash_value = (hash_value * 31) + (unsigned char)(*key);
        key++;
    }
    return hash_value % HASH_TABLE_SIZE;
}

void symbol_table_init(SymbolTable *symbol_table) {
    symbol_table = malloc(sizeof(SymbolTable));
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        symbol_table->buckets[i] = NULL;
    }
    symbol_table->next_index = 0;
}

int symbol_table_insert(SymbolTable *symbol_table, const char *key) {
    const unsigned int hash_value = hash(key);
    for (const HTNode *n = symbol_table->buckets[hash_value]; n; n = n->next) {
        if (strcmp(n->key, key) == 0)
            return n->index;
    }

    HTNode *node = (HTNode *)malloc(sizeof(HTNode));
    if (!node) {
        fprintf(stderr, "Error: malloc failed in ST insert\n");
        exit(EXIT_FAILURE);
    }
    node->key = (char *)malloc(strlen(key) + 1);
    if (!node->key) {
        fprintf(stderr, "Error: malloc failed for key copy\n");
        free(node);
        exit(EXIT_FAILURE);
    }
    strcpy(node->key, key);
    node->index = symbol_table->next_index++;
    node->next = symbol_table->buckets[hash_value];
    symbol_table->buckets[hash_value] = node;

    return node->index;
}

int symbol_table_search(const SymbolTable *symbol_table, const char *key) {
    const unsigned int hash_value = hash(key);
    for (const HTNode *n = symbol_table->buckets[hash_value]; n; n = n->next) {
        if (strcmp(n->key, key) == 0)
            return n->index;
    }
    return -1;
}

const char * symbol_table_get_key(const SymbolTable *symbol_table, const int index) {
    for (int bucket = 0; bucket < HASH_TABLE_SIZE; bucket++) {
        for (const HTNode *n = symbol_table->buckets[bucket]; n; n = n->next) {
            if (n->index == index)
                return n->key;
        }
    }
    return NULL;
}

void symbol_table_print(const SymbolTable *symbol_table, FILE *out) {
    const int count = symbol_table->next_index;
    if (count == 0) {
        fprintf(out, "  (empty)\n");
        return;
    }

    const char **keys = malloc(count * sizeof(const char *));
    if (!keys) {
        fprintf(stderr, "Error: malloc failed in ST print\n");
        return;
    }
    for (int i = 0; i < count; i++)
        keys[i] = NULL;

    for (int bucket = 0; bucket < HASH_TABLE_SIZE; bucket++) {
        for (const HTNode *n = symbol_table->buckets[bucket]; n; n = n->next)
            keys[n->index] = n->key;
    }

    for (int i = 0; i < count; i++) {
        if (keys[i])
            fprintf(out, "  %-6d | %s\n", i, keys[i]);
    }

    free(keys);
}

void symbol_table_free(SymbolTable *symbol_table) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        HTNode *n = symbol_table->buckets[i];
        while (n) {
            HTNode *next = n->next;
            free(n->key);
            free(n);
            n = next;
        }
        symbol_table->buckets[i] = NULL;
    }
    symbol_table->next_index = 0;
}
