#sets simulator as default if make command has no target
.DEFAULT_GOAL := simulator
.PHONY: clean

#target simulator rules
simulator: simulatorfiles/main.o simulatorfiles/instructions.o simulatorfiles/memory.o
	mkdir -p bin
	g++ simulatorfiles/main.o simulatorfiles/instructions.o simulatorfiles/memory.o -o bin/mips_simulator

#target testbench rules
testbench: bin/test.sh
	bin/test.sh

#main object file compilation
main.o:
	g++ -c simulatorfiles/main.cpp

#instructions object file compilation
instructions.o:
	g++ -c simulatorfiles/instructions.cpp

#memory object file compilation
memory.o:
	g++ -c simulatorfiles/memory.cpp

#testbench object file compilation
testbench.o:
	g++ -c testbenchfiles/testbench.cpp

#clean function to remove all temp files
clean: 
	rm bin/mips_simulator bin/mips_testbench simulatorfiles/main.o simulatorfiles/instructions.o simulatorfiles/memory.o testbenchfiles/testbench.o 