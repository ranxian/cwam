CC=gcc

GLIBFLAGS=`pkg-config --cflags --libs glib-2.0`

OBJS=compiler.o syntaxtree.o program_compiler.o stmt.o kv.o program.o machine.o

all: test_compiler test_program_compiler

test_compiler: test_compiler.c $(OBJS)
	$(CC) -o test_compiler test_compiler.c $(OBJS)

test_program_compiler: test_program_compiler.c $(OBJS)
	$(CC) -DDEBUG -o test_program_compiler test_program_compiler.c $(OBJS) `pkg-config --libs glib-2.0`

compiler.o: compiler.c compiler.h kv.h
	$(CC) -c compiler.c

syntaxtree.o: syntaxtree.c syntaxtree.h
	$(CC) -c syntaxtree.c

program_compiler.o: program_compiler.c program_compiler.h
	$(CC) -c program_compiler.c `pkg-config --cflags glib-2.0`

stmt.o: stmt.h stmt.c
	$(CC) -c stmt.c

kv.o: kv.h kv.c
	$(CC) -c kv.c
	
program.o: program.h program.c
	$(CC) -c program.c 

machine.o: machine.h machine.c
	$(CC) -c machine.c

clean:
	rm -rf *.~ *.o
