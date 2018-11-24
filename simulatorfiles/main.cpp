#include "simulator.hpp"
#include "memory.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>

std::string get_filename(int argc, char* argv[]);
char* write_binary_in(std::string FileName, int& LengthOfBinary);

int main(int argc, char* argv[]){

    std::string FileName = get_filename(argc, argv);                                    //process binary file to stream into memory
    int LengthOfBinary;                                                                 //initialise integer to tell length of binary file

    char* Memblock = write_binary_in(FileName, LengthOfBinary);                         //initialise array of pointers and write into it from the binary file
    bool WriteInSuccess = false;                                                        //create boolean to measure
    simulator mips_sim(LengthOfBinary, Memblock, WriteInSuccess);                       //move into instruction memory

    if(WriteInSuccess == false){
        std::exit(-11); 
    }

    //BEGIN CONTROL LOOP WITH SIMULATOR OBJECT
    int instr, index, exitCode;
    while(!mips_sim.finished_sim()){
        instr = mips_sim.fetch();
        index = mips_sim.decode(instr);
        mips_sim.execute(index, instr);
        mips_sim.updatePC();
        mips_sim.update_exit_code(exitCode);
    }
    std::exit(exitCode);
}

std::string get_filename(int argc, char* argv[]){
    std::string InputBinaryFile;                                                        //if argc holds more than executable path
    if(argc>1){
        InputBinaryFile = argv[1];                                                      //let filename string = first input in command line after executable
        return InputBinaryFile;
    }     
    else{
        std::exit(-21);                                                                 //no instructions. exit due to i/o error
    }

}

char* write_binary_in(std::string FileName, int& LengthOfBinary){

    std::ifstream InputBinary(FileName.c_str(), std::ifstream::binary);     //open the file using fstream library
    char* Memblock;

    if(InputBinary){
        InputBinary.seekg(0, InputBinary.end);              //move the file reader pointer to the end of the binary
        LengthOfBinary = InputBinary.tellg();               //find value of pointer
        InputBinary.seekg(0, InputBinary.beg);              //move pointer back to beginnning of binary file

        Memblock = new char[LengthOfBinary];                //allocate sufficient memory for Memblock to contain full binary
        InputBinary.read(Memblock, LengthOfBinary);
        InputBinary.close();

        return(Memblock);                                   //array of chars containing instructions
    }

    else{
        std::cerr<<"\nUnable to read file.\n";
        std::exit(-21); 
    }
}

