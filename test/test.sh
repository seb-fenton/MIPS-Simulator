#!/bin/bash

#ISSUE - MAKE SURE TO REMOVE ALL UNNECESSARY ECHOES AND MAKE SURE TO PRINT TO COMMAND LINE!

###INITIALISATION OF TEST DIRECTORY AND FILES###
echo ""
echo "Initialising test directory..."
mkdir -p test/output
touch test/output/output.csv
#touch test/temp.csv
printf "TestId , Instruction , Status , Author , Message\n" >> test/output/output.csv

###TAKES FILE INPUT###                                           
commandline_args=("$@")

###CREATES BINARY FILES AND CALLS THEM WITH SIMULATOR EXECUTABLE, OUTPUTTING TO CSV###
echo ""
echo "Running tests..."
#TestId , Instruction , Status , Author [, Message]
FILES="test/test_src/*.txt"
for f in $FILES; do
    bool="fail"

    exec 5< $f                                                  #reads first 4 lines of our txt files to retrieve metadata

    read -r line <&5
    expectedOutcome="${line:1:${#line}-1}"
    read -r line <&5
    testIndex="${line:1:${#line}-1}"
    read -r line <&5
    test="${line:1:${#line}-1}"
    read -r line <&5
    message="${line:1:${#line}-1}"
    
    $commandline_args test/test_src/$testIndex.bin              #executes next executable
    output=$?
    if [ $output -eq $expectedOutcome ]; then
        bool="pass"
    fi
    printf "$testIndex , $test , $bool , $USER , $message\n" >> test/output/output.csv          

    if [ $bool = "fail" ]; then                                #prints in console whether or not particular test has faile
        echo "Test failed: $testIndex, output: $output"                          
    fi
done

echo ""
echo "Execution finished."
echo ""

































#BINARY FILE GENERATING CODE


#MIPS_CC = mips-linux-gnu-gcc
#MIPS_OBJCOPY = mips-linux-gnu-objcopy 

#MIPS_CPPFLAGS = -W -Wall -03 -fno-bullitin -march=mips1 -mfp32
#MIPS_LDFLAGS = -nostdlib -Wl,-melf32bitsnip -march=mips1 -nostartfile =mno-check-zero-division -Wl, --gpsize=0 -static -Wl, -BStatic
#MIPS_LDFLAGS += -Wl, --build-id=none

#s.mips.o: %.mips.s
#    $(MIPS_CC) $(MIPS_CPPFLAGS) -c $< -o $@

#%.mips.elf: %.mips.o
#    $(MIPS_CC) $(MIPS_CPPFLAGS) $(MIPS_LDFLAGS) -T linker.id $< -o $@

#bin/mips_simulator: src/simulator.cpp
#    mkdir -p bin
#    g++ -W Wall src/simulator.cpp -o bin/mips_simulator