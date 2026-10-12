# Formal Languages & Compiler Design (LFTC) — Lab 1 Analysis

## 1. Laboratory Requirements Breakdown

Based on `given-docs/Lab1_2026.pdf` and `given-docs/Sample mini-language spec.pdf`, the laboratory requires designing a **Domain-Specific Mini-Language** with formal BNF specifications and practical use-case documentation.

### Core Deliverables:
1. **BNF Specification File (`.txt`)**:
   - Comprehensive lexical specification (Alphabet, Tokens, Identifiers, Constants).
   - Comprehensive syntactic specification (Grammar rules for programs, declarations, statements, expressions).
   - Note: Needs to be retained as a clean single text file, as it will be the foundation for subsequent laboratories (Scanner, Parser, Symbol Table).
2. **Documentation (`README.md` / Document)**:
   - Language domain presentation and purpose.
   - Justification for each required statement type with domain-specific explanatory examples.
   - Sample programs demonstrating practical applications.

### Language Constraints & Minimal Requirements:
* **Types:** At least 2 simple primitive data types (e.g., `int`, `real`) and 1 compound user-defined type (e.g., 1D fixed-size `array`).
* **Statements:**
  1. Assignment statement (`:=` or `=`)
  2. Input / Output statements (`read`, `write`)
  3. Conditional statement (`if ... then ... else`)
  4. Loop statement (`while ... do`)
* **Lexical constraints:**
  - Identifiers must start with a letter and have a maximum length of 256 characters.
  - Constants must strictly match the language's defined primitive types.

---

## 2. Identified Errors in the Provided Sample Specification

The laboratory assignment explicitly notes: `! Find and correct the errors from the following mini-language specification sample`. Below are the critical flaws present in `Sample mini-language spec.pdf` that must be addressed in our implementation:

1. **Broken I/O Production Rule:**
   - *Sample flaw:* `iostmt ::= "READ" | "WRITE" "(" IDENTIFIER ")"`
   - *Issue:* Operator precedence causes `"READ"` to have no argument, while `"WRITE"` requires parentheses.
   - *Correction:* `iostmt ::= "read" "(" identifier ")" | "write" "(" ( expression | identifier ) ")"`
2. **Incomplete `<factor>` in Expression Grammar:**
   - *Sample flaw:* `factor ::= "(" expression ")" | IDENTIFIER`
   - *Issue:* Constants (`10`, `3.14`), array indexing (`arr[i]`), and negative signs are completely missing from factors. Without them, expressions like `x := 5 + y` are syntactically illegal.
   - *Correction:* `factor ::= "(" expression ")" | identifier | constant | array_access`
3. **Missing Operators in `<expression>` and `<term>`:**
   - *Sample flaw:* `expression` only defined `+`, and `term` only defined `*`. Subtraction (`-`) and division (`/`) were omitted.
   - *Correction:* Add `-` to `expression` and `/` to `term` to support full arithmetic with correct precedence.
4. **Token Set Discrepancies:**
   - *Sample flaw:* Reserved words listed `int`, but syntactic rules used `"INTEGER"`, `"BOOLEAN"`, and `"REAL"` which were never registered in the token dictionary. The relation operator `<>` was used in rules but missing from special tokens.
   - *Correction:* Maintain strict 1:1 symmetry between declared special tokens / reserved words and the non-terminals in the grammar.
5. **Ambiguous / Ill-defined Identifier & Constant Lexical Rules:**
   - *Sample flaw:* `identifier ::= letter | letter{letter}{digit}` incorrectly forbids identifiers with digits before letters (e.g., `x1a`), and constant integer rules lacked proper recursion base cases.
   - *Correction:* Define standard regular/BNF rules: `<identifier> ::= <letter> { <letter> | <digit> | "_" }`.

---

## 3. Domain Analysis: Vehicle Telemetry & Autonomous Control (`AutoDrive`)

For our domain-specific language, we selected **Vehicle Telemetry and Autonomous Driving Control (`AutoDrive`)**.

### Why This Domain Excels:
1. **Natural Mapping to Language Primitives:**
   - `int`: Engine RPM, vehicle speed (km/h), selected gear (`-1, 0, 1..6`), millisecond timestamps.
   - `real`: Distance to obstacles (meters), acceleration ($m/s^2$), battery/fuel level (%), brake pressure.
   - `array`: LiDAR distance sweep (e.g., an 8-point radial sensor array `array[8] of real`), or waypoint speed targets.
2. **Clear Statement Justifications:**
   - **`read` / `write`**: Interfacing directly with CAN bus hardware pins, sensor telemetry, and actuator motors.
   - **Assignment**: Setting cruise control targets, updating telemetry states.
   - **`if ... then ... else`**: Automatic Emergency Braking (AEB) thresholds, lane departure alerts, collision prevention.
   - **`while ... do`**: Cruise control speed adjustment loops, sensor polling cycles, obstacle sweep scans.
3. **Future Labs Friendliness (LFTC Labs 2–5):**
   - Clean delimiter-based syntax (`begin ... end`, semicolons `;`) avoids indentation parsing hazards.
   - Standard mathematical operators and unambiguous precedence keep the LL(1) / LR parser in later labs straightforward.

---

## 4. Manual Effort Time Estimation

Carrying out this laboratory manually requires substantial care:

| Phase | Description | Estimated Time |
|---|---|---|
| **Domain Conception & Lexical Design** | Choosing keywords, operators, data types, and token limits | 45 min |
| **Drafting Lexical BNF** | Formulating grammar for letters, digits, IDs, integers, and floats | 1 hr |
| **Drafting Syntactic BNF** | Formulating grammar for programs, declarations, statements, expressions, and precedence | 2 – 2.5 hrs |
| **Error Checking & Disambiguation** | Verifying consistency, fixing dangling-else issues, verifying token sets | 45 min |
| **Documentation & Example Programs** | Writing the domain rationale, justifying all 4 statement categories, crafting 3 complete programs | 1.5 hrs |
| **Total Estimated Time** | | **~5.5 to 7 hours** |
