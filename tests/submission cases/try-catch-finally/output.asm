b:
POP retadr
MOV throwval, 1
RET
PUSH retadr
RET

a:
POP retadr
CALL b
MOV temp1, retval
PUSH retadr
RET

d:
POP retadr
PUSHL L1
CALL a
MOV temp1, retval
JMP L2

L1:
MOV throwval, 2
RET
JMP L2

L2:
MOV z, 33
PUSH retadr
RET

main:
POP retadr
PUSHL L3
MOV x, 11
CALL a
MOV temp1, retval
JMP L4

L3:
MOV y, 22
CALL d
MOV temp1, retval
JMP L4

L4:
PUSH retadr
RET

