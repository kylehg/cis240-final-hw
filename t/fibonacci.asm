;;; F_0 = 0
;;; F_1 = 1
;;; i = 2
;;; while (i < 20) {
;;; F_i = F_(i-1) + F_(i -2)
;;; i = i + 1
;;; }
;;; R0 = i, R1 = DATA_ADDRESS + i, R2 = temp variable, R3 = temp variable

;=================================== Data ====================================;

	.DATA
	.ADDR 0x4000		; test
	.FILL 0			; F_0 = 0
	.FILL 1			; F_1 = 1

;=================================== Code ====================================;
	.CODE
	.ADDR 0x0000
MEM_START	
	.UCONST 0x4002

	CONST R0, 2 		; i = 2
	LC R1, MEM_START 	; R1 = 0x4000 + 2

LOOP
	CMPI R0, #20		; i < 20
	BRzp END		; if (i < 20) go to end

	;; Compute the next fibonacci number
	LDR R2, R1, #-1		; R2 = F_(i-1) 
	LDR R3, R1, #-2		; R3 = F_(i-2)
	ADD R2, R2, R3		; R2 = F_(i-1) + F_(i-2)
	STR R2, R1, #0		; F_i = F_(i-1) + F_(i-2)

	;; Increment the counter and data address
	ADD R0, R0, #1 		; i = i + 1
	ADD R1, R1, #1		; R1 = DATA_ADDRESS + i
	
	BRnzp LOOP		; Return to beginning of loop
END