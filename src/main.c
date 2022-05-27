#include <stdio.h>

#include "quadruples.c"

extern int yyparse();
extern FILE *yyin;
extern void scope_down();
extern void scope_up();
extern void create_program();

struct SymbolTable *current_scope = NULL;
struct AST_Node *program = NULL;

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf_s(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    if (fopen_s(&yyin, argv[1], "r") != 0)
    {
        fprintf_s(stderr, "Error: Could not open file %s\n", argv[1]);
        return 1;
    }

    create_program();

    scope_down();

    yyparse();

    scope_up();

    fclose(yyin);

    quadruples("output.asm");

    printf_s("\nParsing complete.\n");

    return 0;
}
