#pragma once

#include "ast_print.c"
#include "helpers.c"

FILE *output_file = NULL;

char *_operation(struct AST_Node *operation);
char *_block(struct AST_Node *block);

char *_statement(struct AST_Node *statement)
{
    char *ret;
    switch (statement->tag)
    {
    case NODE_TYPE_STATEMENTS:
        ret = _block(statement);
        break;
    case NODE_TYPE_OPERATION:
        ret = _operation(statement);
        break;
    case NODE_TYPE_IDENTIFIER:
        ret = statement->identifier;
        break;
    case NODE_TYPE_INT:
        asprintf(&ret, "%d", statement->intValue);
        break;
    case NODE_TYPE_FLOAT:
        asprintf(&ret, "%f", statement->floatValue);
        break;
    case NODE_TYPE_CHAR:
        asprintf(&ret, "%c", statement->charValue);
        break;
    case NODE_TYPE_STRING:
        ret = statement->stringValue;
        break;
    case NODE_TYPE_BOOL:
        ret = statement->boolValue ? "1" : "0";
        break;
    default:
        ret = NULL;
        fprintf_s(stderr, "Invalid statement: %d\n", statement->tag);
    }
    return ret;
}

char *_block(struct AST_Node *block)
{
    for (unsigned int i = 0; i < block->statements_count; i++)
        _statement(block->statements[i]);
    return NULL;
}

char *_operation(struct AST_Node *operation)
{
    char *ret;
    switch (operation->op)
    {
    case FUNC_DECL:
        fprintf_s(output_file, "%s: \n", operation->left->identifier);
        _block(operation->right);
        fprintf_s(output_file, "RET\n");
        ret = NULL;
        break;

    case ASSIGN_OP:
        char *src = _statement(operation->right);
        fprintf_s(output_file, "MOV %s, %s\n", operation->left->identifier, src);
        ret = operation->left->identifier;
        break;

    case CALL_OP:
        fprintf_s(output_file, "CALL %s\n", operation->left->identifier);
        ret = "t";
        break;

    default:
        fprintf_s(output_file, "OP %s, %s\n", "target", "val");
        ret = NULL;
        break;
    }
    return ret;
}

void quadruples(char *filename)
{
    output_file = fopen(filename, "w");
    assert(output_file != NULL);

    for (unsigned int i = 0; i < program->statements_count; i++)
    {
        struct AST_Node *node = program->statements[i];
        assert(node->tag == NODE_TYPE_OPERATION);
        _operation(node);
        fprintf_s(output_file, "\n");
    }

    fclose(output_file);
}