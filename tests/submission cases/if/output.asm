main:
POP retadr
MOV x, 10
CMP x, 10
JNE L1
MOV b, 10
JMP L2

L1:
CMP x, 5
JNE L3
MOV b, 5
JMP L4

L3:
MOV b, x

L4:

L2:
PUSH retadr
RET

