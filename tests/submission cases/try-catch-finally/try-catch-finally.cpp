void b() {
    throw 1;
}

void a() {
    b();
}

void d() {
    try
    {
        a();
    }
    catch
    {
        throw 2;
    } 
    finally
    {
        int z = 33;
    }
}

void main() {
    try
    {
        int x = 11;
        a();
    }
    catch
    {
        int y = 22;
        d();
    }
}
