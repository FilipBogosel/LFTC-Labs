#include <stdio.h>
#include "pif.h"

#include <stdlib.h>

#include "tokens.h"

void program_internal_form_init(PIF *pif) {
    pif->size = 0;
    pif->capacity = 100;
    pif->entries = malloc(pif->capacity * sizeof(PIFEntry));
}

void program_internal_form_add(PIF *pif, int token_code, int symbol_table_position) {
    if (pif->size == pif->capacity) {
        program_internal_form_resize(pif);
    }
    pif->entries[pif->size].token_code = token_code;
    pif->entries[pif->size].symbol_table_position = symbol_table_position;
    pif->size++;
}

void program_internal_form_print(const PIF *pif, const SymbolTable *symbol_table, FILE *out) {
    fprintf(out, "  %-6s | %-15s | %s\n", "Code", "Token", "ST Pos");
    fprintf(out, "  -------+-----------------+----------\n");

    for (int i = 0; i < pif->size; i++) {
        const int code = pif->entries[i].token_code;
        const int pos  = pif->entries[i].symbol_table_position;
        const char *name = token_name(code);

        if (code == TOKEN_IDENTIFIER || code == TOKEN_CONSTANT) {
            const char *lexeme = symbol_table_get_key(symbol_table, pos);
            fprintf(out, "  %-6d | %-15s | %-4d  (%s)\n",
                    code, name, pos, lexeme ? lexeme : "?");
        } else {
            fprintf(out, "  %-6d | %-15s | %d\n", code, name, pos);
        }
    }
}

void program_internal_form_free(const PIF *pif) {
    free(pif->entries);
}

void program_internal_form_resize(PIF *pif) {
    pif->capacity *= 2;
    pif->entries = realloc(pif->entries, pif->capacity * sizeof(PIFEntry));
    if (!pif->entries) {
        fprintf(stderr, "Error: realloc failed in PIF add\n");
        exit(EXIT_FAILURE);
    }
}
