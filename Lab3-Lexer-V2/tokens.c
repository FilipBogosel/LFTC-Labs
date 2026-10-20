#include "tokens.h"

const char *token_name(const int token_code) {
    switch (token_code) {
        case TOKEN_IDENTIFIER: return "ID";
        case TOKEN_CONSTANT: return "CONST";
        case TOKEN_ELSE: return "ELSE";
        case TOKEN_IF: return "IF";
        case TOKEN_INT: return "INT";
        case TOKEN_PROGRAM: return "PROGRAM";
        case TOKEN_READ: return "READ";
        case TOKEN_REAL: return "REAL";
        case TOKEN_WHILE: return "WHILE";
        case TOKEN_WRITE: return "WRITE";
        case TOKEN_EQUAL: return "EQUAL";
        case TOKEN_NOT_EQUAL: return "NOT_EQUAL";
        case TOKEN_LESS_EQUAL: return "LESS_EQUAL";
        case TOKEN_GREATER_EQUAL: return "GREATER_EQUAL";
        default: {
            static char buffer[2];
            buffer[0] = (char)token_code;
            buffer[1] = '\0';
            return buffer;
        }
    }
}