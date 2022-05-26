#define ST_ARRAY_SIZE 1
#define HASHING_P 67
#define HASHING_M 1000000009

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum SyntaxError
{
    NO_ERROR,
    USED_IDENTIFIER
};

enum KIND
{
    FUNCT,
    PAR,
    VAR
};

enum MODIFIER
{
    AUTO,
    CONST,
    STATIC,
    UNSIGNED,
    NONE
};

enum TYPE
{
    BOOL,
    CHAR,
    DOUBLE,
    FLOAT,
    INT,
    LONG,
    SHORT,
    STRING,
    VOID
};

struct SymbolTable
{
    struct SymbolTableEntry
    {
        char *identifier;
        // KIND kind;
        // type
        // MODIFIER modifier;

        struct SymbolTableEntry *next;
    } * buckets[ST_ARRAY_SIZE];

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

enum SyntaxError insert(struct SymbolTable *table, char *identifier)
{
    unsigned int bucket = hash(identifier);
    struct SymbolTableEntry *head = table->buckets[bucket];

    if (search(head, identifier) != NULL)
        return USED_IDENTIFIER;

    struct SymbolTableEntry *entry = malloc(sizeof(*entry));
    assert(entry != NULL);

    entry->identifier = identifier;
    entry->next = head;

    table->buckets[bucket] = entry;

    return NO_ERROR;
}

static struct SymbolTable *current_scope;

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

int main()
{
    current_scope = create_table();

    int result;
    result = insert(current_scope, "Variable1");
    assert(result == NO_ERROR);

    scope_down();

    result = insert(current_scope, "Variable1");
    assert(result == NO_ERROR);

    scope_up();

    result = insert(current_scope, "Variable1");
    assert(result == USED_IDENTIFIER);

    destroy_table(current_scope);

    printf("All tests passed.\n");

    return 0;
}

// TODO: A search function that searches in the table, if not found in its parent, etc etc. It calls the currently
//  implemented search function for the one correct bucket in each table.

// TODO: A hash function that produces the same hash for identical strings.
//  Note that ST_ARRAY_SIZE is currently set to 1 so that there is only one bucket.

// TODO: SymbolTableEntry properties
