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

L6:
MOV temp3, x
CMP temp3, 1
JNE L9
MOV x, 10
JMP L8

L9:
JMP L8

L8:
PUSH retadr
RET

