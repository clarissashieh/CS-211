build:
	rm -f ./a.out
	gcc -std=c11 -g -Wall main.c execute.c scanner.c compiler.o -lm -Wno-unused-variable -Wno-unused-function

run:
	./a.out

valgrind:
	rm -f ./a.out
	gcc -std=c11 -g -Wall main.c execute.c scanner.c compiler.o -lm -Wno-unused-variable -Wno-unused-function
	valgrind --tool=memcheck --leak-check=full ./a.out

submit:
	/home/cs211/w2024/tools/project03  submit  main.c execute.c

compiler:
	rm -f *.o
	gcc -std=c11 -g -Wall -c parser.c programgraph.c ram.c tokenqueue.c util.c
	ld -relocatable parser.o programgraph.o ram.o tokenqueue.o util.o -o compiler.o
	rm -f parser.o programgraph.o ram.o tokenqueue.o util.o
