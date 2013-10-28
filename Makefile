CC=gcc

all: test_compiler

test_compiler: compiler.o test_compiler.c
	$(CC) -o test_compiler test_compiler.c compiler.o

compiler.o: compiler.c compiler.h
	$(CC) -c compiler.c
