#include "simulator.hpp"


//UNSURE HOW TO CREATE OBJECTS IN THIS CONSTRUCTOR
//regFile needs no initialisation since it has a explicitly defined default constructor. memory thus needs one.
simulator::simulator(int LengthOfBinary, char* Memblock, bool& InputSuccess) : memory(LengthOfBinary, Memblock, InputSuccess){
    programCounter = 0x10000000;
}

//MAP//
int simulator::fetch(sim_mem &memory, int pc){
    
    int instruction = 0;
    for(int i=0; i<4; i++){                                 //fetch and append 4 bytes to create a full 32 byte instruction
        int temp = memory.get_byte(pc + i);
        temp = temp << (8*(3-i));
        instruction = instruction | temp;
    }
    //return the int
    return instruction;
}

int simulator::decode(int instruction){
    char type;
    instruction = instruction >> 24;
    if(instruction == 0)
        type = 'r';
    if(instruction )
        type = 'j';
    
    //return the instruction number
}

char simulator::classification(int instruct){


}

//R INSTRUCTIONS//
int instruction::R_add(){
    return 0;
}


//I INSTRUCTIONS//


//J INSTRUCTIONS//