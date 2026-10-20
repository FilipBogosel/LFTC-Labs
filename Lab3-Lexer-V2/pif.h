#pragma once
#include "symbol_table.h"

typedef struct PIFEntry {
    int token_code;
    int symbol_table_position;
} PIFEntry;

typedef struct PIF {
    PIFEntry* entries;
    int size;
    int capacity;
} PIF;

void program_internal_form_init(PIF *pif);
void program_internal_form_add(PIF *pif, int token_code, int symbol_table_position);
void program_internal_form_print(const PIF *pif, const SymbolTable *symbol_table, FILE *out);
void program_internal_form_free(const PIF *pif);
void program_internal_form_resize(PIF *pif);