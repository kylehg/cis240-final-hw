all: shopper trace

shopper: shopper.c linkedlist.c linkedlist.h
	clang -o shopper shopper.c linkedlist.c

trace: main.c lc4.c lc4.h
	clang -o trace main.c lc4.c

test: test.c
	clang -o test test.c
	./test
	rm test
