#pragma once
#include <stdio.h>

#include "ast.c"

void print_node(struct AST_Node *statement)
{
    switch (statement->tag)
    {
    case NODE_TYPE_IDENTIFIER:
        printf_s("%s", statement->identifier);
        break;
    case NODE_TYPE_INT:
        printf_s("%d", statement->intValue);
        break;
    case NODE_TYPE_FLOAT:
        printf_s("%f", statement->floatValue);
        break;
    case NODE_TYPE_CHAR:
        printf_s("%c", statement->charValue);
        break;
    case NODE_TYPE_STRING:
        printf_s("%s", statement->stringValue);
        break;
    case NODE_TYPE_BOOL:
        printf_s("%s", statement->boolValue ? "true" : "false");
        break;
    case NODE_TYPE_OPERATION:
        printf_s("(");
        print_node(statement->left);
        switch (statement->op)
        {
        case COMMA_OP:
            printf_s(", ");
            break;
        case CALL_OP:
            printf_s("()");
            break;
        case ADD_ASSIGN_OP:
            printf_s(" += ");
            break;
        case AND_ASSIGN_OP:
            printf_s(" &= ");
            break;
        case ASSIGN_OP:
            printf_s(" = ");
            break;
        case DIV_ASSIGN_OP:
            printf_s(" /= ");
            break;
        case MOD_ASSIGN_OP:
            printf_s(" %%= ");
            break;
        case MUL_ASSIGN_OP:
            printf_s(" *= ");
            break;
        case OR_ASSIGN_OP:
            printf_s(" |= ");
            break;
        case SHL_ASSIGN_OP:
            printf_s(" <<= ");
            break;
        case SHR_ASSIGN_OP:
            printf_s(" >>= ");
            break;
        case SUB_ASSIGN_OP:
            printf_s(" -= ");
            break;
        case XOR_ASSIGN_OP:
            printf_s(" ^= ");
            break;
        case ADD_OP:
            printf_s(" + ");
            break;
        case AND_OP:
            printf_s(" & ");
            break;
        case DIV_OP:
            printf_s(" / ");
            break;
        case EQ_OP:
            printf_s(" == ");
            break;
        case GE_OP:
            printf_s(" >= ");
            break;
        case GT_OP:
            printf_s(" > ");
            break;
        case LE_OP:
            printf_s(" <= ");
            break;
        case LT_OP:
            printf_s(" < ");
            break;
        case MOD_OP:
            printf_s(" %% ");
            break;
        case MUL_OP:
            printf_s(" * ");
            break;
        case NE_OP:
            printf_s(" != ");
            break;
        case OR_OP:
            printf_s(" | ");
            break;
        case SHL_OP:
            printf_s(" << ");
            break;
        case SHR_OP:
            printf_s(" >> ");
            break;
        case SUB_OP:
            printf_s(" - ");
            break;
        case XOR_OP:
            printf_s(" ^ ");
            break;
        default:
            fprintf_s(stderr, "Unknown operation: %d\n", statement->op);
            break;
        }
        print_node(statement->right);
        printf_s(")");
        break;
    case NODE_TYPE_STATEMENTS:
        printf_s("BLOCK START");
        for (unsigned int i = 0; i < statement->statements_count; i++)
        {
            printf_s("\nBlock Node %d:\n", i);
            print_node(statement->statements[i]);
        }
        printf_s("\nBLOCK END");
        break;
    case NODE_TYPE_FUNC_DEF:
        printf_s("FUNCTION %s START\n", statement->identifier);
        print_node(statement->left);
        print_node(statement->right);
        printf_s("\nFUNCTION %s END\n", statement->identifier);
        break;
    default:
        fprintf_s(stderr, "Unknown node type: %d\n", statement->tag);
        break;
    }
}

void print_program()
{
    printf_s("\nPROGRAM START\n");
    for (unsigned int i = 0; i < program->statements_count; i++)
    {
        struct AST_Node* statement = program->statements[i];
        switch (statement->tag)
        {
        case NODE_TYPE_OPERATION:
            printf_s("\n\nTop level statement %d, a declaration:\n", i);
            print_node(statement);
            break;
        case NODE_TYPE_STATEMENTS:
            printf_s("\n\nTop level statement %d, a function: \n", i);
            print_node(statement);
            break;
        default:
            fprintf_s(stderr, "Invalid top level statement: %d\n", statement->tag);
            break;
        }
    }
    printf_s("\nPROGRAM END\n");
}