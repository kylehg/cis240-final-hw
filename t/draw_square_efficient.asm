;;; for (int row = 30; row < 90; ++row) {
;;; 	for (int col = 34; col < 94; ++col) {
;;;		int pixel_addr = 0xC000 + row * 128 + col;
;;;		store designated color
;;;	}
;;; }
;;; R0 = row, R1 = pixel_addr_stop, R2 = video start, R3 = pixel address, R4 = color, R5 = #60
;;; Compared to the other implementation, this one:
;;; 1) Doesn't constantly recalculate the full video address, leading to fewer
;;;    instructions in the column (i.e. inner) loop.
;;; 2) Only has two branch instructions.
;;; 
	.CODE
	.ADDR 0x0000
VIDEO_START
	.CONST 0xC022		; column starts at 34, so include that in offset
COLOR
	.CONST 0xF
	
	LC R2, VIDEO_START	; R2 = Start of video memory
	LC R4, COLOR		; R4 = color
	CONST R0, #30		; row = 30
	CONST R5, #60		; R5 = 60
ROW_LOOP
	SLL R3, R0, 7		; pixel_addr = 128 * row
	ADD R3, R2, R3		; pixel_addr += 0xC022
	ADD R1, R3, R5		; pixel_addr_stop = pixel_addr + 60 (end address for column loop)
COL_LOOP
	STR R4, R3, #0		; memory[pixel_addr + 0] = color
	ADD R3, R3, #1		; pixel_addr += 1
	CMP R1, R3		; pixel_addr_stop - addr
	BRp COL_LOOP		; Jump to beginning of col loop if pixel_addr_stop - pixel_addr > 0
COL_LOOP_END
	ADD R0, R0, #1		; row = row + 1
	CMPIU R0, #90		; row - 90
	BRn ROW_LOOP		; Jump to beginning of row loop if row - 90 < 0	
ROW_LOOP_END