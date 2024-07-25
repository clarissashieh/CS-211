build:
	rm -f ./a.out
	rm -f *.gcda
	rm -f *.gcno
	g++ -std=c++17 -g -Wall main.cpp tests.cpp gtest.o -I. -lm -lpthread --coverage -Wno-unused-variable -Wno-unused-function -Wno-write-strings


run:
	rm -f *.gcda
	./a.out


valgrind:
	rm -f ./a.out
	rm -f *.gcda
	rm -f *.gcno
	g++ -std=c++17 -g -Wall main.cpp tests.cpp gtest.o -I. -lm -lpthread --coverage -Wno-unused-variable -Wno-unused-function -Wno-write-strings
	valgrind --tool=memcheck --leak-check=full ./a.out


clean:
	rm -f ./a.out
	rm -f *.gcda
	rm -f *.gcno


submit:
	/home/cs211/w2024/tools/project08  submit  set.h  tests.cpp


framework:
	rm -f *.o
	gcc -g -c ./googletest/gtest-all.cc -I. -I./googletest
	ld -relocatable gtest-all.o -o gtest.o
	rm -f gtest-all.o

