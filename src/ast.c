#include "ast.h"

struct AST_Node *program = NULL;

struct AST_Node *create_node()
{
    struct AST_Node *node = malloc(sizeof(*node));
    assert(node != NULL);
    return node;
}

struct AST_Node *identifier_node(struct SymbolTableEntry *identifier)
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

enum TYPE get_type(struct AST_Node *node)
{
    if (node == NULL)
        return INVALID;
    switch (node->tag)
    {
    case NODE_TYPE_OPERATION:
        return get_type(node->left);
    case NODE_TYPE_IDENTIFIER:
        return node->identifier->main_type;
    case NODE_TYPE_INT:
        return INT_TYPE;
    case NODE_TYPE_FLOAT:
        return FLOAT_TYPE;
    case NODE_TYPE_CHAR:
        return INT_TYPE;
    case NODE_TYPE_STRING:
        return VOID_TYPE;
    case NODE_TYPE_BOOL:
        return INT_TYPE;
    default:
        return VOID_TYPE;
    }
}

_Bool compatible(enum TYPE type1, enum TYPE type2)
{
    return ((type1 == type2) || (type1 == INVALID) || (type2 == INVALID)) && (type1 != VOID_TYPE);
}

extern enum SEMANTIC_ERROR semantic_error;
struct AST_Node *operation_node(enum OPERATION op, struct AST_Node *left, struct AST_Node *right)
{
    enum TYPE left_type = get_type(left);
    enum TYPE right_type = get_type(right);
    if (compatible(left_type, right_type) == 0)
    {
        semantic_error = INCOMPATIBLE_TYPES;
        return NULL;
    }
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

struct AST_Node *function_node(struct SymbolTableEntry *identifier, struct AST_Node *parameters, struct AST_Node *statements)
{
    struct AST_Node *node = create_node();
    node->tag = NODE_TYPE_FUNC_DEF;
    node->identifier = identifier;
    node->left = parameters;
    node->right = statements;
    return node;
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

struct AST_Node *if_node(struct AST_Node *condition, struct AST_Node *then_branch, struct AST_Node *else_branch)
{
    struct AST_Node *node = create_node();
    node->tag = NODE_TYPE_IF;
    node->condition = condition;
    node->then_branch = then_branch;
    node->else_branch = else_branch;
    return node;
}

struct AST_Node *switch_node(struct AST_Node *expression, struct AST_Node *cases)
{
    struct AST_Node *node = create_node();
    node->tag = NODE_TYPE_SWITCH;
    node->left = expression;
    node->right = cases;
    return node;
}

struct AST_Node *while_node(struct AST_Node *condition, struct AST_Node *body)
{
    struct AST_Node *node = create_node();
    node->tag = NODE_TYPE_WHILE;
    node->condition = condition;
    node->then_branch = body;
    node->else_branch = NULL;
    return node;
}

struct AST_Node *do_while_node(struct AST_Node *condition, struct AST_Node *body)
{
    struct AST_Node *node = create_node();
    node->tag = NODE_TYPE_DO_WHILE;
    node->condition = condition;
    node->then_branch = body;
    node->else_branch = NULL;
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

struct AST_Node *for_node(struct AST_Node *initialization, struct AST_Node *condition, struct AST_Node *loop, struct AST_Node *body)
{
    struct AST_Node *node = create_node();
    node->tag = NODE_TYPE_FOR;
    node->condition = condition;
    node->then_branch = add_statement(body->tag == NODE_TYPE_STATEMENTS ? body : block_node(body), loop);
    node->initialization = initialization;
    return node;
}

struct AST_Node *break_node()
{
    struct AST_Node *node = create_node();
    node->tag = NODE_TYPE_BREAK;
    return node;
}

struct AST_Node *continue_node()
{
    struct AST_Node *node = create_node();
    node->tag = NODE_TYPE_CONTINUE;
    return node;
}

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

void destroy_ast(struct AST_Node *root)
{
    if (root == NULL)
        return;

    switch (root->tag)
    {
    case NODE_TYPE_IF:
    case NODE_TYPE_WHILE:
    case NODE_TYPE_DO_WHILE:
        destroy_ast(root->condition);
        destroy_ast(root->then_branch);
        destroy_ast(root->else_branch);
        break;
    case NODE_TYPE_FOR:
        destroy_ast(root->condition);
        destroy_ast(root->then_branch);
        destroy_ast(root->initialization);
        break;
    case NODE_TYPE_FUNC_DEF:
    case NODE_TYPE_OPERATION:
    case NODE_TYPE_SWITCH:
        destroy_ast(root->left);
        destroy_ast(root->right);
        break;
    case NODE_TYPE_STATEMENTS:
        for (unsigned int i = 0; i < root->statements_count; i++)
            destroy_ast(root->statements[i]);
        free(root->statements);
        break;
    case NODE_TYPE_STRING:
        free(root->stringValue);
        break;
    default:
        break;
    }
    free(root);
}

void destroy_program()
{
    destroy_ast(program);
}