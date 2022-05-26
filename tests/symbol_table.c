#include <stdio.h>

#include "../src/symbol_table_interface.c"

struct SymbolTable *current_scope = NULL;

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