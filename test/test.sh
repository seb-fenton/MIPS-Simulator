#!/bin/bash

#REMEMBER WHITESPACE#

###INITIALISATION OF TEST DIRECTORY AND FILES###
echo "--#Initialising test directory"
mkdir -p output
touch test/output/output.csv
#touch test/temp.csv
printf "TestId , Instruction , Status , Author , Message\n" >> test/output/output.csv

###TAKES FILE INPUT###                                           
my_function() {
    echo "--#Stored filename argument:"
    for arg in "${commandline_args[@]}"; do
        echo "$arg"
    done
}

commandline_args=("$@")

my_function

###CREATES BINARY FILES AND CALLS THEM WITH SIMULATOR EXECUTABLE?, OUTPUTTING FORMAT BELOW TO CSV###

#TestId , Instruction , Status , Author [, Message]


#ADDU

$commandline_args test/test_src/addu1.bin

addu1bool=false

if [ "$?" = 254 ]; then
    "$addu1bool"=true
fi

printf "addu1 , addu , $addu1bool , $USER , Testing overflow behaviour\n" >> test/output/output.csv





make clean




#This command gives the return exit code of the main function, by default 0
#N.B. this is a uint therefore -10 will be represented as 246
echo $?

































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