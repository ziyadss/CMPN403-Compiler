#include <stdio.h>

extern int yyparse();
extern FILE *yyin;

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

    yyparse();

    fclose(yyin);

    printf("Parsing complete.\n");

    return 0;
}
