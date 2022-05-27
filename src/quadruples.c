#pragma once

#include "ast_print.c"
#include "helpers.c"

FILE *output_file = NULL;

char *_operation(struct AST_Node *operation);
char *_block(struct AST_Node *block);

char *_node(struct AST_Node *statement)
{
    char *ret = NULL;
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
        fprintf_s(stderr, "Invalid statement: %d\n", statement->tag);
        break;
    }
    return ret;
}

char *_block(struct AST_Node *block)
{
    for (unsigned int i = 0; i < block->statements_count; i++)
        _node(block->statements[i]);

    return NULL;
}

void _parameters(struct AST_Node *declarations)
{
    
}

void _operation_dst(char *identifier, struct AST_Node *operation)
{
    switch (operation->op)
    {
    case TERNARY_OP:
        break;
    case COMMA_OP:
        break;
    case CALL_OP:
        fprintf_s(output_file, "CALL %s\n", operation->left->identifier);
        _parameters(operation->right);
        fprintf_s(output_file, "MOV %s, retval\n", identifier);
        break;
    case ASSIGN_OP:
        fprintf_s(output_file, "MOV %s, %s\n", operation->left->identifier, _node(operation->right));
        break;
    case ADD_OP:
        fprintf_s(output_file, "ADD %s, %s, %s\n", identifier, _node(operation->left), _node(operation->right));
        break;
    default:
        fprintf_s(output_file, "UNDEF_OP %s, %s\n", "target", "val");
        break;
    }
}

char *_operation(struct AST_Node *operation)
{
    char *ret = NULL;
    switch (operation->op)
    {
    case FUNC_DEF:
        if (operation->right != NULL)
        {
            fprintf_s(output_file, "%s: \n", operation->left->identifier);
            _block(operation->right);
            fprintf_s(output_file, "RET\n");
        }
        break;
    case TERNARY_OP:
        break;
    case COMMA_OP:
        break;
    case CALL_OP:
        fprintf_s(output_file, "CALL %s\n", operation->left->identifier);
        ret = "t";
        break;
    case ASSIGN_OP:
        if (operation->right->tag == NODE_TYPE_OPERATION)
            _operation_dst(operation->left->identifier, operation->right);
        else
            fprintf_s(output_file, "MOV %s, %s\n", operation->left->identifier, _node(operation->right));

        ret = operation->left->identifier;
        break;
    case ADD_OP:
        ret = "temp";
        fprintf_s(output_file, "ADD %s, %s, %s\n", ret, _node(operation->left), _node(operation->right));
        break;
    default:
        fprintf_s(output_file, "UNDEF_OP %s, %s\n", "target", "val");
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