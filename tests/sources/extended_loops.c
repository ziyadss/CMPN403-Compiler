int main()
{
    int x = 1, y = 2;
    while (x)
    {
        int a = 10;
        if (y == 3)
        {
            int z = 3;
            break;
        }
        else
        {
            continue;
        }
        int b = 12;
    }
}


/*
main: 
POP retadr
MOV x, 1
MOV y, 2
L1:
CMP x, 1
JNE L2
MOV a, 10
CMP y, 3
JNE L3
MOV z, 3
JMP L2
JMP L4

L3:
JMP L1

L4:
MOV b, 12
JMP L1

L2:
PUSH retadr
RET
*/