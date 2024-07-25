build:
	rm -f ./a.out
	g++ -std=c++17 -g -Wall main.cpp building.cpp node.cpp nodes.cpp osm.cpp tinyxml2.cpp buildings.cpp -Wno-unused-variable -Wno-unused-function

run:
	./a.out

valgrind:
	rm -f ./a.out
	g++ -std=c++17 -g -Wall main.cpp building.cpp node.cpp nodes.cpp osm.cpp tinyxml2.cpp buildings.cpp -Wno-unused-variable -Wno-unused-function
	valgrind --tool=memcheck --leak-check=full ./a.out

clean:
	rm -f ./a.out
