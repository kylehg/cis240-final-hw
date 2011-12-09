.CODE
.ADDR 0

TRAP x20
LEA R5, OS_DATA
STR R5, R5, #0	; should throw exception here

.OS
.CODE
.ADDR x8020
TRAP_TEST:
	LEA R3, OS_DATA
	CONST R4, #2
	STR R4, R3, #0
	LDR R2, R3, #0
	BRp TEST_LDR
	NOP ; shouldn't go here
TEST_LDR:
	RTI

BAD_TRAP:
	NOP ; shouldn't go here

.DATA
.ADDR xA000
OS_DATA:
	.FILL 0
