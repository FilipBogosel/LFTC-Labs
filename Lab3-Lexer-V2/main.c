#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"
#include "symbol_table.h"
#include "pif.h"

/* Extern declarations to link with the globals defined in lexer.l */
extern FILE *yyin;
extern int yylex(void);
extern int has_errors;
extern SymbolTable st;
extern PIF pif;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <source_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        fprintf(stderr, "Error: cannot open file '%s'\n", argv[1]);
        return EXIT_FAILURE;
    }

    symbol_table_init(&st);
    program_internal_form_init(&pif);

    yyin = file;
    yylex();
    fclose(file);

    printf("\n========================================\n");
    printf("  Program Internal Form (PIF)\n");
    printf("========================================\n");
    program_internal_form_print(&pif, &st, stdout);

    printf("\n========================================\n");
    printf("  Symbol Table (Hash Table)\n");
    printf("========================================\n");
    symbol_table_print(&st, stdout);
    printf("\n");

    if (has_errors) {
        printf(">> Lexical analysis completed with ERRORS.\n\n");
    } else {
        printf(">> Lexical analysis completed successfully.\n\n");
    }

    symbol_table_free(&st);
    program_internal_form_free(&pif);

    return has_errors ? EXIT_FAILURE : EXIT_SUCCESS;
}