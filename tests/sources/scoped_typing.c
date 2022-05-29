int foo()
{
    int x = 1;
    float y;
}

int main()
{
    int a = 1, b;

    if (a)
    {
        char c = 'A';
    }
}

/*
foo:
POP retadr
MOV x, 1
PUSH retadr
RET

main:
POP retadr
MOV a, 1
CMP a, 1
JNE L1
MOV c, 'A'

L1:
PUSH retadr
RET
*/