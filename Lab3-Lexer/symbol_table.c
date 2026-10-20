/*
 * symbol_table.c — Hash Table with separate chaining (implementation).
 *
 * Hash function: djb2 (Dan Bernstein) — simple, fast, good distribution
 * for short identifier / constant strings.
 */

#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

/* ------------------------------------------------------------------ */
/*  Hash function                                                      */
/* ------------------------------------------------------------------ */

static unsigned int hash(const char *key)
{
    unsigned int h = 5381;
    while (*key) {
        h = ((h << 5) + h) + (unsigned char)*key;   /* h * 33 + c */
        key++;
    }
    return h % HT_SIZE;
}

/* ------------------------------------------------------------------ */
/*  Public API                                                         */
/* ------------------------------------------------------------------ */

void st_init(SymbolTable *st)
{
    for (int i = 0; i < HT_SIZE; i++)
        st->buckets[i] = NULL;
    st->next_index = 0;
}

int st_insert(SymbolTable *st, const char *key)
{
    unsigned int h = hash(key);

    /* Walk the chain — return early if the key already exists */
    for (HTNode *n = st->buckets[h]; n; n = n->next) {
        if (strcmp(n->key, key) == 0)
            return n->index;
    }

    /* Key is new: allocate a node and prepend to the chain */
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
    node->index = st->next_index++;
    node->next  = st->buckets[h];
    st->buckets[h] = node;

    return node->index;
}

int st_search(const SymbolTable *st, const char *key)
{
    unsigned int h = hash(key);
    for (HTNode *n = st->buckets[h]; n; n = n->next) {
        if (strcmp(n->key, key) == 0)
            return n->index;
    }
    return -1;
}

const char *st_get_key(const SymbolTable *st, int index)
{
    for (int b = 0; b < HT_SIZE; b++) {
        for (HTNode *n = st->buckets[b]; n; n = n->next) {
            if (n->index == index)
                return n->key;
        }
    }
    return NULL;
}

void st_print(const SymbolTable *st, FILE *out)
{
    int count = st->next_index;
    if (count == 0) {
        fprintf(out, "  (empty)\n");
        return;
    }

    /*
     * Collect pointers into a temporary array so we can print in
     * index order (0, 1, 2, …) rather than bucket order.
     */
    const char **keys = (const char **)malloc(count * sizeof(const char *));
    if (!keys) {
        fprintf(stderr, "Error: malloc failed in ST print\n");
        return;
    }
    for (int i = 0; i < count; i++)
        keys[i] = NULL;

    for (int b = 0; b < HT_SIZE; b++) {
        for (HTNode *n = st->buckets[b]; n; n = n->next)
            keys[n->index] = n->key;
    }

    for (int i = 0; i < count; i++) {
        if (keys[i])
            fprintf(out, "  %-6d | %s\n", i, keys[i]);
    }

    free(keys);
}

void st_free(SymbolTable *st)
{
    for (int i = 0; i < HT_SIZE; i++) {
        HTNode *n = st->buckets[i];
        while (n) {
            HTNode *next = n->next;
            free(n->key);
            free(n);
            n = next;
        }
        st->buckets[i] = NULL;
    }
    st->next_index = 0;
}
