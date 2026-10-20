/*
 * pif.c — PIF implementation.
 */

#include <stdlib.h>
#include "pif.h"
#include "tokens.h"

void pif_init(PIF *pif)
{
    pif->entries = (PIFEntry *)malloc(PIF_INITIAL_CAPACITY * sizeof(PIFEntry));
    if (!pif->entries) {
        fprintf(stderr, "Error: malloc failed in PIF init\n");
        exit(EXIT_FAILURE);
    }
    pif->size     = 0;
    pif->capacity = PIF_INITIAL_CAPACITY;
}

void pif_add(PIF *pif, int token_code, int st_position)
{
    if (pif->size >= pif->capacity) {
        pif->capacity *= 2;
        pif->entries = (PIFEntry *)realloc(pif->entries,
                                           pif->capacity * sizeof(PIFEntry));
        if (!pif->entries) {
            fprintf(stderr, "Error: realloc failed in PIF add\n");
            exit(EXIT_FAILURE);
        }
    }
    pif->entries[pif->size].token_code  = token_code;
    pif->entries[pif->size].st_position = st_position;
    pif->size++;
}

void pif_print(const PIF *pif, const SymbolTable *st, FILE *out)
{
    fprintf(out, "  %-6s | %-15s | %s\n", "Code", "Token", "ST Pos");
    fprintf(out, "  -------+-----------------+----------\n");

    for (int i = 0; i < pif->size; i++) {
        int         code = pif->entries[i].token_code;
        int         pos  = pif->entries[i].st_position;
        const char *name = token_name(code);

        if (code == TOK_ID || code == TOK_CONST) {
            const char *lexeme = st_get_key(st, pos);
            fprintf(out, "  %-6d | %-15s | %-4d  (%s)\n",
                    code, name, pos, lexeme ? lexeme : "?");
        } else {
            fprintf(out, "  %-6d | %-15s | %d\n", code, name, pos);
        }
    }
}

void pif_free(PIF *pif)
{
    free(pif->entries);
    pif->entries  = NULL;
    pif->size     = 0;
    pif->capacity = 0;
}
