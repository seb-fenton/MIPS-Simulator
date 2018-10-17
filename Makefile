#sets simulator as default if make command has no target
.DEFAULT_GOAL := simulator
.PHONY: clean

#target simulator rules
simulator: simulator.o
	g++ simulator.o -o bin/mips_simulator

#target testbench rules
testbench: testbench.o
	g++ testbench.o -o bin/mips_testbench

#simulator object file compilation
simulator.o:
	g++ -c simulator.cpp

#testbench object file compilation
testbench.o:
	g++ -c testbench.cpp

#clean function to remove all temp files
clean: 
	rm bin/mips_simulator bin/mips_testbench testbench.o simulator.o