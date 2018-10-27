.DEFAULT_GOAL := all

all: main.o memory.o  simulator.o
	g++ main.o memory.o simulator.o -o memtest

main.o: main.cpp main.hpp
	g++ -c main.cpp

memory.o: memory.cpp memory.hpp
	g++ -c memory.cpp

simulator.o: simulator.cpp simulator.hpp
	g++ -c simulator.cpp

clean: 
	rm *.o memtest