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
        fprintf(stderr, "Invalid statement: %d\n", statement->tag);
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

void _parameters_pop(struct AST_Node *parameters)
{
    if (parameters == NULL)
        return;

    while (parameters->right != NULL)
    {
        assert(parameters->right->tag == NODE_TYPE_IDENTIFIER);
        fprintf(output_file, "POP %s\n", parameters->right->identifier);
        parameters = parameters->left;
    }

    assert(parameters->tag == NODE_TYPE_IDENTIFIER);
    fprintf(output_file, "POP %s\n", parameters->identifier);
}

void _parameters_push(struct AST_Node *parameters)
{
    if (parameters == NULL)
        return;

    if (parameters->tag == NODE_TYPE_OPERATION)
    {
        _parameters_push(parameters->left);
        _parameters_push(parameters->right);
    }
    else
        fprintf(output_file, "PUSH %s\n", _node(parameters));
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
        _parameters_push(operation->right);
        fprintf(output_file, "CALL %s\n", operation->left->identifier);
        fprintf(output_file, "MOV %s, retval\n", identifier);
        break;
    case ADD_ASSIGN_OP:
        fprintf(output_file, "ADD %s, %s, %s\n", identifier, identifier, _node(operation->right));
        break;
    case AND_ASSIGN_OP:
        break;
    case ASSIGN_OP:
        fprintf(output_file, "MOV %s, %s\n", operation->left->identifier, _node(operation->right));
        break;
    case DIV_ASSIGN_OP:
        fprintf(output_file, "DIV %s, %s, %s\n", identifier, identifier, _node(operation->right));
        break;
    case MOD_ASSIGN_OP:
        fprintf(output_file, "MOD %s, %s, %s\n", identifier, identifier, _node(operation->right));
        break;
    case MUL_ASSIGN_OP:
        fprintf(output_file, "MUL %s, %s, %s\n", identifier, identifier, _node(operation->right));
        break;
    case OR_ASSIGN_OP:
        break;
    case SHL_ASSIGN_OP:
        fprintf(output_file, "SHL %s, %s, %s\n", identifier, identifier, _node(operation->right));
        break;
    case SHR_ASSIGN_OP:
        fprintf(output_file, "SHR %s, %s, %s\n", identifier, identifier, _node(operation->right));
        break;
    case SUB_ASSIGN_OP:
        fprintf(output_file, "SUB %s, %s, %s\n", identifier, identifier, _node(operation->right));
        break;
    case XOR_ASSIGN_OP:
        fprintf(output_file, "XOR %s, %s, %s\n", identifier, identifier, _node(operation->right));
        break;
    case ADD_OP:
        fprintf(output_file, "ADD %s, %s, %s\n", identifier, _node(operation->left), _node(operation->right));
        break;
    case AND_OP:
        break;
    case BIT_AND_OP:
        fprintf(output_file, "AND %s, %s, %s\n", identifier, _node(operation->left), _node(operation->right));
        break;
    case BIT_NOT_OP:
        fprintf(output_file, "XOR %s, %s, -1\n", identifier, _node(operation->left));
        break;
    case BIT_OR_OP:
        fprintf(output_file, "OR %s, %s, %s\n", identifier, _node(operation->left), _node(operation->right));
        break;
    case DEC_OP:
        fprintf(output_file, "DEC %s\n", identifier);
        break;
    case DIV_OP:
        fprintf(output_file, "DIV %s, %s, %s\n", identifier, _node(operation->left), _node(operation->right));
        break;
    case EQ_OP:
        break;
    case GE_OP:
        break;
    case GT_OP:
        break;
    case INC_OP:
        fprintf(output_file, "INC %s\n", identifier);
        break;
    case LE_OP:
        break;
    case LT_OP:
        break;
    case MOD_OP:
        fprintf(output_file, "MOD %s, %s, %s\n", identifier, _node(operation->left), _node(operation->right));
        break;
    case MUL_OP:
        fprintf(output_file, "MUL %s, %s, %s\n", identifier, _node(operation->left), _node(operation->right));
        break;
    case NE_OP:
        break;
    case NOT_OP:
        break;
    case OR_OP:
        break;
    case SHL_OP:
        fprintf(output_file, "SHL %s, %s, %s\n", identifier, _node(operation->left), _node(operation->right));
        break;
    case SHR_OP:
        fprintf(output_file, "SHR %s, %s, %s\n", identifier, _node(operation->left), _node(operation->right));
        break;
    case SUB_OP:
        fprintf(output_file, "SUB %s, %s, %s\n", identifier, _node(operation->left), _node(operation->right));
        break;
    case XOR_OP:
        fprintf(output_file, "XOR %s, %s, %s\n", identifier, _node(operation->left), _node(operation->right));
        break;
    default:
        fprintf(output_file, "UNDEF_OP %s, %s\n", "target", "val");
        break;
    }
}

