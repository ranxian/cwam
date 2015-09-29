# CWAM: Prolog Interpreter Based on Warren Abstract Machine in C

This is a prolog interpreter written in C by Xian Ran (xianran@pku.edu.cn)

Please compile and run this program on Unix.

## Compile

~~~
make clean && make
~~~

## Running

There are 6 files in this directory, namely `1.pr` to `6.pr`, which are used to test prolog program. Using `5.pr` as an example, the result of running it will be following, the line starts with `%` is the output of the interpreter. 
The purpose of `5.pr` is to concat two lists.

~~~
This is XR's prolog interpreter
Type help to get help
?- consult('5.pr').
YES.
?- list.
  0:	      query$: trust_me
  1:	              put_constant '5.pr' A0
  2:	              call consult
  3:	              halt
  4:	    concat~1: try_me_else concat~2 (11)
  5:	              allocate
  6:	              get_constant [] A0
  7:	              get_variable Y0 A1
  8:	              get_value Y0 A2
  9:	              deallocate
 10:	              proceed
 11:	    concat~2: trust_me
 12:	              allocate
 13:	              get_variable Y0 A0
 14:	              unify_list Y3 Y1 Y2
 15:	              unify_variable Y0 Y3
 16:	              get_variable Y4 A1
 17:	              get_variable Y5 A2
 18:	              unify_list Y7 Y1 Y6
 19:	              unify_variable Y5 Y7
 20:	              put_value Y2 A0
 21:	              put_value Y4 A1
 22:	              put_value Y6 A2
 23:	              call concat (4)
 24:	              deallocate
 25:	              proceed
?- concat(X, Y, [a, b, c]).
YES.
X = []
Y = [a, b, c]
more? [Y]es, [N]o
y
YES.
X = [a]
Y = [b, c]
more? [Y]es, [N]o
y
YES.
X = [a, b]
Y = [c]
more? [Y]es, [N]o
y
YES.
X = [a, b, c]
Y = []
more? [Y]es, [N]o
y
NO.
~~~

## Note
This is a project of Peking University's Advanced Compiler Lab course, becuase of limited time it still needs to be refined.
 There may also be unfriendly warning and error messages. Please don't distribute any source code or executable without my awareness.
