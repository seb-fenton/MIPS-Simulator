#!/bin/bash

FILES="test/test_src/*.txt"

for F in $FILES
do
    test/parser_src/bin/parser $F "test/test_src/#"
done

FILES="test/test_io_src/*.txt"

for F in $FILES
do
    test/parser_src/bin/parser $F "test/test_io_src/#"
done

FILES="test/test_io_src_manual/*.txt"

for F in $FILES
do
    test/parser_src/bin/parser $F "test/test_io_src_manual/#"
done
