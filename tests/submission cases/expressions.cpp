void main()
{
  int x = 0;
  int y = 10;
  int z = x / y - x;
  bool a = true;
  bool b = false;
  bool c = a && b;
}


/*
main: 
POP retadr
MOV x, 0
MOV y, 10
DIV temp1, x, y
SUB z, temp1, x
MOV a, 1
MOV b, 0
CMP a, 0
JE L1
CMP b, 0
JE L1
MOV c, 1
JMP L2

L1:
MOV c, 0

L2:
PUSH retadr
RET
*/