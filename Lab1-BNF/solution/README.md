# Mini-Language Documentation

**Course:** Formal Languages and Compiler Design (LFTC)  
**Lab 1** — Mini-Language Specification & Use Case

---

## 1. Language Overview

This is a minimal, C-style procedural programming language designed for basic numerical computations and data processing. It provides simple control flow, scalar variables, and fixed-size arrays with a clean, concise syntax.

### Key Characteristics:
* **C-like syntax:** Block scoping with braces `{ ... }`, standard operators (`=`, `==`, `!=`), and parenthesized conditions for control statements.
* **Predictable execution:** Static typing, upfront variable declarations, and simple sequential control flow.
* **Minimal token set:** Only 8 reserved keywords, keeping parsing straightforward.

---

## 2. Types

| Type | Description | Example |
|---|---|---|
| `int` | Whole numbers (positive, negative, zero) | `0`, `42`, `-10` |
| `real` | Floating-point decimal numbers | `3.14`, `0.5`, `-1.25` |
| `T[N]` | Fixed-size 1D array of type `T` and size `N` | `int[10]`, `real[5]` |

---

## 3. Statements & Justifications

### 3.1. Assignment (`=`)
Used to store computed values or initialize variables.
```c
total = sum + 10;
```

### 3.2. Input / Output (`read`, `write`)
Used for basic program interaction: `read` retrieves input into a variable, and `write` outputs an expression or value.
```c
read(value);
write(result);
```

### 3.3. Conditional (`if ... else`)
Used for branching logic based on boolean comparison conditions.
```c
if (score >= 50) {
    passed = 1;
} else {
    passed = 0;
}
```

### 3.4. Loop (`while`)
Used for repetitive computations, accumulation loops, and array traversals.
```c
while (i < n) {
    sum = sum + i;
    i = i + 1;
}
```

---

## 4. Example Programs

### Program 1: Maximum of Two Numbers
Reads two integers from input, determines the maximum, and prints the result.

```c
program MaxNumber {
    int a, b;
    int max;

    read(a);
    read(b);

    if (a > b) {
        max = a;
    } else {
        max = b;
    }

    write(max);
}
```

### Program 2: Sum of First N Numbers
Reads a positive integer `n`, accumulates the sum of integers from 1 up to `n`, and outputs the total.

```c
program SumNumbers {
    int n, i;
    int sum;

    read(n);
    sum = 0;
    i = 1;

    while (i <= n) {
        sum = sum + i;
        i = i + 1;
    }

    write(sum);
}
```

### Program 3: Minimum Element in an Array
Reads 5 decimal values into a fixed-size array, finds the minimum element, and prints it.

```c
program ArrayMin {
    real[5] numbers;
    int i, n;
    real min, current;

    n = 5;
    i = 0;

    while (i < n) {
        read(current);
        numbers[i] = current;
        i = i + 1;
    }

    min = numbers[0];
    i = 1;

    while (i < n) {
        if (numbers[i] < min) {
            min = numbers[i];
        }
        i = i + 1;
    }

    write(min);
}
```
