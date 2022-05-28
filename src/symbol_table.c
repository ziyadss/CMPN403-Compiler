#include "symbol_table.h"
#include "helpers.h"

extern int yyerror(const char *format, ...);

struct SymbolTable *current_scope = NULL;
enum SemanticError semantic_error = NO_ERROR;

enum TYPE {
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

struct SymbolTableEntry *insert(char *identifier, _Bool is_const, _Bool is_init, _Bool is_func)
{
    unsigned int bucket = hash(identifier);
    struct SymbolTableEntry *head = current_scope->buckets[bucket];

    if (search_bucket(head, identifier) != NULL)
    {
        semantic_error = USED_IDENTIFIER;
        return NULL;
    }

    struct SymbolTableEntry *entry = malloc(sizeof(*entry));
    assert(entry != NULL)
	{

    entry->name = identifier;
    entry->next = head;
    entry->TYPES = NULL;
    entry->is_init = is_init;
    entry->is_used = 0;
    entry->is_func = is_func;
    entry->is_const = is_const;

    current_scope->buckets[bucket] = entry;

    semantic_error = NO_ERROR;
    return entry;
}

void insertParam(struct SymbolTableEntry* entry, enum TYPE type)
{
	arrput(entry->TYPES, type);
	
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

struct SymbolTableEntry *lookup(char *identifier)
{
    struct SymbolTableEntry *found = search_tables(current_scope, identifier);
    // run checks, as parameters to lookup (const, init, func)
    // if found == NULL OR checks fail, return NULL;
    if (found == NULL)
    {
        semantic_error = UNDECLARED_IDENTIFIER;
        return NULL;
    }

    return found;
}

char *get_error_message()
{
    switch(semantic_error)
    {
        case NO_ERROR:
            return NULL;
        case USED_IDENTIFIER:
            return "Identifier already used";
        case UNDECLARED_IDENTIFIER:
            return "Identifier not declared";
        default:
            return "Unknown error";
    }
}

void destroy_table(struct SymbolTable *table)
{
    for (unsigned int i = 0; i < ST_ARRAY_SIZE; i++)
    {
        struct SymbolTableEntry *head = table->buckets[i];
        while (head != NULL)
        {
            struct SymbolTableEntry *next = head->next;
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
