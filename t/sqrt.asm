;;; Square root program
;;; B = 0
;;; IF (A > = 0) {
;;;     WHILE (B * B <= A) {
;;;             B = B + 1
;;;     } 
;;; }
;;; B = B - 1
;;; R0 = A, R1 = B
	.CODE
	.ADDR 0x0000            ; Start instructions at 0x00

	CONST R1, #0            ; Initialize B to 0

        CMPI R0, #0             ; Compare A to 0
	BRn LOOP_END 		; Jump to end if A is negative
LOOP
        MUL R2, R1, R1          ; Compute B * B, store in C
        CMP R0, R2              ; A - C = A - B * B
        BRn LOOP_END            ; Exit if !(0 <= A - C)
        ADD R1, R1, #1          ; B = B + 1
        BRnzp LOOP              ; Jump to beginning of loop
LOOP_END
        ADD R1, R1, #-1         ; B = B - 1
END