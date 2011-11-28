shopper: shopper.c linkedlist.c linkedlist.h
	clang -o shopper shopper.c linkedlist.c

trace: trace.c 
	clang -o trace trace.c
