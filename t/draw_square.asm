;;; for (int row = 30; row < 90; ++row) {
;;; 	for (int col = 34; col < 94; ++col) {
;;;		int pixel_addr = 0xC000 + row * 128 + col;
;;;		store designated color
;;;	}
;;; }
;;; R0 = row, R1 = col, R2 = start of video address, R3 = pixel address, R4 = color
	.CODE
	.ADDR 0x0000
VIDEO_START
	.CONST 0xC000
COLOR
	.CONST 0xF
	
	CONST R0, #30		; row = 30
	LC R2, VIDEO_START	; R2 = Start of video memory
	LC R4, COLOR		; R4 = color
	
ROW_LOOP
	CMPIU R0, #90		; row - 90
	BRzp ROW_LOOP_END	; Exit outer loop; (row - 90) >= 0
	CONST R1, #34		; col = 34
COL_LOOP
	CMPIU R1, #94		; col - 94
	BRzp COL_LOOP_END 	; Exit inner loop; (col - 94) >= 0

	SLL R3, R0, 7		; pixel_addr = 128 * row
	ADD R3, R3, R1		; pixel_addr = pixel_offset + col
	ADD R3, R3, R2		; pixel_addr = pixel_offset + 0xC000
	STR R4, R3, #0		; memory[pixel_addr + 0] = color
	
	ADD R1, R1, #1		; col = col + 1
	BRnzp COL_LOOP		; Jump to beginning of column loop
COL_LOOP_END
	ADD R0, R0, #1		; row = row + 1
	BRnzp ROW_LOOP 		; Jump to beginning of row loop
ROW_LOOP_END