build:
	rm -f ./a.out
	gcc -std=c11 -g -Wall -lm main.c execute.c scanner.c compiler.o -Wno-unused-variable -Wno-unused-function

run:
	./a.out

valgrind:
	rm -f ./a.out
	gcc -std=c11 -g -Wall -lm main.c execute.c scanner.c compiler.o -Wno-unused-variable -Wno-unused-function
	valgrind --tool=memcheck --leak-check=no ./a.out

submit:
	/home/cs211/w2024/tools/project02  submit  main.c execute.c

compiler:
	rm -f *.o
	gcc -std=c11 -g -Wall -c parser.c programgraph.c ram.c tokenqueue.c util.c
	ld -relocatable parser.o programgraph.o ram.o tokenqueue.o util.o -o compiler.o
	rm -f parser.o programgraph.o ram.o tokenqueue.o util.o
