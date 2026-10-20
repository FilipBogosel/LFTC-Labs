/*
 * pif.h — Program Internal Form (PIF).
 *
 * The PIF is a dynamic array of (token_code, st_position) pairs
 * representing the linearised token stream of the source program.
 *
 * For reserved words, operators, and separators  →  st_position = -1
 * For identifiers (TOK_ID)  and constants (TOK_CONST)  →  st_position = index in ST
 */

#ifndef PIF_H
#define PIF_H

#include <stdio.h>
#include "symbol_table.h"

#define PIF_INITIAL_CAPACITY 256

/* A single PIF entry */
typedef struct {
    int token_code;     /* Token code (ASCII or >= 256, see tokens.h) */
    int st_position;    /* ST index, or -1 if not stored in ST        */
} PIFEntry;

/* Dynamic-array container */
typedef struct {
    PIFEntry *entries;
    int       size;
    int       capacity;
} PIF;

/* Initialize an empty PIF */
void pif_init(PIF *pif);

/* Append a (token_code, st_position) entry */
void pif_add(PIF *pif, int token_code, int st_position);

/*
 * Print every PIF entry with its readable token name (via token_name())
 * and, for TOK_ID / TOK_CONST entries, the actual lexeme looked up
 * from the Symbol Table.
 */
void pif_print(const PIF *pif, const SymbolTable *st, FILE *out);

/* Free all memory */
void pif_free(PIF *pif);

#endif /* PIF_H */
