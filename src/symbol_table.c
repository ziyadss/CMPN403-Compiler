#pragma once

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

struct SymbolTableEntry *search_bucket(struct SymbolTableEntry *node, char *identifier)
{
    while (node != NULL)
    {
        if (strcmp(node->identifier, identifier) == 0)
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

// TODO: A hash function that produces the same hash for identical strings.
//  Note that ST_ARRAY_SIZE is currently set to 1 so that there is only one bucket.

// TODO: SymbolTableEntry properties
