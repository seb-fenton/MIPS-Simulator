#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>

int main(int argc, char* argv[]){

    //initialise variable fileName to store input
    std::string fileName;

    //if argc indicates that argv contains more than just the name of the bin/mips_testbench file
    if (argc > 1) {
        //sets std::string fileName to contain the name of the bash script 
        fileName = "test/test.sh  ";
        //inserts the name of the simulator program/any other executable into the fileName string
        fileName.insert(14, argv[1]);
        //runs the fileName string in the system terminal thus causing the bash to be executed with 
        system(fileName.c_str());
    } 
    
    //if argc indicates that argv contains just bin/mips_testbench
    else {
        //ISSUE!!! 
        //Error catcher incase of illegal filename
        std::cerr<<"Error: No file input"<<std::endl;
        std::exit(0);
    }
}