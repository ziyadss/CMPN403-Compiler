#include "symbol_table.c"

enum SyntaxError
{
    NO_ERROR, USED_IDENTIFIER
};

extern struct SymbolTable *current_scope;

enum SyntaxError insert(struct SymbolTable *table, char *identifier)
{
    unsigned int bucket = hash(identifier);
    struct SymbolTableEntry *head = table->buckets[bucket];

    if (search_bucket(head, identifier) != NULL)
        return USED_IDENTIFIER;

    struct SymbolTableEntry *entry = malloc(sizeof(*entry));
    assert(entry != NULL);

    entry->identifier = identifier;
    entry->next = head;

    table->buckets[bucket] = entry;

    return NO_ERROR;
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
    destroy_table(old_scope);
}