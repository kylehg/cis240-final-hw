;;;  Allocate space for some temporary variables
	.DATA
	.ADDR 0x4000
RED   .UCONST 0x7C00	; 0 11111 00000 00000
GREEN .UCONST 0x03E0	; 0 00000 11111 00000
BLUE  .UCONST 0x001F	; 0 00000 00000 11111
BLACK .UCONST 0x0000	; 0 00000 00000 00000	
	.DATA
	.ADDR 0x4200
TEMPS 	.BLKW 0x200
	name_array .STRINGZ "Ben Charrow\n"
	.DATA
	.ADDR 0x4000
STATE	.BLKW 0x8
	
;================================ Subroutines ================================;
	.CODE	
	.ADDR 0x1000
;;; Save contents of registers 0-3 to memory, uses R6
.FALIGN
SAVE_STATE
	LEA R6, STATE	
	STR R0, R6, #0
	STR R1, R6, #1
	STR R2, R6, #2
	STR R3, R6, #3
	RET
	
;;; Restore contents of registers 0-3 from memory, uses R6
.FALIGN
RESTORE_STATE
	LEA R6, STATE
	LDR R0, R6, #0
	LDR R1, R6, #1
	LDR R2, R6, #2
	LDR R3, R6, #3
	RET
	
;=================================== Main ====================================;
;;; r0 = x, r1 = y, r2 = color, r3 = temp
	.CODE
	.ADDR 0x0000
;;; Initialize variables
	CONST R0, #60		; x = 60
	CONST R1, #0 		; y = 0
	LC R2, RED		; color = RED
;;; Main
MAIN_LOOP
	;; Get input
	JSR SAVE_STATE
	TRAP 0x09		; TRAP_GET_EVENT
	JSR RESTORE_STATE

	;; Erase old block by drawing black square at current (x, y)
	ADD R3, R5, #0		; Copy R5 into R3 so that it will saved by SAVE_STATE
	JSR SAVE_STATE
	LC R2, BLACK		
	TRAP 0x0A		; TRAP_DRAW_BLOCK
	JSR RESTORE_STATE
	
	;; Parse input; switch statement.  Don't bother jumping to end of
	;; switch at each statement.  Just evaluate all comparisons
S_NAME	CMPIU R3, 0x61		; input == 'a', if yes print name
	BRnp S_RED
	JSR SAVE_STATE
	LEA R0, name_array	; r0 = start address of name
	TRAP 0x08		; TRAP_PUTS
	JSR RESTORE_STATE
S_RED	CMPIU R3, 0x72		; input == 'r', color = red
	BRnp S_GREEN
	LC R2, RED
S_GREEN	CMPIU R3, 0x67		; input == 'g', color = green
	BRnp S_BLUE
	LC R2, GREEN		
S_BLUE 	CMPIU R3, 0x62		; input == 'b', color = blue
	BRnp S_PX
	LC R2, BLUE
S_PX 	CMPIU R3, 0x6A		; input == 'j', x = x - 1
	BRnp S_MX
	ADD R0, R0, #-1
S_MX 	CMPIU R3, 0x6B		; input == 'k', x = x + 1
	BRnp S_RESET
	ADD R0, R0, #1	
S_RESET CMPIU R3, 0x71		; input == 'q', x = 60, y = 0
	BRnp S_END
	CONST R0, #60
	CONST R1, #0
S_END

	ADD R1, R1, #1		; y = y + 1
	
	;; Make sure (x, y) is in boundaries
XMIN	CMPI R0, #0		; if x < 0, set x = 0
	BRzp XMAX		
	CONST R0, #0
XMAX	CONST R3, #120		; if x > 120, set x = 120
	CMP R0, R3
	BRn YMAX		
	ADD R0, R3, #0
YMAX	CONST R3, #116		; if y > 116, set y = 116
	CMP R1, R3
	BRn DRAW
	ADD R1, R3, #0
	
	;; Draw colored square at current (x, y)
DRAW	JSR SAVE_STATE
	TRAP 0x0A		; TRAP_DRAW_BLOCK
	JSR RESTORE_STATE

	BRnzp MAIN_LOOP
END_PROGRAM
