/*
 * symbol_table.h — Hash Table with separate chaining.
 *
 * A single unified table stores both identifiers and constants.
 * Each distinct string inserted receives a unique, strictly
 * incremental integer index starting at 0.
 */

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>

#define HT_SIZE 211   /* prime bucket count for good distribution */

/* A node in one of the bucket chains */
typedef struct HTNode {
    char           *key;    /* The lexeme (identifier name or constant literal) */
    int             index;  /* Unique incremental index assigned at insertion   */
    struct HTNode  *next;   /* Next node in the chain (or NULL)                 */
} HTNode;

/* The Symbol Table container */
typedef struct SymbolTable {
    HTNode *buckets[HT_SIZE];
    int     next_index;     /* Next index to assign */
} SymbolTable;

/* Initialize an empty symbol table (all buckets NULL, index counter 0) */
void st_init(SymbolTable *st);

/*
 * Insert `key` into the symbol table.
 * If the key already exists, returns its existing index.
 * Otherwise assigns next_index, stores the key, and returns the new index.
 */
int st_insert(SymbolTable *st, const char *key);

/*
 * Search for `key`.  Returns its index if found, -1 otherwise.
 */
int st_search(const SymbolTable *st, const char *key);

/*
 * Reverse lookup: return the key stored at `index`, or NULL.
 */
const char *st_get_key(const SymbolTable *st, int index);

/*
 * Print every entry ordered by index (0, 1, 2, …).
 */
void st_print(const SymbolTable *st, FILE *out);

/* Free all allocated memory */
void st_free(SymbolTable *st);

#endif /* SYMBOL_TABLE_H */
