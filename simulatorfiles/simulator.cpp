#include "simulator.hpp"

//UNSURE HOW TO CREATE OBJECTS IN THIS CONSTRUCTOR
//regFile needs no initialisation since it has a explicitly defined default constructor. memory thus needs one.
simulator::simulator(int LengthOfBinary, char* Memblock, bool& InputSuccess) : memory(LengthOfBinary, Memblock, InputSuccess){
    programCounter = 0x10000000;
}

int simulator::fetch(sim_mem &memory, int pc){
    int instruction = 0;
    for(int i=0; i<4; i++){                             //fetch and append 4 bytes to create a full 32 byte instruction
        bool nullbool;
        int temp = memory.get_byte((pc + i), nullbool);
        temp = temp << (8*(3-i));
        instruction = instruction | temp;
    }
    //return the int
    return instruction;
}

int simulator::decode(int instruction){
    int opcode = instruction >> 26;
    switch(opcode){
        //R
        case 0b000000: return simulator::Rclassification(instruction);
        //J
        case 0b000010: return 17;   //j
        case 0b000011: return 18;   //jal
        //I
        case 0b001000: return 2;    //addi
        case 0b001001: return 3;    //addiu

        case 0b001100: return 6;    //andi

        case 0b000100: return 7;    //beq

        case 0b000001: return simulator::branch_classification(instruction);             //bgez, bgezal, bltz, bltzal
        
        case 0b000111: return 10;   //bgtz
        case 0b000110: return 11;   //blez
        
        case 0b000101: return 14;   //bne

        case 0b100000: return 21;   //lb
        case 0b100100: return 22;   //lbu
        case 0b100001: return 23;   //lh
        case 0b100101: return 24;   //lhu
        case 0b001111: return 25;   //lui
        case 0b100011: return 26;   //lw
        case 0b100010: return 27;   //lwl
        case 0b100110: return 28;   //lwr

        case 0b001101: return 36;   //ori

        case 0b101000: return 37;   //sb
        case 0b101001: return 38;   //sh

        case 0b001010: return 42;   //slti
        case 0b001011: return 43;   //sltiu
        case 0b101011: return 51;   //sw

        case 0b001110: return 53;   //xori


        


    }
}

//R INSTRUCTIONS//

int simulator::Rclassification(int instruct){
    instruct = instruct & 0b111111;

    switch(instruct){
        case 0b100000: return 1;    //ADD
        case 0b100001: return 4;    //ADDU

        case 0b100100: return 5;    //AND

        case 0b011010: return 15;   //DIV
        case 0b011011: return 16;   //DIVU

        case 0b001001: return 18;   //JALR
        case 0b001000: return 20;   //JR

        case 0b010000: return 29;   //MFHI
        case 0b010010: return 30;   //MFLO
        case 0b010001: return 31;   //MTHI
        case 0b010011: return 32;   //MTLO

        case 0b011000: return 33;   //MULT
        case 0b011001: return 34;   //MULTU

        case 0b100101: return 35;   //OR

        case 0b000000: return 39;   //SLL
        case 0b000100: return 40;   //SLLV
        case 0b101010: return 41;   //SLT
        case 0b101011: return 44;   //SLTU

        case 0b000011: return 45;   //SRA
        case 0b000111: return 46;   //SRAV
        case 0b000010: return 47;   //SRL
        case 0b000110: return 48;   //SRLV

        case 0b100010: return 49;   //SUB
        case 0b100011: return 50;   //SUBU
        case 0b100110: return 52;   //XOR
    }
}

//I INSTRUCTIONS//

int branch_classifications(int instruction){
    instruction = instruction >> 16;
    int destination = instruction & 0b11111;
    switch(destination){
        case 0b00001: return 8;  //BGEZ
        case 0b10001: return 9;  //BGEZAL
        case 0b00000: return 12;  //BLTZ
        case 0b10000: return 13;  //BLTZAL
    }
}

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