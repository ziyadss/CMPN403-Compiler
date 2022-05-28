int fibb(int n)
{
  return n < 2 ? n : fibb(n - 1) + fibb(n - 2);
}

int main()
{
  int x = fibb(4);
  return 0;
}

/*
fibb: 
POP retadr
POP n
CMP n, 2
JGE L1
MOV temp1, n
JMP L2

L1:
SUB temp1, n, 2
PUSH temp1
CALL fibb
MOV temp2, retval
SUB temp1, n, 1
PUSH temp1
CALL fibb
MOV temp1, retval
ADD temp1, temp1, temp2
MOV temp1, temp1

L2:
MOV retval, temp1
PUSH retadr
RET

main: 
POP retadr
PUSH 4
CALL fibb
MOV x, retval
MOV retval, 0
PUSH retadr
RET
*/