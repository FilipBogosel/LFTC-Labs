# Lab 3 — Lexical Analyzer

**Course:** Formal Languages and Compiler Design (LFTC)  
**Lab 3** — Lexical Analysis using Flex + C

---

## 1. Overview

This project implements a **lexical analyzer (scanner)** for the mini-language defined in Lab 1, using **Flex** for token pattern matching and **C** for data structure management.

### Input
- **Token list** ([tokens.txt](tokens.txt)) — defines all valid lexical tokens and their integer codes
- **Source program** — a `.txt` file written in the mini-language

### Output
- **Program Internal Form (PIF)** — a linear sequence of `(token_code, ST_position)` pairs
- **Symbol Table (ST)** — a single hash-table storing both identifiers and constants
- **Lexical errors** — reported to `stderr` with line numbers

---

## 2. Tool Usage — Flex

[Flex](https://github.com/westes/flex) is a fast lexical analyzer generator. It reads a `.l` specification file containing pattern–action rules and produces a C source file (`lex.yy.c`) with the function `yylex()`.

### How Flex is used in this project

The file [`lexer.l`](lexer.l) is structured in three sections separated by `%%`:

| Section | Purpose |
|---|---|
| **Definitions** | `%{ ... %}` C preamble (includes, globals), Flex options (`%option noyywrap`), and named regex definitions (`LETTER`, `DIGIT`, `INTEGER`, `REAL`, `IDENTIFIER`) |
| **Rules** | Pattern–action pairs: each regex pattern triggers a C action that inserts the recognized token into the PIF and, for identifiers/constants, into the Symbol Table |
| **User code** | `main()` function |

### Key Flex features used
- **Longest-match rule**: when multiple patterns match the current input, Flex picks the one consuming the most characters (e.g. `==` is preferred over `=`)
- **First-match rule**: when two patterns match the same length, the rule listed first wins — reserved words (`"int"`, `"if"`, …) are listed before the general `{IDENTIFIER}` pattern
- **`%option noyywrap`**: disables multi-file input (no need for `-lfl` at link time)

---

## 3. Token Code Convention

Token codes are split into two ranges to avoid collisions:

| Range | Usage | Examples |
|---|---|---|
| **0–127 (ASCII)** | Single-character operators and separators use their raw ASCII value | `'+'` = 43, `';'` = 59, `'{'` = 123 |
| **≥ 256** | Reserved words, multi-char operators, identifiers, and constants | `TOK_ID` = 256, `TOK_PROGRAM` = 261, `TOK_EQ` = 266 |

This makes single-character token handling trivial (the token code *is* the character) and ensures no overlap between the two ranges.

---

## 4. Lexical Token Detection

### 4.1. Reserved Words

The 8 reserved words are matched as **literal strings** before the identifier rule:

```
"else"  "if"  "int"  "program"  "read"  "real"  "while"  "write"
```

Because they appear before the `{IDENTIFIER}` rule and Flex uses first-match for equal-length matches, a word like `int` is always recognized as a reserved word, never as an identifier.

### 4.2. Operators

Multi-character operators (`==`, `!=`, `<=`, `>=`) are listed before single-character ones (`=`, `<`, `>`). Flex's longest-match rule ensures `==` is never split into two `=` tokens.

Single-character operators (`+`, `-`, `*`, `/`, `=`, `<`, `>`) pass their ASCII value as the token code directly.

### 4.3. Identifiers

Matched by the regex:
```
{LETTER}({LETTER}|{DIGIT}|_)*
```
Where `LETTER = [a-zA-Z]` and `DIGIT = [0-9]`. The identifier must start with a letter and may contain letters, digits, and underscores (max 256 characters, enforced in the action code).

Upon recognition, the identifier is **inserted into the Symbol Table**. The returned index is stored in the PIF entry.

### 4.4. Constants

Two constant patterns are recognized:

| Type | Regex | Examples |
|---|---|---|
| Integer | `0 \| [1-9][0-9]*` | `0`, `5`, `42`, `100` |
| Real | `(0 \| [1-9][0-9]*)\.[0-9]+` | `0.0`, `3.14`, `10.5` |

**Note on signed constants:** The BNF allows an optional sign (`+`/`-`) before integer constants. In the lexer, the sign is treated as a separate operator token, which is standard practice — it avoids ambiguity in expressions like `a - 5` vs. `a = -5`. The parser stage handles unary operators.

Constants are inserted into the same Symbol Table as identifiers.

### 4.5. Error Handling

Any character not matched by the rules above triggers the catch-all rule (`.`), which reports a lexical error with the line number and the offending character on `stderr`. The lexer **continues scanning** after an error (error recovery), producing a complete PIF alongside any error messages.

---

## 5. Data Structures

### 5.1. Symbol Table — Hash Table with Separate Chaining

**Files:** [`symbol_table.h`](symbol_table.h), [`symbol_table.c`](symbol_table.c)

A **single unified hash table** stores both identifiers and constants. Each distinct string inserted receives a unique, strictly incremental integer index starting at 0.

#### Structure
```
┌─────────┐
│ bucket 0 │ → [key="sum", idx=6] → NULL
│ bucket 1 │ → NULL
│ bucket 2 │ → [key="i", idx=1] → [key="0", idx=9] → NULL
│   ...    │
│bucket 210│ → NULL
└─────────┘
```

- **211 buckets** (prime number for good distribution)
- **djb2 hash function** (Dan Bernstein) — fast, well-distributed for short strings
- **Separate chaining** with singly-linked lists for collision resolution

#### Operations

| Operation | Avg. Complexity | Description |
|---|---|---|
| `st_init(st)` | O(m) | Clear all m=211 buckets |
| `st_insert(st, key)` | O(1) amortized | Insert key; returns index (existing or newly assigned) |
| `st_search(st, key)` | O(1) amortized | Search by key; returns index or -1 |
| `st_get_key(st, idx)` | O(n) | Reverse lookup: index → key (for PIF display) |
| `st_print(st, out)` | O(n + m) | Print all entries ordered by index |
| `st_free(st)` | O(n + m) | Deallocate all nodes |

**Duplicate handling:** If a key already exists, `st_insert` returns the existing index without creating a new node.

### 5.2. Program Internal Form (PIF)

**Files:** [`pif.h`](pif.h), [`pif.c`](pif.c)

A dynamic array of `(token_code, st_position)` pairs:

```c
typedef struct {
    int token_code;     /* Token code (ASCII char or >= 256) */
    int st_position;    /* ST index, or -1 if not in ST      */
} PIFEntry;
```

| Token category | `token_code` | `st_position` |
|---|---|---|
| Reserved word | 258–265 | -1 (self-identifying) |
| Multi-char operator | 266–269 | -1 (self-identifying) |
| Single-char operator / separator | ASCII value | -1 (self-identifying) |
| Identifier | 256 (`TOK_ID`) | Index in ST |
| Constant | 257 (`TOK_CONST`) | Index in ST |

The `pif_print()` function uses `token_name()` from `tokens.c` to display readable token names, and `st_get_key()` from the Symbol Table to resolve identifier/constant lexemes.

The array starts with capacity 256 and doubles when full.

### 5.3. Token Codes

**Files:** [`tokens.h`](tokens.h), [`tokens.c`](tokens.c)

Codes ≥ 256 are defined as `#define` macros. The `token_name(code)` function handles both ranges: it returns keyword/operator strings for codes ≥ 256 and the ASCII character for codes < 256.

See [`tokens.txt`](tokens.txt) for the complete token list.

---

## 6. Building & Running

### Prerequisites
- GCC (or any C99 compiler)
- Flex (`flex`)

### Build
```bash
make
```

### Run
```bash
./lexer programs/program1.txt         # ArrayProcessor — no errors
./lexer programs/program2.txt         # DataClassifier — no errors
./lexer programs/program3_errors.txt  # Error demo — detects # and &
```

Or use the Makefile targets:
```bash
make run1   # program1
make run2   # program2
make run3   # program3 (with errors)
```

### Clean
```bash
make clean
```

---

## 7. Project Structure

```
Lab3-Lexer/
├── Makefile              # Build system
├── README.md             # This documentation
├── bnf.txt               # Language specification (from Lab 1)
├── tokens.txt            # Token list with codes
├── tokens.h / tokens.c   # Token code definitions (≥256) & name lookup
├── symbol_table.h / .c   # Hash table with separate chaining
├── pif.h / pif.c         # Program Internal Form (dynamic array + print)
├── lexer.l               # Flex specification + main()
└── programs/
    ├── program1.txt      # ArrayProcessor (from Lab 1)
    ├── program2.txt      # DataClassifier (from Lab 1)
    └── program3_errors.txt  # Program with intentional lexical errors
```
