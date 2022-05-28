#ifndef QUADRUPLES_H
#define QUADRUPLES_H

#include "ast.h"
#include "helpers.h"
#include "symbol_table.h"

void quadruples();

struct ReturnTuple {
    struct AST_Node *AST_component;
    char **char_array;
};

#endif // QUADRUPLES_H