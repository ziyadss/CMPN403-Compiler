// program

// top level declaration
int x = 2, y, z = (false ? 1 : 2);

// functions
int foo();
int foo(int, int);
int foo(int x, int);
int foo(int x, int y = 1);

int factorial(float num)
{
    return num <= 1 ? 1 : num * factorial(num - 1);
}

enum BOOLEAN
{
    TRUE = 1,
    FALSE = 0
};

enum IDK;

int main()
{
        ;;;;;;
    bool hehe = true;
    char z = 'z';
    int dec = 3, hex = 0x3A, bin = 0b11101010, oct = 077;
    float dec_f = 1e10, x = 1e-5, x = 1., x = 1.e-2, x = 3.14, x = .1, x = 0.1e-1;
    double hex_f = 0x1ffp10, hex_f = 0X0p-1, hex_f = 0x1.p0, hex_f = 0xf.p-1, hex_f = 0x0.123p-1, hex_f = 0xa.bp10;

    int x = 3 * 2;

    if (x > 0)
        x = x + 1;
    else
        x = x - 1;

    if (x > 0)
    {
        x = x + 1;
    }
    else 
    {
        x = x - 1;
    }
    // hello!

    /* hello!

    heh
    // hey 
    /* hey

    */

   // */

    float y = hello();
    z = foo(p1, p2);

    unsigned int x = 3;
    int const unsigned y = 1;
    

    switch(x)
    {
        case 0:
            x++;
        case 1:
            x--;
    }

    try
    {
        throw x;
    }
    catch (int x)
    {
        x++;
    }
    finally
    {
        print(x);
    }

    x += 1, y--;
    int a = b = 1;

    z *= true ? exp : c-- & ++x && b << 3 ^ factorial(3);

    {
        int x = 0;
    }

    if (x)
        if (y)
            x = 2;
        else
            x = 3;

    int x = 0, y;

    switch (x)
    {
    case 1:
        x = 2;
        y = 1;
        break;
    case 2:
        x = 3;
        break;
    default:
        x = 4;
        break;
    }

    while (x)
        x = 2;

    do
        x = 2;
    while (x);

    for (x = 0; x < 10; x++)
        x = 2;

    for (int x = 0; x < 10; x++)
    {
        if (x == 2)
            continue;
        x = 2;
    }

    while (x)
    {
        x = 2;
        if (x == 2)
            break;
    }

    return 0;

    try
    {
        throw x;
    }
    catch (int x)
    {
        x++;
    }
    catch
    {
        x++;
    }
    finally
    {
        print(x);
    }

    // enum_type
    enum
    {
        TRUE = 1,
        FALSE = 0
    };

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

    enum enum_type_2 enum_var_3 = ENUM_TYPE_IDK;
}