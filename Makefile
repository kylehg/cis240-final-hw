all: shopper trace

shopper: shopper.c linkedlist.c linkedlist.h
	clang -o shopper shopper.c linkedlist.c

trace: trace.c memory.c memory.h
	clang -o trace trace.c memory.c

test: test.c
	clang -o test test.c
	./test
	rm test
