#include "symbol_table.c"

enum SyntaxError
{
    NO_ERROR, USED_IDENTIFIER
};

extern struct SymbolTable *current_scope;

void print_table(unsigned int line)
{
    struct SymbolTable *table = current_scope;
    printf_s("\nAt line %d, current table: ", line);
    while (table != NULL)
    {
        for (int i = 0; i < ST_ARRAY_SIZE; i++)
        {
            struct SymbolTableEntry *head = table->buckets[i];
            while (head != NULL)
            {
                printf_s("%s, ", head->identifier);
                head = head->next;
            }
        }
        printf_s("\nParent: ");
        table = table->parent;
    }
    printf_s("Global Scope\n\n");
}

enum SyntaxError insert(char *identifier, _Bool is_const, _Bool is_init, _Bool is_func)
{
    unsigned int bucket = hash(identifier);
    struct SymbolTableEntry *head = current_scope->buckets[bucket];

    if (search_bucket(head, identifier) != NULL)
        return USED_IDENTIFIER;

    struct SymbolTableEntry *entry = malloc(sizeof(*entry));
    assert(entry != NULL);

    entry->identifier = identifier;
    entry->next = head;
	entry->TYPES = NULL;
	entry->is_init = is_init;
	entry->is_used = 0;
	entry->is_func = is_func;
	entry->is_const = is_const;

    current_scope->buckets[bucket] = entry;

    return NO_ERROR;
}

void is_used(char *identifier)
{
	struct SymbolTableEntry* used_entry = search_tables(current_scope, identifier);
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
    destroy_table(old_scope);
}
