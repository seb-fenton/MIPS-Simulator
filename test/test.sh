#!/bin/bash

#REMEMBER WHITESPACE#

###INITIALISATION OF TEST DIRECTORY AND FILES###
echo "--#Initialising test directory"
mkdir -p test/output
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

#1
$commandline_args test/test_src/addu1.bin

addu1bool="false"

echo $?

if [ "$?" -eq 254 ]; then
    addu1bool="true"
fi

printf "addu1 , addu , $addu1bool , $USER , Testing overflow behaviour\n" >> test/output/output.csv

#2
$commandline_args test/test_src/addu2.bin

addu2bool="false"

if [ "$?" -eq 0 ]; then
    addu2bool="true"
fi

printf "addu2 , addu , $addu2bool , $USER , Testing basic functionality\n" >> test/output/output.csv

#JR
$commandline_args test/test_src/addu1.bin

jrbool="false"

if [ "$?" -eq 0 ]; then
    jrbool="true"
  
fi

printf "jr , addu , $jrbool , $USER , Testing basic functionality\n" >> test/output/output.csv






#This command gives the return exit code of the main function, by default 0
#N.B. this is a uint therefore -10 will be represented as 246


































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