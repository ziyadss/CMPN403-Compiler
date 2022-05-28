void main()
{

  for (int i = 0; i < 10; i++)
  {
    int b = 10;
  }

  int x = 0;

  while (x < 20)
  {
    x++;
  }

  x = 0;

  do
  {
    x++;
  } while (x < 20);

  switch (x)
  {
  case 1:
    x = 10; break;

  default:
    break;
  }
}

/*
main: 
POP retadr
MOV i, 0
L1:
CMP i, 10
JGE L2
MOV b, 10
INC i
JMP L1

L2:
MOV x, 0
L3:
CMP x, 20
JGE L4
INC x
JMP L3

L4:
MOV x, 0
L5:
INC x
CMP x, 20
JL L5

L6
MOV temp3, x
CMP temp3, 1
JNE L8
MOV x, 10
JMP L7

L8:
JMP L7

L7:
PUSH retadr
RET
*/