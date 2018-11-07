#include "main.hpp"
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
    //enables hexadecimal input in cin/out? WIP
    std::cin.unsetf(std::ios::dec);
    std::cin.unsetf(std::ios::hex);
    std::cin.unsetf(std::ios::oct);

    //std::cerr<<"\nRetrieving file name..."<<std::endl;              
    std::string FileName = get_filename(argc, argv);                                    //process binary file to stream into memory
    int LengthOfBinary;                                                                 //initialise integer to tell length of binary file

    //std::cerr<<"Writing binary data to character array..."<<std::endl;                  
    char* Memblock = write_binary_in(FileName, LengthOfBinary);                         //initialise array of pointers and write into it from the binary file
    bool WriteInSuccess = false;                                                        //create boolean to measure
    simulator mips_sim(LengthOfBinary, Memblock, WriteInSuccess);                       //move into instruction memory

    if(WriteInSuccess == false){
        //std::cerr<<"\nMemory write-in failed. Exiting with error code -11\n";
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
    
    //std::cerr << "\nSimulation Completed.\n";
    std::exit(exitCode);
    //return 0;
}

std::string get_filename(int argc, char* argv[]){
    std::string InputBinaryFile;
    //if argc holds more than executable path
    if(argc>1){
        //let filename string = first input in command line after executable
        InputBinaryFile = argv[1];
        return InputBinaryFile;
    }     
    else{
        //CHANGE. no instructions. exit due to end of instruction memory
        std::cerr<< "No binary file given. Exit with error code -11" << std::endl;
        std::exit(-11);
    }

}

//function to write binary data into memory; returns a boolean to check for memory exception -11
char* write_binary_in(std::string FileName, int& LengthOfBinary){

    std::ifstream InputBinary(FileName.c_str(), std::ifstream::binary);     //open the file using fstream library
    char* Memblock;

    //if a file has managed to be initialised using std::ifstream
    if(InputBinary){
        InputBinary.seekg(0, InputBinary.end);              //move the file reader pointer to the end of the binary
        LengthOfBinary = InputBinary.tellg();               //find value of pointer
        InputBinary.seekg(0, InputBinary.beg);              //move pointer back to beginnning of binary file

        Memblock = new char[LengthOfBinary];                //allocate sufficient memory for Memblock to contain full binary
        InputBinary.read(Memblock, LengthOfBinary);
        InputBinary.close();

        //return whether or not memory write was successful
        return(Memblock);
    }

    else{
        std::cerr<<"\nUnable to read file.\n";
        std::exit(-11); //-ISSUE - IS THIS CORRECT???
    }
}

//MEMORY WRITEIN TESTING
    /*int Address = 0x10000000;
    for(int i=0; i<LengthOfBinary; i++){
        bool read;
        MemModule.get_byte(Address, read);
        std::stringstream ss;
        ss << std::hex;
        ss << std::setw(2) << std::setfill('0') << (unsigned int)Memblock[i];
        std::cout<<"\nAddress "<<Address<<": "<<ss.str();
        std::cout<<"\n"<<read;
        Address = Address+0x1;
    }*/
