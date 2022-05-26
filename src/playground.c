#define ST_ARRAY_SIZE 1

#include <string.h>

enum SyntaxError
{
    NO_ERROR,
    USED_IDENTIFIER
};

struct SymbolTable
{
    struct SymbolTableEntry
    {
        char *identifier;
        /* ... */

        struct SymbolTableEntry *next;
    };

    struct SymbolTable *parent;
    struct SymbolTableEntry *buckets[ST_ARRAY_SIZE];
};

unsigned int hash(char *string)
{
    static unsigned int counter = 0;
    return counter++ % ST_ARRAY_SIZE;
}

_Bool search(struct SymbolTableEntry *node, char *identifier)
{
    do
    {
        if (strcmp(node->identifier, identifier) == 0)
            return 1;
    } while (node = node->next);

    return 0;
}

int insert(struct SymbolTable *table, char *identifier)
{
    unsigned int bucket = hash(identifier);
    struct SymbolTableEntry *head = table->buckets[bucket];

    if (search(head, identifier))
        return USED_IDENTIFIER;

    struct SymbolTableEntry *entry = malloc(sizeof(*entry));
    entry->identifier = identifier;
    entry->next = head;

    table->buckets[bucket] = entry;

    return NO_ERROR;
}

static struct SymbolTable *current_scope;

int destroy_table(struct SymbolTable *table)
{
    // destroy the buckets
    free(table);
}

int scope_down()
{
    struct SymbolTable *new_scope = malloc(sizeof(*new_scope));
    new_scope->parent = current_scope;
    current_scope = new_scope;
}

int scope_up()
{
    struct SymbolTable *old_scope = current_scope;
    current_scope = old_scope->parent;
    destroy_table(old_scope);
}

int main()
{
    current_scope = malloc(sizeof(*current_scope));

    int result;
    result = insert(current_scope, "Variable1");
    // result should be NO_ERROR

    scope_down();

    result = insert(current_scope, "Variable1");
    // result should be NO_ERROR

    scope_up();

    result = insert(current_scope, "Variable1");
    // result should be USED_IDENTIFIER

    destroy_table(current_scope);

    return 0;
}

// TODO: A search function that searches in the table, if not found in its parent, etc etc. It calls the currently
//  implemented search function for the one correct bucket in each table.

// TODO: A hash function that produces the same hash for identical strings. Can do strlen for now??
//  Note that ST_ARRAY_SIZE is currently set to 1 so that there is only one bucket.

// TODO: SymbolTableEntry properties

// TODO: destroy_table logic

// TODO: Try running the current main function, it's untested so far.