# Lab 3 — Lexical Analyzer
# Build:  make
# Run:    make run1 / make run2 / make run3

CC      = gcc
CFLAGS  = -Wall -Wextra -std=c99
LEX     = flex

TARGET  = lexer

.PHONY: all clean run1 run2 run3

all: $(TARGET)

lex.yy.c: lexer.l tokens.h symbol_table.h pif.h
	$(LEX) lexer.l

$(TARGET): lex.yy.c tokens.c symbol_table.c pif.c
	$(CC) $(CFLAGS) -o $(TARGET) lex.yy.c tokens.c symbol_table.c pif.c

clean:
	rm -f $(TARGET) lex.yy.c

run1: $(TARGET)
	@echo "=== program1.txt (ArrayProcessor) ==="
	./$(TARGET) programs/program1.txt

run2: $(TARGET)
	@echo "=== program2.txt (DataClassifier) ==="
	./$(TARGET) programs/program2.txt

run3: $(TARGET)
	@echo "=== program3_errors.txt (Error demo) ==="
	./$(TARGET) programs/program3_errors.txt
