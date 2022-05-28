#ifndef QUADRUPLES_H
#define QUADRUPLES_H

#include "ast.h"
#include "helpers.h"
#include "symbol_table.h"

char *_gen_label();
void _parameters_pop(struct AST_Node *parameters);
void _parameters_push(struct AST_Node *parameters);
char *_inv_cond_operation(struct AST_Node *operation, char **jmp);
char *_condition(struct AST_Node *condition);
char *_if(struct AST_Node *statement, _Bool ternary);
char *_node(struct AST_Node *statement, _Bool left, _Bool ternary);
char *_block(struct AST_Node *block);
void _operation_dst(char *identifier, struct AST_Node *operation);
char *_operation(struct AST_Node *operation, _Bool left);
void quadruples();

#endif // QUADRUPLES_H