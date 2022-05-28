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
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (yyin == NULL)
    {
        fprintf(stderr, "Error: Could not open file %s\n", argv[1]);
        return 1;
    }

    create_program();

    scope_down();

    yyparse();

    scope_up();

    fclose(yyin);

    quadruples("output.asm");

    printf("\nParsing complete.\n");

    return 0;
}
