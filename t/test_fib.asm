; Fibonacci Function
; CIS 240 Fall 2011
; Varun Sampath

.CODE
.ADDR 0
USER_STACK_ADDR .UCONST x7FFF
LC R6, USER_STACK_ADDR
ADD R5, R6, #0	; set up FP
CONST R0, #8		; fib param 5
STR R0, R6, #-1	; push param onto stack
ADD R6, R6, #-1	; move SP
JSR FIB
LDR R1, R6, #0	; load return value
ADD R6, R6, #1	; remove RV
PROG_END:
NOP

; Calculates Fibonacci number
; C function declaration:
; int fib(int n);
; n >= 1
.FALIGN
FIB:
ADD R6, R6, #-3	; move SP for space for FP, RA, RV
STR R5, R6, #0	; store old FP
STR R7, R6, #1	; store return address
ADD R5, R6, #0	; move SP

LDR R4, R5, #3	; load param
CONST R0, #1	; fibn-1
CONST R1, #1	; fibn-2
CONST R2, #1	; loop counter

FIB_TEST:
CMP R2, R4		; compare loop counter
BRz FIB_END
ADD R3, R1, #0	; save fibn-1
ADD R1, R1, R0	; create new fibn
ADD R0, R3, #0	; save fibn-1 as fibn-2
ADD R2, R2, #1	; increment loop counter
JMP FIB_TEST

FIB_END:
STR R1, R5, #2	; store return value
LDR R7, R5, #1	; load return address
LDR R5, R5, #0	; load old FP
ADD R6, R6, #2	; remove FP, RA
JMPR R7					; return

.OS
.CODE
.ADDR x8200
CONST R7, #0
RTI
