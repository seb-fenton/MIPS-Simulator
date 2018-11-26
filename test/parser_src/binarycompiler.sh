#!/bin/bash

FILES="test/test_instruction_src/*.txt"

for F in $FILES
do
    test/parser_src/bin/parser $F "test/test_instruction_src/#"
done

FILES="test/test_io_instruction_src/*.txt"

for F in $FILES
do
    test/parser_src/bin/parser $F "test/test_io_instruction_src/#"
done

FILES="test/test_io_instruction_src_manual/*.txt"

for F in $FILES
do
    test/parser_src/bin/parser $F "test/test_io_instruction_src_manual/#"
done

FILES="test/test_simulator_instruction_src/*.txt"

for F in $FILES
do
    test/parser_src/bin/parser $F "test/test_simulator_instruction_src/#"
done
