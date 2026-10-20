%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokens.h"
#include "symbol_table.h"
#include "pif.h"

int line_number = 1;
int has_errors  = 0;
SymbolTable st;
PIF pif;
%}

%option noyywrap nounput noinput

LETTER      [a-zA-Z]
DIGIT       [0-9]
NON_ZERO    [1-9]
IDENTIFIER  {LETTER}({LETTER}|{DIGIT}|_)*
INTEGER     0|{NON_ZERO}{DIGIT}*
REAL        (0|{NON_ZERO}{DIGIT}*)\.{DIGIT}+
WHITESPACE  [ \t\r]+

%%


\n              { line_number++; }
{WHITESPACE}    { /* skip */ }

"else"          { program_internal_form_add(&pif, TOKEN_ELSE, -1); }
"if"            { program_internal_form_add(&pif, TOKEN_IF, -1); }
"int"           { program_internal_form_add(&pif, TOKEN_INT, -1); }
"program"       { program_internal_form_add(&pif, TOKEN_PROGRAM, -1); }
"read"          { program_internal_form_add(&pif, TOKEN_READ, -1); }
"real"          { program_internal_form_add(&pif, TOKEN_REAL, -1); }
"while"         { program_internal_form_add(&pif, TOKEN_WHILE, -1); }
"write"         { program_internal_form_add(&pif, TOKEN_WRITE, -1); }

"=="            { program_internal_form_add(&pif, TOKEN_EQUAL, -1); }
"!="            { program_internal_form_add(&pif, TOKEN_NOT_EQUAL, -1); }
"<="            { program_internal_form_add(&pif, TOKEN_LESS_EQUAL, -1); }
">="            { program_internal_form_add(&pif, TOKEN_GREATER_EQUAL, -1); }

"+"|"-"|"*"|"/"|"="|"<"|">"|"("|")"|"["|"]"|"{"|"}"|";"|"," {
                    program_internal_form_add(&pif, yytext[0], -1); 
                }

{REAL}          { program_internal_form_add(&pif, TOKEN_CONSTANT, symbol_table_insert(&st, yytext)); }
{INTEGER}       { program_internal_form_add(&pif, TOKEN_CONSTANT, symbol_table_insert(&st, yytext)); }

{IDENTIFIER}    {
                    if (strlen(yytext) > 256) {
                        fprintf(stderr, "Lexical error at line %d: identifier '%.30s...' exceeds 256 characters\n", line_number, yytext);
                        has_errors = 1;
                    } else {
                        program_internal_form_add(&pif, TOKEN_IDENTIFIER, symbol_table_insert(&st, yytext));
                    }
                }

.               {
                    fprintf(stderr, "Lexical error at line %d: invalid token '%s'\n", line_number, yytext);
                    has_errors = 1;
                }

%%