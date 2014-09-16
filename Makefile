CC=clang
CFLAGS=-Wall
OBJS=compiler.o syntaxtree.o program_compiler.o stmt.o kv.o program.o \
	 machine.o query_compiler.o variable.o helper.o

all: cwam test_compiler test_program_compiler test_query_compiler

cwam: main.c $(OBJS)
	$(CC) $(CFLAGS) -o cwam main.c $(OBJS)

test_compiler: test_compiler.c $(OBJS)
	$(CC) $(CFLAGS) -o test_compiler test_compiler.c $(OBJS)

test_program_compiler: test_program_compiler.c $(OBJS)
	$(CC) $(CFLAGS) -DDEBUG -o test_program_compiler test_program_compiler.c $(OBJS)

test_query_compiler: test_query_compiler.c $(OBJS)
	$(CC) $(CFLAGS) -o test_query_compiler test_query_compiler.c $(OBJS)

compiler.o: compiler.c compiler.h kv.h
	$(CC) $(CFLAGS) -c compiler.c

syntaxtree.o: syntaxtree.c syntaxtree.h
	$(CC) $(CFLAGS) -c syntaxtree.c

program_compiler.o: program_compiler.c compiler.h
	$(CC) $(CFLAGS) -c program_compiler.c

query_compiler.o: compiler.h query_compiler.c
	$(CC) $(CFLAGS) -c query_compiler.c

stmt.o: stmt.h stmt.c
	$(CC) $(CFLAGS) -c stmt.c

kv.o: kv.h kv.c
	$(CC) $(CFLAGS) -c kv.c
	
program.o: program.h program.c
	$(CC) $(CFLAGS) -c program.c 

machine.o: machine.h machine.c
	$(CC) $(CFLAGS) -c machine.c

variable.o: machine.h variable.c
	$(CC) $(CFLAGS) -c variable.c

helper.o: helper.h helper.c
	$(CC) $(CFLAGS) -c helper.c

clean:
	rm -rf *.~ *.o
