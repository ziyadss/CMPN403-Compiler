#pragma once

#include <assert.h>
#include <stdlib.h>

enum OPERATION
{
    TERNARY_OP,
    COMMA_OP,
    CALL_OP,
    ADD_ASSIGN_OP,
    AND_ASSIGN_OP,
    ASSIGN_OP,
    DIV_ASSIGN_OP,
    MOD_ASSIGN_OP,
    MUL_ASSIGN_OP,
    OR_ASSIGN_OP,
    SHL_ASSIGN_OP,
    SHR_ASSIGN_OP,
    SUB_ASSIGN_OP,
    XOR_ASSIGN_OP,
    ADD_OP,
    AND_OP,
    BIT_AND_OP,
    BIT_NOT_OP,
    BIT_OR_OP,
    DEC_OP,
    DIV_OP,
    EQ_OP,
    GE_OP,
    GT_OP,
    INC_OP,
    LE_OP,
    LT_OP,
    MOD_OP,
    MUL_OP,
    NE_OP,
    NOT_OP,
    OR_OP,
    SHL_OP,
    SHR_OP,
    SUB_OP,
    XOR_OP
};

enum NODE_TYPE
{
    NODE_TYPE_FUNC_DEF,
    NODE_TYPE_STATEMENTS,
    NODE_TYPE_OPERATION,
    NODE_TYPE_IDENTIFIER,
    NODE_TYPE_INT,
    NODE_TYPE_FLOAT,
    NODE_TYPE_CHAR,
    NODE_TYPE_STRING,
    NODE_TYPE_BOOL,
};

struct AST_Node
{
    union
    {
        struct statements
        {
            unsigned int statements_count, statements_capacity;
            struct AST_Node **statements;
        };

        struct operation
        {
            enum OPERATION op;
            struct AST_Node *left;
            struct AST_Node *right;
        };

        int intValue;
        double floatValue;
        char charValue;
        char *stringValue;
        _Bool boolValue;
    };
    char *identifier;
    enum NODE_TYPE tag;
};

struct AST_Node *create_node()
{
    struct AST_Node *node = malloc(sizeof(*node));
    assert(node != NULL);
    return node;
}

struct AST_Node *identifier_node(char *identifier)
{
    struct AST_Node *node = create_node();
    node->tag = NODE_TYPE_IDENTIFIER;
    node->identifier = identifier;
    return node;
}

struct AST_Node *int_node(int value)
{
    struct AST_Node *node = create_node();
    node->tag = NODE_TYPE_INT;
    node->intValue = value;
    return node;
}

struct AST_Node *float_node(float value)
{
    struct AST_Node *node = create_node();
    node->tag = NODE_TYPE_FLOAT;
    node->floatValue = value;
    return node;
}

struct AST_Node *char_node(char value)
{
    struct AST_Node *node = create_node();
    node->tag = NODE_TYPE_CHAR;
    node->charValue = value;
    return node;
}

struct AST_Node *string_node(char *value)
{
    struct AST_Node *node = create_node();
    node->tag = NODE_TYPE_STRING;
    node->stringValue = value;
    return node;
}

struct AST_Node *bool_node(_Bool value)
{
    struct AST_Node *node = create_node();
    node->tag = NODE_TYPE_BOOL;
    node->boolValue = value;
    return node;
}

struct AST_Node *operation_node(enum OPERATION op, struct AST_Node *left, struct AST_Node *right)
{
    struct AST_Node *node = create_node();
    node->tag = NODE_TYPE_OPERATION;
    node->op = op;
    node->left = left;
    node->right = right;
    return node;
}

struct AST_Node *block_node(struct AST_Node *statement)
{
    struct AST_Node *node = create_node();
    node->tag = NODE_TYPE_STATEMENTS;
    node->statements_count = 1;
    node->statements_capacity = 2;
    node->statements = malloc(sizeof(*node->statements) * node->statements_capacity);
    assert(node->statements != NULL);
    node->statements[0] = statement;
    return node;
}

struct AST_Node *function_node(char *identifier, struct AST_Node *parameters, struct AST_Node *statements)
{
    struct AST_Node *node = create_node();
    node->tag = NODE_TYPE_FUNC_DEF;
    node->identifier = identifier;
    node->left = parameters;
    node->right = statements;
    return node;
}

struct AST_Node *add_statement(struct AST_Node *block, struct AST_Node *statement)
{
    assert(block->tag == NODE_TYPE_STATEMENTS);
    if (block->statements_count == block->statements_capacity)
    {
        block->statements_capacity *= 2;
        block->statements = realloc(block->statements, sizeof(*block->statements) * block->statements_capacity);
        assert(block->statements != NULL);
    }

    block->statements[block->statements_count++] = statement;
    return block;
}

struct AST_Node *call_node(struct AST_Node *identifier, struct AST_Node *arguments)
{
    struct AST_Node *node = create_node();
    node->tag = NODE_TYPE_OPERATION;
    node->op = CALL_OP;
    node->left = identifier;
    node->right = arguments;
    return node;
}

extern struct AST_Node *program;

void create_program()
{
    program = create_node();
    assert(program != NULL);
    program->tag = NODE_TYPE_STATEMENTS;
    program->statements_count = 0;
    program->statements_capacity = 2;
    program->statements = malloc(sizeof(*program->statements) * program->statements_capacity);
    assert(program->statements != NULL);
}

void program_append(struct AST_Node *statement)
{
    add_statement(program, statement);
}