# CMPN403-Compiler

Languages and Compilers Project - third year CUFE students.

A compiler for a language inspired by C.  
Major differences include:

- No pointers.
- No casts.
- No structs or unions.
- A 'try-catch-finally' block and a 'throw' statement.
- A built-in string type.

## Notes about generated quadruples

- Assumes the presence of four registers - retval, retadr, temp1, temp2.

## Building and Running

```shell
    make
    ./bin/compiler.exe tests/source.c
```

## Team Members

- Ahmed Ayman Saad
- Khaled Ashraf Zohair
- Mazen Amr
- Muhab Hossam
- Ziyad Sameh Sobhy
