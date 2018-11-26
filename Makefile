#sets simulator as default if make command has no target
.DEFAULT_GOAL := simulator
.PHONY: clean

CPPFLAGS = -c --std=c++11

#target simulator rules
simulator: simulatorfiles/main.o simulatorfiles/simulator.o simulatorfiles/memory.o
	mkdir -p bin
	g++ simulatorfiles/main.o simulatorfiles/simulator.o simulatorfiles/memory.o -o bin/mips_simulator

#target testbench rules
testbench: test/testbenchinitialiser.o
	g++ test/testbenchinitialiser.o -o bin/mips_testbench
	#compiles all binaries on each initialisation of testbench; uses olly-larkin's licensed software
	make -C test/parser_src -f ./Makefile parser 
	test/parser_src/binarycompiler.sh			
	

#main object file compilation
main.o:
	g++ $(CPPFLAGS) simulatorfiles/main.cpp

#simulator object file compilation
simulator.o:
	g++ $(CPPFLAGS) simulatorfiles/simulator.cpp

#memory object file compilation
memory.o:
	g++ $(CPPFLAGS) simulatorfiles/memory.cpp

#testbench object file compilation
testbenchinitialiser.o:
	g++ $(CPPFLAGS) test/testbenchinitialiser.cpp

#clean function to remove all temp files that are not testbench outputs
clean:
	rm bin/mips_simulator bin/mips_testbench simulatorfiles/main.o simulatorfiles/simulator.o simulatorfiles/memory.o test/testbenchinitialiser.o
	make -C test/parser_src -f ./Makefile clean
	make -C test/parser_src -f ./Makefile clean_binaries

#clean function to remove testbench outputs
#clean_t:
	#rm -r test/output
	#rm test/output/output.csv
