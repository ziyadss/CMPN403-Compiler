sum:
POP retadr
POP y
POP x
ADD retval, x, y
PUSH retadr
RET

main:
POP retadr
PUSH 10
PUSH 20
CALL sum
MOV a, retval
PUSH a
PUSH a
CALL sum
MOV a, retval
PUSH retadr
RET

