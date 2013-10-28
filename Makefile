CC=gcc

all: test_compiler

test_compiler: compiler.o syntaxtree.o test_compiler.c
	$(CC) -o test_compiler test_compiler.c compiler.o syntaxtree.o

compiler.o: compiler.c compiler.h
	$(CC) -c compiler.c

syntaxtree.o: syntaxtree.c syntaxtree.h
	$(CC) -c syntaxtree.c
