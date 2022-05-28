#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define ST_ARRAY_SIZE 1
#define HASH_SEED 0

enum TYPE
{
    BOOL_TYPE,
    CHAR_TYPE,
    DOUBLE_TYPE,
    FLOAT_TYPE,
    INT_TYPE,
    LONG_TYPE,
    SHORT_TYPE,
    SIGNED_TYPE,
    STRING_TYPE,
    UNSIGNED_TYPE,
    VOID_TYPE,
    ENUM_TYPE
};

struct SymbolTableEntry
{
    char *name;
    enum TYPE *TYPES;
    _Bool is_init;
    _Bool is_used;
    _Bool is_func;
    _Bool is_const;

    struct SymbolTableEntry *next;
};

struct SymbolTable
{
    struct SymbolTableEntry *buckets[ST_ARRAY_SIZE];

    struct SymbolTable *parent;
};

enum SEMANTIC_ERROR
{
    NO_ERROR,
    USED_IDENTIFIER,
    UNDECLARED_IDENTIFIER,
    UNINITIALIZED_IDENTIFIER,
    NOT_A_FUNCTION,
    IS_A_FUNCTION
};

struct SymbolTableEntry *insert(char *identifier, _Bool is_const, _Bool is_init, _Bool is_func);
void scope_down();
void scope_up();
struct SymbolTableEntry *lookup(char *identifier, _Bool func, _Bool init);

#endif // SYMBOL_TABLE_H