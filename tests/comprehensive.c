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

int main()
{
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

    try
        x++;
    catch
        x--;
    finally
        x++;

    try x++;
    catch (int x) x--;
    catch x--;
    finally x++;

    try x++;
    catch (int x) x--;
}