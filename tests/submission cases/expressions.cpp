void main()
{
  int x = 0;
  int y = 10;
  int z = x / y - x;
  bool a = true;
  bool b = false;
  bool c = a && b;
}


/**

MOV a, 0
MOV y, 10
DIV TMP, x, y
SUB z, TMP, x
MOV a, 1
MOV b, 1
CMP a, 0
JE L2
CMP b, 0
JE L2
MOV c, 1
JMP L2

L1:
MOV c, 0

L2:

**/