#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define ST_ARRAY_SIZE 1
#define HASH_SEED 0

enum TYPE
{
    INT_TYPE
    /*...*/
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

enum SemanticError
{
    NO_ERROR,
    USED_IDENTIFIER
};

struct SymbolTableEntry *insert(char *identifier, _Bool is_const, _Bool is_init, _Bool is_func);
void scope_down();
void scope_up();
struct SymbolTableEntry *lookup(char *identifier);

#endif // SYMBOL_TABLE_H