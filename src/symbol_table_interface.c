#pragma once

#include "symbol_table.c"

enum SemanticError
{
    NO_ERROR,
    USED_IDENTIFIER
};

extern struct SymbolTable *current_scope;
extern enum SemanticError semantic_error;

void print_table(unsigned int line)
{
    struct SymbolTable *table = current_scope;
    printf("\nAt line %d, current table: ", line);
    while (table != NULL)
    {
        for (unsigned int i = 0; i < ST_ARRAY_SIZE; i++)
        {
            struct SymbolTableEntry *head = table->buckets[i];
            while (head != NULL)
            {
                printf("%s, ", head->name);
                head = head->next;
            }
        }
        printf("\nParent: ");
        table = table->parent;
    }
    printf("Global Scope\n\n");
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
    assert(entry != NULL);

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

void is_used(char *identifier)
{
    struct SymbolTableEntry *used_entry = search_tables(current_scope, identifier);
    used_entry->is_used = 1;
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
        semantic_error = USED_IDENTIFIER;
        return NULL;
    }

    return found;
}