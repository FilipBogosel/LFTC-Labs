# Lexical Analyzer Documentation

## 1. Use of Tools
This project utilizes **Flex (Lex)** as the primary tool for lexical analysis and **CMake** for build automation.
*   **Flex:** The `lexer.l` file contains the lexical specifications. Flex processes this file to automatically generate a C scanner (`lex.yy.c`). This generated scanner contains a deterministic finite automaton (DFA) that reads the input character stream and matches it against the defined regular expressions.
*   **CMake:** Used to manage the build process, link the generated `lex.yy.c` file with the manual C implementations (`main.c`, `symbol_table.c`, `pif.c`), and produce the final executable.

## 2. Lexical Tokens Detection (Identifiers & Constants)
Token detection is driven by regular expressions defined in the Flex configuration.

*   **Identifiers:** Detected using the rule `{LETTER}({LETTER}|{DIGIT}|_)*`. 
    *   *Validation:* When an identifier is matched, the scanner executes C code to check its length. If it exceeds 256 characters, a lexical error is flagged. Otherwise, the identifier is inserted into the Symbol Table.
*   **Constants:**
    *   *Integers:* Detected using the rule `0|{NON_ZERO}{DIGIT}*`. This ensures numbers do not have leading zeros unless the number is exactly `0`.
    *   *Reals (Floating Point):* Detected using the rule `(0|{NON_ZERO}{DIGIT}*)\.{DIGIT}+`. This requires a valid integer portion followed by a decimal point and at least one fractional digit.
    *   *Handling:* Both integer and real constants are inserted into the Symbol Table as string values (`yytext`), and their unique Symbol Table index is passed to the Program Internal Form (PIF) alongside the `TOKEN_CONSTANT` code.

## 3. Data Structures and Operations for Symbol Table (ST) Management
The Symbol Table is implemented as a **Hash Table** with separate chaining to handle collisions. It uses an array of 211 buckets (a prime number to improve distribution), where each bucket points to a linked list of `HTNode` structures.

**Core Operations:**
*   **Initialization (`symbol_table_init`):** Allocates memory for the hash table structure, sets all bucket pointers to `NULL`, and initializes the global `next_index` counter to 0.
*   **Hashing (`hash`):** Uses a variant of the djb2 algorithm. It starts with a base value of 5381 and iteratively multiplies by 31 while adding the ASCII value of each character in the key, returning modulo 211.
*   **Insertion (`symbol_table_insert`):** 
    1. Computes the hash for the incoming string key.
    2. Traverses the linked list at the resulting bucket to check if the key already exists. If it does, it returns the existing index.
    3. If the key is new, it allocates memory for a new `HTNode` and copies the string into it.
    4. The node receives the current `next_index` (which is then incremented) and is inserted at the head of the linked list for that bucket.
*   **Searching (`symbol_table_search` / `symbol_table_get_key`):** Allows reverse lookups. The search function traverses a specific bucket's linked list to find a string and return its integer index. The get-key function iterates through all buckets to retrieve a string given its index (used primarily for printing the PIF).
*   **Memory Management (`symbol_table_free`):** Iterates through all 211 buckets and safely frees the dynamically allocated memory for both the copied string keys and the `HTNode` structures, ensuring no memory leaks occur at termination.