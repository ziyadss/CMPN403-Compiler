// struct AST_Node
// {

// };

// #define STRINGIFY(x) #x

enum NODE_TYPE
{
    MOV_OP, FUNC_DECL_START, FUNC_DECL_END, FUNC_CALL, INC_OP, DEC_OP, ADD_OP, SUB_OP, NOT_OP, BIT_NOT_OP
};

char *node(enum NODE_TYPE op, char *dst, char *src1, char *src2)
{
    switch (op)
    {
    case MOV_OP:
        printf_s("MOV, %s, %s\n", dst, src1);
        return dst;
        break;

    case FUNC_DECL_START:
        printf_s("%s:\n", dst);
        break;

    case FUNC_DECL_END:
        printf_s("RET\n\n");
        break;

    case FUNC_CALL:
        printf_s("CALL %s\n", dst);
        break;

    default:
        printf_s("Error: Unknown operation %d\n", op);
        exit(1);
        break;
    }

    return NULL;
}