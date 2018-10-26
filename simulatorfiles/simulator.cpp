#include "simulator.hpp"


//UNSURE HOW TO CREATE OBJECTS IN THIS CONSTRUCTOR
//regFile needs no initialisation since it has a explicitly defined default constructor. memory thus needs one.
simulator::simulator(int LengthOfBinary, char* Memblock, bool& InputSuccess) : memory(LengthOfBinary, Memblock, InputSuccess){
    programCounter = 0x10000000;
}

 //instruction fetches and appends 4 bytes to create a full 32 bit instruction
int simulator::fetch(sim_mem &memory, int pc){
    //initialise instruction word
    int instruction = 0;
    //redundant boolean to parse into get_byte structure
    bool redundant;
   
   //for integer values 0-4
    for(int i=0; i<4; i++){      
        //temporary int = byte of location+offset i                       
        int temp = memory.get_byte((pc + i), redundant);
        //left shift retrieved byte to conform to big-endianness
        temp = temp << (8*(3-i)); //(could also do temp=temp << 8 ? either way, same results)
        //bitwise or with temp to append bytes into word instruction
        instruction = instruction | temp;
    }
    //return the int instruction word
    return instruction;
}

//decode an instruction into the function type that it is - ISSUE?
int simulator::decode(int instruction){
    //declare char type to take opcode
    char type;
    //instruction now contains opcode
    instruction = instruction >> 24; //- ISSUE? DOES THIS LOSE INFORMATION?
    if(instruction == 0)
        type = 'r';
    if(instruction )
        type = 'j';
    //return the instruction number
}

char simulator::classification(int instruction){


}

//R INSTRUCTIONS//
/*int instruction::R_add(){
    return 0;
}*/


//I INSTRUCTIONS//


//J INSTRUCTIONS//



void simulator::diagnostics(){
    bool successfultest = true;
    std::cout << "Starting Memory Test.\n";
    for (int i = 0 ; i<32 ; i++){
        if(regFile.get_reg(i) != 0){
            std::cout << "\nRegister[" << i << "] is non-zero: "<< regFile.get_reg(i);
            successfultest = false;
        }
    }
    
    simulator::CheckMemZeroes(successfultest);
    simulator::SetAccessCheck(successfultest);
    simulator::GetAccessCheck(successfultest);
    simulator::CheckBlankRegions(successfultest);

    if(successfultest)
        std::cout << "\nMemory works as intended\n";
    else
        std::cout << "\nMemory function failure\n";
}

void simulator::CheckMemZeroes(bool &success){
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
}

void simulator::SetAccessCheck(bool &success){
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
}

void simulator::GetAccessCheck(bool &success){
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
}

void simulator::CheckBlankRegions(bool &success){
    bool read = true;
    char readbyte;
    
    for(int i=0x4; i<0x10000000; i++){ //null
        readbyte = memory.get_byte(i, read);
        if(read && readbyte != -1)
            success = false;
    }
    
    for(int i=0x11000000; i<0x20000000; i++){ //null
        readbyte = memory.get_byte(i, read);
        if(read && readbyte != -1)
            success = false;
    }
    
    for(int i=0x24000000; i<0x30000000; i++){ //null
        readbyte = memory.get_byte(i, read);
        if(read && readbyte != -1)
            success = false;
    }
    
    for(int i=0x30000008; i<=0xFFFFFFFF; i++){ //null
        readbyte = memory.get_byte(i, read);
        if(read && readbyte != -1)
            success = false;
    }
}