#pragma once
#include <stdio.h>

#include "ast.c"

void print_node(struct AST_Node *statement)
{
    switch (statement->tag)
    {
    case NODE_TYPE_IDENTIFIER:
        printf("%s", statement->identifier->name);
        break;
    case NODE_TYPE_INT:
        printf("%d", statement->intValue);
        break;
    case NODE_TYPE_FLOAT:
        printf("%f", statement->floatValue);
        break;
    case NODE_TYPE_CHAR:
        printf("%c", statement->charValue);
        break;
    case NODE_TYPE_STRING:
        printf("%s", statement->stringValue);
        break;
    case NODE_TYPE_BOOL:
        printf("%s", statement->boolValue ? "true" : "false");
        break;
    case NODE_TYPE_OPERATION:
        printf("(");
        print_node(statement->left);
        switch (statement->op)
        {
        case COMMA_OP:
            printf(", ");
            break;
        case CALL_OP:
            printf("()");
            break;
        case ADD_ASSIGN_OP:
            printf(" += ");
            break;
        case AND_ASSIGN_OP:
            printf(" &= ");
            break;
        case ASSIGN_OP:
            printf(" = ");
            break;
        case DIV_ASSIGN_OP:
            printf(" /= ");
            break;
        case MOD_ASSIGN_OP:
            printf(" %%= ");
            break;
        case MUL_ASSIGN_OP:
            printf(" *= ");
            break;
        case OR_ASSIGN_OP:
            printf(" |= ");
            break;
        case SHL_ASSIGN_OP:
            printf(" <<= ");
            break;
        case SHR_ASSIGN_OP:
            printf(" >>= ");
            break;
        case SUB_ASSIGN_OP:
            printf(" -= ");
            break;
        case XOR_ASSIGN_OP:
            printf(" ^= ");
            break;
        case ADD_OP:
            printf(" + ");
            break;
        case AND_OP:
            printf(" & ");
            break;
        case DIV_OP:
            printf(" / ");
            break;
        case EQ_OP:
            printf(" == ");
            break;
        case GE_OP:
            printf(" >= ");
            break;
        case GT_OP:
            printf(" > ");
            break;
        case LE_OP:
            printf(" <= ");
            break;
        case LT_OP:
            printf(" < ");
            break;
        case MOD_OP:
            printf(" %% ");
            break;
        case MUL_OP:
            printf(" * ");
            break;
        case NE_OP:
            printf(" != ");
            break;
        case OR_OP:
            printf(" | ");
            break;
        case SHL_OP:
            printf(" << ");
            break;
        case SHR_OP:
            printf(" >> ");
            break;
        case SUB_OP:
            printf(" - ");
            break;
        case XOR_OP:
            printf(" ^ ");
            break;
        default:
            fprintf(stderr, "Unknown operation: %d\n", statement->op);
            break;
        }
        print_node(statement->right);
        printf(")");
        break;
    case NODE_TYPE_STATEMENTS:
        printf("BLOCK START");
        for (unsigned int i = 0; i < statement->statements_count; i++)
        {
            printf("\nBlock Node %d:\n", i);
            print_node(statement->statements[i]);
        }
        printf("\nBLOCK END");
        break;
    case NODE_TYPE_FUNC_DEF:
        printf("FUNCTION %s START\n", statement->identifier->name);
        print_node(statement->left);
        print_node(statement->right);
        printf("\nFUNCTION %s END\n", statement->identifier->name);
        break;
    default:
        fprintf(stderr, "Unknown node type: %d\n", statement->tag);
        break;
    }
}

void print_program()
{
    printf("\nPROGRAM START\n");
    for (unsigned int i = 0; i < program->statements_count; i++)
    {
        struct AST_Node* statement = program->statements[i];
        switch (statement->tag)
        {
        case NODE_TYPE_OPERATION:
            printf("\n\nTop level statement %d, a declaration:\n", i);
            print_node(statement);
            break;
        case NODE_TYPE_FUNC_DEF:
            printf("\n\nTop level statement %d, a function: \n", i);
            print_node(statement);
            break;
        default:
            fprintf(stderr, "Invalid top level statement: %d\n", statement->tag);
            break;
        }
    }
    printf("\nPROGRAM END\n");
}