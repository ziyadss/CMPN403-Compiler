#include <stdio.h>

#include "quadruples.c"

extern int yyparse();
extern FILE *yyin;
extern void scope_down();
extern void scope_up();
extern void create_program();

struct SymbolTable *current_scope = NULL;
struct AST_Node *program = NULL;
enum SemanticError semantic_error = NO_ERROR;

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

    printf("\nParsing complete.\n");

    quadruples("output.asm");

    printf("\nCode generation complete.\n");

    return 0;
}
