.CODE
.ADDR 0
TRAP x0F0

OS_VID_MEM .UCONST xC000
VID_WIDTH .UCONST #128
VID_HEIGHT .UCONST #124
BLUE .UCONST x001F
GREEN .UCONST x03E0

.OS
.CODE
.ADDR x80F0
TRAP_DRAW_CHECKERS:
	LC R0, OS_VID_MEM
	LC R1, VID_WIDTH
	LC R2, VID_HEIGHT
	CONST R3, #0 ; maintain row count
	ROW_LOOP:
	CONST R4, #0 ; maintain column count
	COL_LOOP:
	AND R5, R3, #1 ; check if row is even
	AND R6, R4, #1 ; check if col is even
	XOR R6, R5, R6 ; check if (even even) or (odd odd)
	BRnp UPDATE_COL
	LC R5, VID_WIDTH
	MUL R5, R5, R3 ; row * 128
	CONST R6, #4
	MUL R5, R5, R6 ; row * 4 * 128
	MUL R1, R4, R6 ; R1 = (col * 4)
	ADD R5, R1, R5 ; R5 = (row*4*128) + (col*4)
	ADD R5, R0, R5 ; add xC000 in
	LC R6, BLUE
	STR R6, R5, #0 ; write 4 pixels in a row
	STR R6, R5, #1
	STR R6, R5, #2
	STR R6, R5, #3
	UPDATE_COL:
	ADD R4, R4, #1 ; increment col counter
	CONST R2, x80
	CMPU R4, R2 ; only change every 4 rows
	BRnp COL_LOOP
	ADD R3, R3, #1 ; increment row counter
	CMPI R3, #31
	BRz CHECKER_END
	JMP ROW_LOOP
CHECKER_END:
RTI
