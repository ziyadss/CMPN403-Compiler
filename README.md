# CMPN403-Compiler

Languages and Compilers Project - third year CUFE students.

A compiler for a mini-language inspired by C.  
Major differences include:

- No pointers.
- No casts.
- No structs or unions.
- A 'try-catch-finally' block and a 'throw' statement.
- A built-in string type.

Currently in phase 1, this project consists of two phases:

- Lexer and parser rules.
- TODO

## Building

```shell
    mkdir -p build bin
    lex -o build/lex.yy.c src/lexer.l
    yacc -o build/y.tab.c -d src/parser.y
    gcc -Wall -Wextra -o bin/compiler.exe build/lex.yy.c build/y.tab.c
```

## Running

```shell
    ./bin/compiler.exe src/source.c
```

## Team Members

- Ahmed Ayman Saad
- Khaled Ashraf Zohair
- Mazen Amr
- Muhab Hossam
- Ziyad Sameh Sobhy
