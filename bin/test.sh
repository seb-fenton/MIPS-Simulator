#!/bin/bash

#INITIALISATION OF TEST DIRECTORY AND FILES
echo "Initialising test directory"
mkdir -p bin/test
touch bin/test/testoutput.csv

#ASKING FOR USER INPUT
echo -n "Enter path to simulator executable: "
read ans
source $ans
