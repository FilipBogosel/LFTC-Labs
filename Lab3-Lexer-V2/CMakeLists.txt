cmake_minimum_required(VERSION 4.3)
project(Lab3_Lexer_V2 C)

set(CMAKE_C_STANDARD 99)
add_compile_options(-Wall -Wextra -pedantic)
add_executable(Lab3_Lexer_V2
        main.c
        lex.yy.c
        symbol_table.c
        pif.c
        tokens.c
)
