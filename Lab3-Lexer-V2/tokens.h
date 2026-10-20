#pragma once
#define TOKEN_IDENTIFIER        256
#define TOKEN_CONSTANT     257

#define TOKEN_ELSE      258
#define TOKEN_IF        259
#define TOKEN_INT       260
#define TOKEN_PROGRAM   261
#define TOKEN_READ      262
#define TOKEN_REAL      263
#define TOKEN_WHILE     264
#define TOKEN_WRITE     265

#define TOKEN_EQUAL     266
#define TOKEN_NOT_EQUAL  267
#define TOKEN_LESS_EQUAL      268
#define TOKEN_GREATER_EQUAL   269

const char *token_name(int token_code);

