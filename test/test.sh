#!/bin/bash

###OPTIONAL INITIALISATION OF TEST FILES IF CSV NEEDED###

mkdir -p test/output
mkdir -p test/temp
#touch test/output/output.csv
#touch test/temp.csv
#echo "TestId , Instruction , Status , Author , Message" #>> test/output/output.csv

###TAKES FILE INPUT###
commandline_args=("$@")

###START OF BASIC FUNCTION TESTS###

FILES="test/test_instruction_src/*.txt"
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

    $commandline_args test/test_instruction_src/$testIndex.bin              #executes next executable
    output=$?
    if [ $output -eq $expectedOutcome ]; then
        bool="pass"
    fi
    echo "$testIndex , $test , $bool , $USER , || Expected outcome: "$expectedOutcome" | Actual outcome: "$output" || $message ||" #>> test/output/output.csv
done

###END OF BASIC FUNCTION TESTS###

###START OF BASIC SIMULATOR FUNCTION TESTS###

FILES="test/test_simulator_instruction_src/*.txt"
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

    $commandline_args test/test_simulator_instruction_src/$testIndex.bin              #executes next executable
    output=$?
    if [ $output -eq $expectedOutcome ]; then
        bool="pass"
    fi
    echo "$testIndex , $test , $bool , $USER , || Expected outcome: "$expectedOutcome" | Actual outcome: "$output" || $message ||" #>> test/output/output.csv
done

###END OF BASIC SIMULATOR FUNCTION TESTS###

###START OF OUTPUT FUNCTION TESTS###

FILES="test/test_io_instruction_src_output/*.txt"
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

    output=$($commandline_args test/test_io_instruction_src_output/$testIndex.bin)           #executes next executable

    if [ "$output" = "$expectedOutcome" ]; then
        bool="pass"
    fi

    echo "$testIndex , $test , $bool , $USER , || Expected outcome: "$expectedOutcome" | Actual outcome: "$output" || $message ||" #>> test/output/output.csv
done

###END OF OUTPUT FUNCTION TESTS###

###START OF INPUT FUNCTION TESTS###

FILES="test/test_io_instruction_src_input/*.txt"
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
    read -r line <&5
    getcInput="${line:1:${#line}-1}"

    echo "$getcInput" | $commandline_args test/test_io_instruction_src_input/$testIndex.bin
    output=$?

    if [ "$output" = "$expectedOutcome" ]; then
        bool="pass"
    fi

    echo "$testIndex , $test , $bool , $USER , || Expected outcome: "$expectedOutcome" | Actual outcome: "$output" || $message ||" #>> test/output/output.csv
done

###END OF INPUT FUNCTION TESTS###

#START OF MANUAL TESTS#
#named as such as this is the region to put custom tests that require modified bash script

$commandline_args test/test_io_instruction_src_manual/manual1.bin
output=$?
bool="fail"

if [ "$output" -ne 245 ]; then
    bool="pass"
fi

echo "manual1 , div , $bool , $USER , || Expected outcome: !245 | Actual outcome: $output || Testing division by 0 no arithmetic exception || Dependencies: lui||" #>> test/output/output.csv


$commandline_args
output=$?
bool="fail"

if [ $output -eq 235 ]; then
    bool="pass"
fi

echo "noinput , nop , $bool , $USER , || Expected outcome: 235 | Actual outcome: $output || Testing no-input error || Dependencies: ||" #>> test/output/output.csv

cat test/test_text_files/eof.txt | $commandline_args test/test_io_instruction_src_input/lhio1.bin
output=$?
bool="fail"

if [ $output -eq 255 ]; then
    bool="pass"
fi

echo "eof1 , lh , $bool , $USER , || Expected outcome: 255 | Actual outcome: $output || Testing eof output || Dependencies: lui, jr ||" #>> test/output/output.csv

cat test/test_text_files/eof.txt | $commandline_args test/test_io_instruction_src_input/lwio1.bin
output=$?
bool="fail"

if [ $output -eq 255 ]; then
    bool="pass"
fi

echo "eof2 , lw , $bool , $USER , || Expected outcome: 255 | Actual outcome: $output || Testing eof output || Dependencies: lui, jr ||" #>> test/output/output.csv

###END OF MANUAL TESTS###
