#include <stdio.h>

extern int yyparse();
extern FILE *yyin;

struct SymbolTable *current_scope = NULL;

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

    yyparse();

    fclose(yyin);

    printf_s("Parsing complete.\n");

    return 0;
}
