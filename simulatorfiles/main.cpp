#include "main.hpp"
//#include "instructions.hpp"
#include "memory.hpp"
#include <iostream>
#include <string>
#include <map>
#include <vector>

int main(int argc, char* argv[]){
    std::cin.unsetf(std::ios::dec);
    std::cin.unsetf(std::ios::hex);
    std::cin.unsetf(std::ios::oct);
    //INITIALISE MEMORY AND REGISTERS
    sim_mem MemModule;
    char c = 1;
    int adr;
    while (c > 0){
        std::cout << "\n\n Input a value to set:" << std::endl;
        std::cin >> c;
        std::cout << "\n Input a memory address to SET:" << std::endl;
        std::cin >> adr;
        MemModule.set_byte(adr, c);
        std::cout << "\n Input a memory address to check:";
        std::cin >> adr;
        std::cout << MemModule.get_byte(adr);
    }

    //LOAD BINARY INTO MEMORY
    /*
    //Defines string to hold the input binary
    std::string InputBinaryFile;
    bool BinaryInputTrueOrFalse;
    //std::cout << argv[0] << std::endl;

    //if argc holds more than main.cpp path
    if (argc > 1) {
        //if true, then let InputBinaryFile equal the first command line parameter
        InputBinaryFile = argv[1];
        BinaryInputTrueOrFalse = true;
        //std::cout << InputBinaryFile << std::endl;
    } else {
        //ISSUE!!! 
        //How do we handle having no binary input? does it just initialise and sit there on its own, or does the program exit?
        std::cerr<<"ERROR: -01????"<<std::endl;
        //std::exit(-11??);
    }

    std::vector<char> InputBinaryVector;



    //BEGIN CONTROL LOOP WITH SIMULATOR OBJECT
        //Obtain instruction
        //Parse instruction
        //Function Map  //std::map<std::string> function_map;
            //instruction does its thing
        //PC + 4 or branch adjustment
    }*/

    return 0;
}