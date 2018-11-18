#sets simulator as default if make command has no target
.DEFAULT_GOAL := simulator
.PHONY: clean

#target simulator rules
simulator: simulatorfiles/main.o simulatorfiles/simulator.o simulatorfiles/memory.o
	mkdir -p bin
	g++ simulatorfiles/main.o simulatorfiles/simulator.o simulatorfiles/memory.o -o bin/mips_simulator

#target testbench rules
testbench: test/testbenchinitialiser.o
	g++ test/testbenchinitialiser.o -o bin/mips_testbench

#main object file compilation
main.o:
	g++ -c simulatorfiles/main.cpp

#simulator object file compilation
simulator.o:
	g++ -c simulatorfiles/simulator.cpp

#memory object file compilation
memory.o:
	g++ -c simulatorfiles/memory.cpp

#testbench object file compilation
testbenchinitialiser.o:
	g++ -c test/testbenchinitialiser.cpp

#clean function to remove all temp files that are not testbench outputs
clean: 
	rm bin/mips_simulator bin/mips_testbench simulatorfiles/main.o simulatorfiles/simulator.o simulatorfiles/memory.o test/testbenchinitialiser.o

#clean function to remove testbench outputs
#clean_t:
	#rm -r test/output
	#rm test/output/output.csv 
	