CC=gcc

GLIBFLAGS=`pkg-config --cflags --libs glib-2.0`

OBJS=compiler.o syntaxtree.o program_compiler.o stmt.o machine.o

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

stmt.o: stmt.h stmt.c
	$(CC) -c stmt.c

machine.o: machine.h machine.c
	$(cc) -c machine.c

clean:
	rm -rf *.~ *.o
