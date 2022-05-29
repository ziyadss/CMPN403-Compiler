#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "helpers.h"
#include "prints.h"

#define ST_ARRAY_SIZE 1
#define HASH_SEED 0

enum TYPE
{
    INVALID = -1,
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
    ENUM_TYPE,
    CONST_TYPE
};

struct SymbolTableEntry
{
    char *name;
    enum TYPE *types;
    _Bool is_init;
    _Bool is_used;
    _Bool is_func;
    _Bool is_param;
    _Bool is_const;

    enum TYPE main_type;

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
    IS_A_FUNCTION,
    INVALID_TYPE,
    UNDEFINED_FUNCTION,
    INCOMPATIBLE_TYPES
};

struct SymbolTableEntry *insert(char *identifier, _Bool is_const, _Bool is_init, _Bool is_func, _Bool is_param);
void scope_down();
void scope_up();
struct SymbolTableEntry *lookup(char *identifier, _Bool func, _Bool need_init, _Bool init);

struct AST_Node *change_list_params(struct AST_Node *initializer_list, enum TYPE *types, _Bool param);
enum TYPE *insert_into_array(enum TYPE *arr, enum TYPE type);

void def_func(struct SymbolTableEntry *entry);

#endif // SYMBOL_TABLE_H