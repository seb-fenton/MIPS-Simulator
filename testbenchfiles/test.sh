#!/bin/bash

#INITIALISATION OF TEST DIRECTORY AND FILES
echo "Initialising test directory"
mkdir -p bin/test
touch bin/test/testoutput.csv

#TAKES FILE INPUT                                           ISSUE!!
echo -n "Enter path to simulator executable: "
#Will not work with automated testing, need to somehow pass input?
read ans
source $ans
