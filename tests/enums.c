enum BOOLEAN
{
    TRUE = 1,
    FALSE = 0
};

enum IDK;

enum
{
    TRUE = 1,
    FALSE = 0
};

int main()
{
    // enum_type
    enum enum_type
    {
        ENUM_TYPE_A,
        ENUM_TYPE_B,
        ENUM_TYPE_C
    } enum_var_1 = ENUM_TYPE_A;

    enum
    {
        ENUM_A,
        ENUM_B,
        ENUM_C
    } enum_var_2 = ENUM_A;

    // enum enum_type_2 enum_var_3 = ENUM_TYPE_IDK;
}