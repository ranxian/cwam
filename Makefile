CC=gcc

OBJS=compiler.o syntaxtree.o program_compiler.o stmt.o kv.o program.o \
	 machine.o query_compiler.o variable.o helper.o

all: cwam test_compiler test_program_compiler test_query_compiler

cwam: main.c $(OBJS)
	$(CC) -o cwam main.c $(OBJS)

test_compiler: test_compiler.c $(OBJS)
	$(CC) -o test_compiler test_compiler.c $(OBJS)

test_program_compiler: test_program_compiler.c $(OBJS)
	$(CC) -DDEBUG -o test_program_compiler test_program_compiler.c $(OBJS)

test_query_compiler: test_query_compiler.c $(OBJS)
	$(CC) -o test_query_compiler test_query_compiler.c $(OBJS)

compiler.o: compiler.c compiler.h kv.h
	$(CC) -c compiler.c

syntaxtree.o: syntaxtree.c syntaxtree.h
	$(CC) -c syntaxtree.c

program_compiler.o: program_compiler.c compiler.h
	$(CC) -c program_compiler.c

query_compiler.o: compiler.h query_compiler.c
	$(CC) -c query_compiler.c

stmt.o: stmt.h stmt.c
	$(CC) -c stmt.c

kv.o: kv.h kv.c
	$(CC) -c kv.c
	
program.o: program.h program.c
	$(CC) -c program.c 

machine.o: machine.h machine.c
	$(CC) -c machine.c

variable.o: machine.h variable.c
	$(CC) -c variable.c

helper.o: helper.h helper.c
	$(CC) -c helper.c

clean:
	rm -rf *.~ *.o
