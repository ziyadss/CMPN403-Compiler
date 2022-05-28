#include <stdio.h>

#include "quadruples.c"

extern int yyparse();
extern FILE *yyin;
extern void scope_down();
extern void scope_up();
struct AST_Node *create_program();

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

    program = create_program();

    current_scope = create_table();

    yyparse();

    fclose(yyin);

    printf("Parsing complete.\n");

    quadruples("output.asm");

    printf("\nCode generation complete.\n");

    destroy_table(current_scope);
    destroy_ast(program);

    printf("\nCleanup successful.\n");

    return 0;
}
