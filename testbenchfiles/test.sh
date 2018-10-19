#!/bin/bash

###INITIALISATION OF TEST DIRECTORY AND FILES###
echo "--#Initialising test directory"
mkdir -p bin/test
touch bin/test/testoutput.csv
printf "TestId\tInstruction\tStatus\tAuthor\tMessage\n" >> bin/test/testoutput.csv

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






