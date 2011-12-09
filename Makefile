all: shopper trace

shopper: shopper.c linkedlist.c linkedlist.h
	clang -o shopper shopper.c linkedlist.c

trace: trace.c lc4.c lc4.h
	clang -o trace trace.c lc4.c

test: test.c
	clang -o test test.c
	./test
	rm test
