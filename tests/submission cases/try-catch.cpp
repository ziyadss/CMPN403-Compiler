void c() {
    throw 1;
}

void b() {
    c();
}

void a() {
    try
    {
        b();
    }
    catch
    {
        return;
    }
}
