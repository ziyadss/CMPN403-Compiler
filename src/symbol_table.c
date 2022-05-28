#include "helpers.h"
#include "symbol_table.h"

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

extern int yyerror(const char *format, ...);

struct SymbolTable *current_scope = NULL;
enum SEMANTIC_ERROR semantic_error = NO_ERROR;

unsigned int hash(char *string)
{
    int hash = stbds_hash_string(string, HASH_SEED);
    return hash % ST_ARRAY_SIZE;
}

struct SymbolTableEntry *search_bucket(struct SymbolTableEntry *node, char *identifier)
{
    while (node != NULL)
    {
        if (strcmp(node->name, identifier) == 0)
            return node;
        node = node->next;
    }

    return NULL;
}

struct SymbolTableEntry *search_tables(struct SymbolTable *table, char *identifier)
{
    unsigned int bucket = hash(identifier);

    while (table != NULL)
    {
        struct SymbolTableEntry *entry = search_bucket(table->buckets[bucket], identifier);
        if (entry != NULL)
            return entry;
        table = table->parent;
    }

    return NULL;
}

struct SymbolTable *create_table()
{
    struct SymbolTable *table = malloc(sizeof(*table));
    assert(table != NULL);

    table->parent = NULL;
    for (unsigned int i = 0; i < ST_ARRAY_SIZE; i++)
        table->buckets[i] = NULL;

    return table;
}

struct SymbolTableEntry *insert(char *identifier, _Bool is_const, _Bool is_init, _Bool is_func, _Bool is_func_parameter)
{
    unsigned int bucket = hash(identifier);
    struct SymbolTableEntry *head = current_scope->buckets[bucket];

    if (search_bucket(head, identifier) != NULL)
    {
        semantic_error = USED_IDENTIFIER;
        return NULL;
    }

    struct SymbolTableEntry *entry = malloc(sizeof(*entry));
    assert(entry != NULL);

    entry->name = identifier;
    entry->next = head;
    entry->types = NULL;
    entry->is_init = is_init;
    entry->is_used = 0;
    entry->is_func = is_func;
    entry->is_const = is_const;
    entry->is_func_parameter = is_func_parameter;

    current_scope->buckets[bucket] = entry;

    semantic_error = NO_ERROR;
    return entry;
}

_Bool changeParameters(char *identifier, enum TYPE* types, _Bool func, _Bool init)
{
    struct SymbolTableEntry *entry = lookup(identifier, func, init);

    if (entry == NULL)
        return 0;

    entry->types = types;
    return 1;
}

_Bool changeListParams(char **arr, enum TYPE* types)
{
    for (ptrdiff_t i = 0; i < arrlen(arr); ++i) 
    {
        if (!changeParameters(arr[i], types, 0, 0 ))
            return 0;
    }
    return 1;
}
void scope_down()
{
    struct SymbolTable *new_scope = create_table();
    new_scope->parent = current_scope;
    current_scope = new_scope;
}

void scope_up()
{
    struct SymbolTable *old_scope = current_scope;
    current_scope = old_scope->parent;
    // destroy_table(old_scope);
}

struct SymbolTableEntry *lookup(char *identifier, _Bool func, _Bool init)
{
    struct SymbolTableEntry *found = search_tables(current_scope, identifier);
    // run checks, as parameters to lookup (const, init, func)
    // if found == NULL OR checks fail, return NULL;
    if (found == NULL)
        semantic_error = UNDECLARED_IDENTIFIER;
    // else if (found->is_init == 0 && init == 0)
    //     semantic_error = UNINITIALIZED_IDENTIFIER;
    // else if (found->is_func == 0 && func == 1)
    //     semantic_error = NOT_A_FUNCTION;
    // else if (found->is_func == 1 && func == 0)
    //     semantic_error = IS_A_FUNCTION;
    else
        return found;

    return NULL;
}

char *get_error_message()
{
    switch (semantic_error)
    {
    case NO_ERROR:
        return NULL;
    case USED_IDENTIFIER:
        return "Identifier already used";
    case UNDECLARED_IDENTIFIER:
        return "Identifier not declared";
    case UNINITIALIZED_IDENTIFIER:
        return "Identifier not initialized";
    case NOT_A_FUNCTION:
        return "Identifier is not a function";
    case IS_A_FUNCTION:
        return "Identifier is a function";
    default:
        return "Unknown error";
    }
}

enum TYPE* generate_array() 
{
    enum TYPE* arr = NULL;    
    return arr;
}
//mesh ha generate signleton 

enum TYPE* insert_into_array(enum TYPE *arr, enum TYPE type)
{
    arrput(arr, type);
    return arr;
}

void delete_array(enum TYPE **arr)
{
    if (arr == NULL)
        return;

    arrfree(*arr);
    arr = NULL;
}

//todo template later
char **generate_char_array()
{
    char** arr = NULL; 
    return arr;
}

char **insert_into_char_array(char **arr, char* identifier)
{
    arrput(arr, identifier);
    return arr;
}

void delete_char_array(char ***arr)
{
    if (arr == NULL)
        return;

    arrfree(*arr);
    arr = NULL;
}
//nice

void destroy_table(struct SymbolTable *table)
{
    for (unsigned int i = 0; i < ST_ARRAY_SIZE; i++)
    {
        struct SymbolTableEntry *head = table->buckets[i];
        while (head != NULL)
        {
            struct SymbolTableEntry *next = head->next;
            delete_array(&(head->types));
            free(head);
            head = next;
        }
    }

    free(table);
}



void destroy_global_table()
{
    destroy_table(current_scope);
}
