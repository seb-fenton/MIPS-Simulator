#include "main.hpp"
#include "instructions.hpp"
#include "memory.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>

void diagnostics(sim_reg &RegFile, sim_mem &MemModule);
void SetAccessCheck(sim_mem &memory, bool &success);
void GetAccessCheck(const sim_mem &memory, bool &success);
void CheckMemZeroes(const sim_mem &memory, bool &success);
void CheckBlankRegions(const sim_mem &memory, bool &success);

using namespace std;

int main(int argc, char* argv[]){
    //enables hexadecimal input in cin/out?
    std::cin.unsetf(std::ios::dec);
    std::cin.unsetf(std::ios::hex);
    std::cin.unsetf(std::ios::oct);

    //INITIALISE MEMORY AND REGISTERS
    sim_reg RegFile;
    sim_mem MemModule;

    diagnostics(RegFile, MemModule);

    /*LOAD BINARY INTO MEMORY
    
    //declare a string for the input binary file
    std::string InputBinaryFile;

    //if argc holds more than executable path
    if(argc>1){
        //let filename string = first input in command line after executable
        InputBinaryFile = argv[1];
        //std::cout << InputBinaryFile << std::endl;
    }     
    else{
        //ISSUE!!!
        //How do we handle having no binary input? does it just initialise and sit there on its own, or does the program exit?
        std::cerr<<"No binary file given. Ctrl + x to exit."<<std::endl;
        //std::exit(-11??); - dont exit as should be able to run independently.
    }
    
    //open the file using fstream library
    std::ifstream InputBinary(InputBinaryFile, std::ifstream::binary);
    //initialise an array of chars to hold the incoming filestream
    char* Memblock;
    //initialise an integer to determine the amount of bytes in the binary file
    int LengthOfBinary;

    //if a file has managed to be initialised using std::ifstream
    if(InputBinary){

        //move the file reader pointer to the end of the binary
        InputBinary.seekg (0, InputBinary.end);
        //find the value of the pointer i.e. find the length of the binary as pointer is at end
        LengthOfBinary = InputBinary.tellg();
        //move pointer back to beginnning of binary file
        InputBinary.seekg (0, InputBinary.beg);

        //WHAT TO DO IF BINARY DOES NOT HAVE CORRECT LENGTH? ISSUE!!!

        //allocate sufficient memory for Memblock to contain full binary
        Memblock = new char[LengthOfBinary];
        
        //read binary into memblock
        InputBinary.read(Memblock, LengthOfBinary);

        //close binary file once finished
        InputBinary.close();
    }

    else{
        std::cerr<<"\nUnable to read file.\n";
    }

    //Initialise stack counter to first value of instruction area of memory
    int Address = 0x10000000;

    //for each byte of binary
    for(int i = 0; i<LengthOfBinary; i++){
        //value to be input in set_byte command; reset to 0 each time
        char InputValue = Memblock[i];
        //bool to satisfy set_byte parameters
        bool InputSuccess;
        //call set_byte command on instruction memory
        MemModule.set_byte(Address, InputValue, InputSuccess);

        if(!InputSuccess){
            //THROW SOME ERROR? ISSUE!
        }

        //iterate up the memory stack
        Address = Address + 0x4;
    }
    */




    //BEGIN CONTROL LOOP WITH SIMULATOR OBJECT
        //Obtain instruction
        //Parse instruction
        //Function Map  //std::map<std::string> function_map;
            //instruction does its thing
        //PC + 4 or branch adjustment
    

    return 0;
}


void diagnostics(sim_reg &RegFile, sim_mem &memory){
    bool successfultest = true;
    cout << "Starting Memory Test.\nChecking all registers for Zeroes.\n";
    for (int i = 0 ; i<32 ; i++){
        if(RegFile.get_reg(i) != 0){
            cout << "\nRegister[" << i << "] is non-zero: "<< RegFile.get_reg(i);
            successfultest = false;
        }
    }
    
    memory.printmem();
    CheckMemZeroes(memory, successfultest);
    SetAccessCheck(memory, successfultest);
    GetAccessCheck(memory, successfultest);
    CheckBlankRegions(memory, successfultest);

    if(successfultest)
        cout << "\nCONGRATS BOI\n";
    else
        cout << "\nYOU DONE FUCKED UP\n";
}


void CheckMemZeroes(const sim_mem &memory, bool &success){
    bool read = false;
    for(int i=0x10000000; i<0x11000000; i++){ //instr
        if(memory.get_byte(i, read) != 0)
            success = false;
    }

    for(int i=0x20000000; i<0x24000000; i++){ //data
        if(memory.get_byte(i, read) != 0)
            success = false;
    }

    for(int i=0x30000000; i<0x30000004; i++){ //getc
        if(memory.get_byte(i, read) != 0)
            success = false;
    }

    cout << "\nAll readable memory is set to zero.";
}

void SetAccessCheck(sim_mem &memory, bool &success){
    bool written = true;
    for(int i=0x0; i<0x4; i++){ //null
        memory.set_byte(i, -1, written);
        if(written)
            success = false;
    }
    for(int i=0x10000000; i<0x11000000; i++){ //instr
        memory.set_byte(i, -1, written);
        if(written)
            success = false;
    }
    for(int i=0x20000000; i<0x24000000; i++){ //data
        memory.set_byte(i, -1, written);
        if(!written)
            success = false;
    }
    for(int i=0x30000004; i<0x30000008; i++){ //putc
        memory.set_byte(i, -1, written);
        if(!written)
            success = false;
    }
    cout << "\nWrite access is appropriate for all regions.";
}

void GetAccessCheck(const sim_mem &memory, bool &success){
    bool read = true;
    char readbyte;
    for(int i=0x0; i<0x4; i++){ //null
        readbyte = memory.get_byte(i, read);
        if(read)
            success = false;
    }

    for(int i=0x10000000; i<0x11000000; i++){ //instr
        readbyte = memory.get_byte(i, read);
        if(!read && readbyte != 0)
            success = false;
    }

    for(int i=0x20000000; i<0x24000000; i++){ //data
        readbyte = memory.get_byte(i, read);
        if(!read && readbyte != -1)
            success = false;
    }

    for(int i=0x30000004; i<0x30000008; i++){ //putc
        readbyte = memory.get_byte(i, read);
        if(read)
            success = false;
    }
    cout << "\nRead access is appropriate for all regions.";
}

void CheckBlankRegions(const sim_mem &memory, bool &success){

}