char *_operation(struct AST_Node *operation)
{
    char *ret = NULL;
    switch (operation->op)
    {
    case TERNARY_OP:
        break;
    case COMMA_OP:
        break;
    case CALL_OP:
        _parameters_push(operation->right);
        fprintf(output_file, "CALL %s\n", operation->left->identifier);
        ret = "t";
        break;
    case ADD_ASSIGN_OP:
        break;
    case AND_ASSIGN_OP:
        break;
    case ASSIGN_OP:
        if (operation->right->tag == NODE_TYPE_OPERATION)
            _operation_dst(operation->left->identifier, operation->right);
        else
            fprintf(output_file, "MOV %s, %s\n", operation->left->identifier, _node(operation->right));
        ret = operation->left->identifier;
        break;
    case DIV_ASSIGN_OP:
        break;
    case MOD_ASSIGN_OP:
        break;
    case MUL_ASSIGN_OP:
        break;
    case OR_ASSIGN_OP:
        break;
    case SHL_ASSIGN_OP:
        break;
    case SHR_ASSIGN_OP:
        break;
    case SUB_ASSIGN_OP:
        break;
    case XOR_ASSIGN_OP:
        break;
    case ADD_OP:
        ret = "temp";
        fprintf(output_file, "ADD %s, %s, %s\n", ret, _node(operation->left), _node(operation->right));
        break;
    case AND_OP:
        break;
    case BIT_AND_OP:
        ret = "temp";
        fprintf(output_file, "AND %s, %s, %s\n", ret, _node(operation->left), _node(operation->right));
        break;
    case BIT_NOT_OP:
        ret = "temp";
        fprintf(output_file, "XOR %s, %s, -1\n", ret, _node(operation->left));
        break;
    case BIT_OR_OP:
        ret = "temp";
        fprintf(output_file, "OR %s, %s, %s\n", ret, _node(operation->left), _node(operation->right));
        break;
    case DEC_OP:
        ret = "temp";
        fprintf(output_file, "SUB %s, %s, 1\n", ret, _node(operation->left));
        break;
    case DIV_OP:
        ret = "temp";
        fprintf(output_file, "DIV %s, %s, %s\n", ret, _node(operation->left), _node(operation->right));
        break;
    case EQ_OP:
        break;
    case GE_OP:
        break;
    case GT_OP:
        break;
    case INC_OP:
        ret = "temp";
        fprintf(output_file, "ADD %s, %s, 1\n", ret, _node(operation->left));
        break;
    case LE_OP:
        break;
    case LT_OP:
        break;
    case MOD_OP:
        ret = "temp";
        fprintf(output_file, "MOD %s, %s, %s\n", ret, _node(operation->left), _node(operation->right));
        break;
    case MUL_OP:
        ret = "temp";
        fprintf(output_file, "MUL %s, %s, %s\n", ret, _node(operation->left), _node(operation->right));
        break;
    case NE_OP:
        break;
    case NOT_OP:
        break;
    case OR_OP:
        break;
    case SHL_OP:
        ret = "temp";
        fprintf(output_file, "SHL %s, %s, %s\n", ret, _node(operation->left), _node(operation->right));
        break;
    case SHR_OP:
        ret = "temp";
        fprintf(output_file, "SHR %s, %s, %s\n", ret, _node(operation->left), _node(operation->right));
        break;
    case SUB_OP:
        ret = "temp";
        fprintf(output_file, "SUB %s, %s, %s\n", ret, _node(operation->left), _node(operation->right));
        break;
    case XOR_OP:
        ret = "temp";
        fprintf(output_file, "XOR %s, %s, %s\n", ret, _node(operation->left), _node(operation->right));
        break;
    default:
        fprintf(output_file, "UNDEF_OP %s, %s\n", "target", "val");
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
        switch (node->tag)
        {
        case NODE_TYPE_STATEMENTS:
            _operation(node);
            break;
        case NODE_TYPE_FUNC_DEF:
            fprintf(output_file, "%s: \nPOP retadr\n", node->identifier);
            _parameters_pop(node->left);
            _block(node->right);
            fprintf(output_file, "PUSH retadr\nRET\n");
            break;
        default:
            fprintf(stderr, "Invalid top level statement: %d\n", node->tag);
            break;
        }
        fprintf(output_file, "\n");
    }

    fclose(output_file);
}