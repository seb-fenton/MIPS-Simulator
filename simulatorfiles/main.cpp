#include "main.hpp"
//#include "instructions.hpp"
#include "memory.hpp"
#include <iostream>
#include <string>
#include <map>
#include <vector>


using namespace std;
int main(int argc, char* argv[]){
    //enables hexadecimal input in cin/out?
    std::cin.unsetf(std::ios::dec);
    std::cin.unsetf(std::ios::hex);
    std::cin.unsetf(std::ios::oct);

    //INITIALISE MEMORY AND REGISTERS
    sim_reg RegFile;
    sim_mem MemModule;

    bool successfultest = true;
    cout << "Starting Memory Test.\nChecking all registers for Zeroes.\n";
    for (int i = 0 ; i<32 ; i++){
        if(RegFile.get_reg(i) != 0){
            cout << "\nRegister[" << i << "] is non-zero: "<< RegFile.get_reg(i);
            successfultest = false;
        }
            
    }

    cout << "Checking all Memory Areas for zeroes.\n";
    //addrnull
    for (int i = 0; i<4 ; i++){
        if(MemModule.TESTGETBYTE(i) != 0){
            successfultest = false;
            cout << "\nnull[" << i << "] is non zero: " << MemModule.TESTGETBYTE(i);
        }
            
    }
    //instruction space
    for (int i = 0x10000000; i<(0x11000000); i++){
        if(MemModule.TESTGETBYTE(i) != 0){
            successfultest = false;
        cout << "\ninstr[" << i << "] is nonzero: " << MemModule.TESTGETBYTE(i);
        }
    }
    //data space
    for (int i = 0x20000000; i<(0x24000000); i++){
        if(MemModule.TESTGETBYTE(i) != 0){
            successfultest = false;
        cout << "\ndata[" << i << "] is nonzero: " << MemModule.TESTGETBYTE(i);
        }
    }
    for (int i = 0x30000000; i<(0x30000004); i++){
        if(MemModule.TESTGETBYTE(i) != 0){
            successfultest = false;
        cout << "\ngetc[" << i << "] is nonzero: " << MemModule.TESTGETBYTE(i);
        }
    }
    for (int i = 0x30000004; i<(0x30000008); i++){
        if(MemModule.TESTGETBYTE(i) != 0){
            successfultest = false;
        cout << "\nputc[" << i << "] is nonzero: " << MemModule.TESTGETBYTE(i);
        }
    }

    //access checking
    for (int i=0; i<=0xFFFFFFFF; i++){
        char checker = MemModule.get_byte(i);
        //VALID AREA AND NOT DENIED

        //INVALID AREA AND DENIED
    }
    

    if(successfultest)
        cout << "CONGRATS BOI";

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

    //create vector of chars/ints holding whole binary string
    std::vector<char> InputBinaryVector;

    //using fstream or something along those lines
    //separate into instructions here? or in simulator object?







    //BEGIN CONTROL LOOP WITH SIMULATOR OBJECT
        //Obtain instruction
        //Parse instruction
        //Function Map  //std::map<std::string> function_map;
            //instruction does its thing
        //PC + 4 or branch adjustment
    }*/

    return 0;
}