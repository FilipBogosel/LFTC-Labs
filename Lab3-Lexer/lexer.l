/*
 * lexer.l — Flex specification for the mini-language lexer.
 *
 * Produces:
 *   1. Program Internal Form (PIF)
 *   2. Symbol Table (unified hash table for identifiers + constants)
 *   3. Lexical error messages on stderr
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokens.h"
#include "symbol_table.h"
#include "pif.h"

/* ---- Global state ------------------------------------------------ */

int  line_number = 1;
int  has_errors  = 0;

SymbolTable st;     /* Single unified Symbol Table */
PIF         pif;

%}

    /* ----- Flex options ----- */
%option noyywrap
%option nounput
%option noinput

    /* ----- Named definitions ----- */
LETTER      [a-zA-Z]
DIGIT       [0-9]
NON_ZERO    [1-9]

IDENTIFIER  {LETTER}({LETTER}|{DIGIT}|_)*
INTEGER     0|{NON_ZERO}{DIGIT}*
REAL        (0|{NON_ZERO}{DIGIT}*)\.{DIGIT}+

WHITESPACE  [ \t\r]+

%%
    /* ---- Whitespace & newlines ---- */
\n              { line_number++; }
{WHITESPACE}    { /* skip */ }

    /* ---- Reserved words (before IDENTIFIER — first-match rule) ---- */
"else"          { pif_add(&pif, TOK_ELSE,    -1); }
"if"            { pif_add(&pif, TOK_IF,      -1); }
"int"           { pif_add(&pif, TOK_INT,     -1); }
"program"       { pif_add(&pif, TOK_PROGRAM, -1); }
"read"          { pif_add(&pif, TOK_READ,    -1); }
"real"          { pif_add(&pif, TOK_REAL,    -1); }
"while"         { pif_add(&pif, TOK_WHILE,   -1); }
"write"         { pif_add(&pif, TOK_WRITE,   -1); }

    /* ---- Multi-char operators (before single-char — longest match) ---- */
"=="            { pif_add(&pif, TOK_EQ,  -1); }
"!="            { pif_add(&pif, TOK_NEQ, -1); }
"<="            { pif_add(&pif, TOK_LE,  -1); }
">="            { pif_add(&pif, TOK_GE,  -1); }

    /* ---- Single-char operators (ASCII value as token code) ---- */
"+"             { pif_add(&pif, '+', -1); }
"-"             { pif_add(&pif, '-', -1); }
"*"             { pif_add(&pif, '*', -1); }
"/"             { pif_add(&pif, '/', -1); }
"="             { pif_add(&pif, '=', -1); }
"<"             { pif_add(&pif, '<', -1); }
">"             { pif_add(&pif, '>', -1); }

    /* ---- Separators (ASCII value as token code) ---- */
"("             { pif_add(&pif, '(', -1); }
")"             { pif_add(&pif, ')', -1); }
"["             { pif_add(&pif, '[', -1); }
"]"             { pif_add(&pif, ']', -1); }
"{"             { pif_add(&pif, '{', -1); }
"}"             { pif_add(&pif, '}', -1); }
";"             { pif_add(&pif, ';', -1); }
","             { pif_add(&pif, ',', -1); }

    /* ---- Constants (real listed first for longest-match clarity) ---- */
{REAL}          { pif_add(&pif, TOK_CONST, st_insert(&st, yytext)); }
{INTEGER}       { pif_add(&pif, TOK_CONST, st_insert(&st, yytext)); }

    /* ---- Identifiers ---- */
{IDENTIFIER}    {
                    if (strlen(yytext) > 256) {
                        fprintf(stderr,
                            "Lexical error at line %d: identifier '%.30s...' "
                            "exceeds 256 characters\n", line_number, yytext);
                        has_errors = 1;
                    } else {
                        pif_add(&pif, TOK_ID, st_insert(&st, yytext));
                    }
                }

    /* ---- Catch-all: lexical error ---- */
.               {
                    fprintf(stderr,
                        "Lexical error at line %d: invalid token '%s'\n",
                        line_number, yytext);
                    has_errors = 1;
                }

%%

/* ================================================================== */
/*  main                                                               */
/* ================================================================== */

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <source_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        fprintf(stderr, "Error: cannot open '%s'\n", argv[1]);
        return EXIT_FAILURE;
    }

    st_init(&st);
    pif_init(&pif);

    yyin = file;
    yylex();
    fclose(file);

    /* ---- Output ---- */
    printf("\n");
    printf("========================================\n");
    printf("  Program Internal Form (PIF)\n");
    printf("========================================\n");
    pif_print(&pif, &st, stdout);

    printf("\n========================================\n");
    printf("  Symbol Table (Hash Table)\n");
    printf("========================================\n");
    printf("  %-6s | %s\n", "Index", "Key");
    printf("  -------+----------------\n");
    st_print(&st, stdout);
    printf("\n");

    if (has_errors)
        printf(">> Lexical analysis completed with ERRORS.\n\n");
    else
        printf(">> Lexical analysis completed successfully.\n\n");

    st_free(&st);
    pif_free(&pif);

    return has_errors ? EXIT_FAILURE : EXIT_SUCCESS;
}
