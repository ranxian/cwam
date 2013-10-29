CC=gcc

GLIBFLAGS=`pkg-config --cflags --libs glib-2.0`

all: test_compiler test_program_compiler

test_compiler: compiler.o syntaxtree.o test_compiler.c
	$(CC) -o test_compiler test_compiler.c compiler.o syntaxtree.o

test_program_compiler: program_compiler.o test_program_compiler.c
	$(CC) -DDEBUG -o test_program_compiler test_program_compiler.c program_compiler.o compiler.o syntaxtree.o `pkg-config --libs glib-2.0`

compiler.o: compiler.c compiler.h
	$(CC) -c compiler.c

syntaxtree.o: syntaxtree.c syntaxtree.h
	$(CC) -c syntaxtree.c

program_compiler.o: program_compiler.c program_compiler.h syntaxtree.o compiler.o
	$(CC) -c program_compiler.c `pkg-config --cflags glib-2.0`

clean:
	rm -rf *.~ *.o
