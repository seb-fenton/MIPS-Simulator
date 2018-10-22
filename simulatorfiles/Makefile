.DEFAULT_GOAL := all

all: main.o memory.o 
	g++ main.o memory.o -o memtest

main.o: main.cpp main.hpp
	g++ -c main.cpp

memory.o: memory.cpp memory.hpp
	g++ -c memory.cpp

clean: 
	rm *.o memtest