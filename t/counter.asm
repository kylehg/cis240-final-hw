
;; Edit 10-12-10 by Paul Gurniak - removed user heap/stack setup template.  Will put it back once it is covered in class.

.CODE ;; This section contains code.
.ADDR x0000 ;; Specify address to start putting the code

;; Demo counter starts here.  Organized into two functions, a main function where PennSim starts, which indefinitely calls an increment function that adds the value of R1 (1 by default) to R0.

.FALIGN
main_start
	CONST R0, #0
	CONST R1, #1
infinite_loop
	JSR increment
	JMP infinite_loop

.FALIGN
increment
	ADD R0, R0, R1
	RET

;; End demo counter

;; Include a simple OS that just RTIs back to user code from the default PennSim entry point of x8200.

.OS
.CODE
.ADDR x8200
.FALIGN
	CONST R7, #0
	RTI

	