#define ST_ARRAY_SIZE 1

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

// template el ghalaba - abdo
#define MAP_KV(T, Name)       \
    typedef struct Map_##Name \
    {                         \
        char *key;            \
        T value;              \
    } Map_##Name##_El, *Map_##Name

struct SymbolTableEntry
{
    char *identifier;
    /* ... */

    struct SymbolTableEntry *next;
};

struct SymbolTable
{
    struct SymbolTableEntry *buckets[ST_ARRAY_SIZE];

    struct SymbolTable *parent;
};

unsigned int hash(char *string)
{
    return strlen(string) % ST_ARRAY_SIZE;
}

struct SymbolTableEntry *search(struct SymbolTableEntry *node, char *identifier)
{
    while (node != NULL)
    {
        if (strcmp(node->identifier, identifier) == 0)
            return node;
        node = node->next;
    }

    return NULL;
}

struct SymbolTable *create_table()
{
    struct SymbolTable *table = malloc(sizeof(*table));
    assert(table != NULL);

    table->parent = NULL;
    for (int i = 0; i < ST_ARRAY_SIZE; i++)
        table->buckets[i] = NULL;

    return table;
}

void destroy_table(struct SymbolTable *table)
{
    struct SymbolTableEntry **entries = table->buckets;

    for (int i = 0; i < ST_ARRAY_SIZE; ++i)
    {
        while (entries[i]->next != NULL)
        {
            struct SymbolTableEntry *tmp = entries[i];
            entries[i] = entries[i]->next;
            free(tmp);
        }
        free(entries[i]);
    }

    free(table);
}

// TODO: A search function that searches in the table, if not found in its parent, etc etc. It calls the currently //  implemented search function for the one correct bucket in each table.
// scope up returns a symboltable with an array of STEs, where STEs are ordered from the top of the scope to the bottom of the scope
// counter example: a parent scope that continues after a child scope will return a match if the declaration is found in the bottom half of the parent scope
// which is obviously wrong
//
// TODO: scope insertion

// TODO: A hash function that produces the same hash for identical strings.
//  Note that ST_ARRAY_SIZE is currently set to 1 so that there is only one bucket.
//  IMPORTANT: DONE***********

// TODO: SymbolTableEntry properties
