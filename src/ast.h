#ifndef AST_H
#define AST_H

#include <assert.h>
#include <stdlib.h>

enum OPERATION
{
    RET_OP,
    THROW_OP,
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
    NODE_TYPE_IF,
    NODE_TYPE_SWITCH,
    NODE_TYPE_WHILE,
    NODE_TYPE_DO_WHILE,
    NODE_TYPE_FOR,
    NODE_TYPE_BREAK,
    NODE_TYPE_CONTINUE,
    NODE_TYPE_TRY,
    NODE_TYPE_FUNC_DEF,
    NODE_TYPE_STATEMENTS,
    NODE_TYPE_OPERATION,
    NODE_TYPE_IDENTIFIER,
    NODE_TYPE_INT,
    NODE_TYPE_FLOAT,
    NODE_TYPE_CHAR,
    NODE_TYPE_STRING,
    NODE_TYPE_BOOL
};

struct AST_Node
{
    union
    {
        struct
        {
            struct AST_Node *condition;
            struct AST_Node *then_branch;
            union
            {
                struct AST_Node *else_branch;
                struct AST_Node *initialization;
            };
        };

        struct
        {
            unsigned int statements_count, statements_capacity;
            struct AST_Node **statements;
        };

        struct
        {
            struct AST_Node *try_block;
            struct AST_Node *catch_block;
            struct AST_Node *finally_block;
        };

        struct
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
    struct SymbolTableEntry *identifier;
    enum NODE_TYPE tag;
};

struct AST_Node *identifier_node(struct SymbolTableEntry *identifier);
struct AST_Node *int_node(int value);
struct AST_Node *float_node(float value);
struct AST_Node *char_node(char value);
struct AST_Node *string_node(char *value);
struct AST_Node *bool_node(_Bool value);
struct AST_Node *operation_node(enum OPERATION op, struct AST_Node *left, struct AST_Node *right);
struct AST_Node *block_node(struct AST_Node *statement);
struct AST_Node *function_node(struct SymbolTableEntry *identifier, struct AST_Node *parameters, struct AST_Node *statements);
struct AST_Node *call_node(struct AST_Node *identifier, struct AST_Node *arguments);
struct AST_Node *if_node(struct AST_Node *condition, struct AST_Node *then_branch, struct AST_Node *else_branch);
struct AST_Node *switch_node(struct AST_Node *expression, struct AST_Node *cases);
struct AST_Node *while_node(struct AST_Node *condition, struct AST_Node *body);
struct AST_Node *do_while_node(struct AST_Node *condition, struct AST_Node *body);
struct AST_Node *add_statement(struct AST_Node *block, struct AST_Node *statement);
struct AST_Node *for_node(struct AST_Node *initialization, struct AST_Node *condition, struct AST_Node *loop, struct AST_Node *body);
struct AST_Node *try_node(struct AST_Node *try_block, struct AST_Node *catch_block, struct AST_Node *finally_block);
struct AST_Node *break_node();
struct AST_Node *continue_node();
void program_append(struct AST_Node *statement);

#endif // AST_H