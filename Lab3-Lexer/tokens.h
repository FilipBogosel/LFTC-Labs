/*
 * tokens.h — Token code definitions for the mini-language lexer.
 *
 * Convention:
 *   - Single-character tokens (operators & separators) use their ASCII
 *     value as the token code (e.g. '+' = 43, ';' = 59).
 *   - Multi-character and semantic tokens start at 256 to avoid any
 *     collision with the ASCII range.
 */

#ifndef TOKENS_H
#define TOKENS_H

/* --- Identifiers and Constants (stored in Symbol Table) --- */
#define TOK_ID        256
#define TOK_CONST     257

/* --- Reserved Words (from bnf.txt §II.a) --- */
#define TOK_ELSE      258
#define TOK_IF        259
#define TOK_INT       260
#define TOK_PROGRAM   261
#define TOK_READ      262
#define TOK_REAL      263
#define TOK_WHILE     264
#define TOK_WRITE     265

/* --- Multi-character Operators (from bnf.txt §II.a) --- */
#define TOK_EQ        266    /* == */
#define TOK_NEQ       267    /* != */
#define TOK_LE        268    /* <= */
#define TOK_GE        269    /* >= */

/*
 * Single-character operators and separators are NOT defined here.
 * They use their raw ASCII value as the token code:
 *
 *   Operators:   +  -  *  /  =  <  >
 *   Separators:  (  )  [  ]  {  }  ;  ,
 */

/*
 * Returns a human-readable string for any token code.
 * For codes >= 256: the keyword / operator name.
 * For codes < 256:  the ASCII character itself.
 */
const char *token_name(int code);

#endif /* TOKENS_H */
