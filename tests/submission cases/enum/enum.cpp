enum NAMES
{
    AHMED=0,
    KHALED=1,
    MAZEN=2,
    MUHAB=3,
    ZIYAD=4
};

int main()
{
    enum NAMES n;
    n = AHMED;

    enum NAMES z = n;
    return 0;
}