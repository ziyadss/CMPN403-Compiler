#include <stdio.h>

#include "../src/symbol_table_interface.c"

struct SymbolTable *current_scope = NULL;

int main()
{
    enum SemanticError result;

    scope_down();

    result = insert("Variable1", 0, 0, 0);
    assert(result == NO_ERROR);

    scope_down();

    result = insert("Variable1", 0, 0, 0);
    assert(result == NO_ERROR);

    result = insert("Variable2", 0, 0, 0);
    assert(result == NO_ERROR);

    scope_up();

    result = insert("Variable1", 0, 0, 0);
    assert(result == USED_IDENTIFIER);

    scope_up();

    printf("All tests passed.\n");

    return 0;
}