/*
 * tokens.c — Token name lookup.
 *
 * Converts any integer token code back to a readable string.
 */

#include "tokens.h"

const char *token_name(int code)
{
    switch (code) {
        /* Identifiers / Constants */
        case TOK_ID:      return "id";
        case TOK_CONST:   return "const";

        /* Reserved words */
        case TOK_ELSE:    return "else";
        case TOK_IF:      return "if";
        case TOK_INT:     return "int";
        case TOK_PROGRAM: return "program";
        case TOK_READ:    return "read";
        case TOK_REAL:    return "real";
        case TOK_WHILE:   return "while";
        case TOK_WRITE:   return "write";

        /* Multi-character operators */
        case TOK_EQ:      return "==";
        case TOK_NEQ:     return "!=";
        case TOK_LE:      return "<=";
        case TOK_GE:      return ">=";

        default: {
            /*
             * Single-character tokens: the code IS the ASCII value,
             * so we return the character as a one-char string.
             */
            static char buf[2];
            buf[0] = (char)code;
            buf[1] = '\0';
            return buf;
        }
    }
}
