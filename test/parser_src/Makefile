parser: main.o parser.o instructionList.o
	mkdir -p bin
	g++ src/main.o src/parser.o src/instructionList.o -o bin/parser

main.o: src/main.cpp
	g++ --std=c++11 -c src/main.cpp -o src/main.o

parser.o: src/parser.cpp src/parser.hpp
	g++ --std=c++11 -c src/parser.cpp -o src/parser.o

instructionList.o: src/instructionList.cpp src/instructionList.hpp
	g++ --std=c++11 -c src/instructionList.cpp -o src/instructionList.o

clean:
	rm -f -rf src/*.o bin

clean_binaries:
	rm -f -rf ../test_instruction_src/*.bin
	rm -f -rf ../test_io_instruction_src_output/*.bin
	rm -f -rf ../test_io_instruction_src_input/*.bin
	rm -f -rf ../test_io_instruction_src_manual/*.bin
	rm -f -rf ../test_simulator_instruction_src/*.bin