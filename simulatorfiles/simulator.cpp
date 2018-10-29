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
    return instruction;
}

int simulator::decode(int instruction){
    int opcode = instruction >> 26;
    switch(opcode){
        //--------R Instructions--------//
        case 0b000000: return simulator::r_classification(instruction);
        //--------J Instructions--------//
        case 0b000010: return 17;   //j
        case 0b000011: return 18;   //jal
        //--------I Instructions--------//
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

        default:    std::cerr<<"Instruction decoding failed - invalid instruction. Exiting program...";
                    std::exit(-12);
                  
    }
}

int simulator::r_classification(int instruction){
    instruction = instruction & 0b111111;

    switch(instruction){
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

        default:    std::cerr<<"R_instruction decoding failed - invalid instruction. Exiting program...";
                    std::exit(-12);
    }
}

int simulator::branch_classification(int instruction){
    instruction = instruction >> 16;
    int destination = instruction & 0b11111;
    switch(destination){
        case 0b00001: return 8;  //BGEZ
        case 0b10001: return 9;  //BGEZAL
        case 0b00000: return 12;  //BLTZ
        case 0b10000: return 13;  //BLTZAL

        default:    std::cerr<<"Branch instruction decoding failed - invalid instruction. Exiting program...";
                    std::exit(-12);
    }
}

void simulator::execute(int instruction){
    switch(instruction){

        //--------R Instructions--------//
        case 1: simulator::r_add(instruction);   //ADD
        case 4: simulator::r_addu(instruction);  //ADDU

        case 5: simulator::r_and(instruction);   //AND

        case 15: simulator::r_div(instruction);   //DIV
        case 16: simulator::r_divu(instruction);   //DIVU

        case 18:    //JALR
        case 20:    //JR

        case 29:    //MFHI
        case 30:    //MFLO
        case 31:    //MTHI
        case 32:    //MTLO

        case 33:    //MULT
        case 34:    //MULTU

        case 35:    //OR

        case 39:    //SLL
        case 40:    //SLLV
        case 41:    //SLT
        case 44:    //SLTU

        case 45:    //SRA
        case 46:    //SRAV
        case 47:    //SRL
        case 48:    //SRLV

        case 49:    //SUB
        case 50:    //SUBU
        case 52:    //XOR



        //--------I Instructions--------//
        case 2: simulator::i_addi(instruction);    //addi
        case 3: simulator::i_addiu(instruction);    //addiu

        case 6: simulator::i_andi(instruction);    //andi

        case 7: return ;    //beq

        case 8:              //bgez, bgezal, bltz, bltzal
        case 9:              //bgezal
        case 12:             //bltz
        case 13:             //bltzal

        case 10: return ;   //bgtz
        case 11: return ;   //blez
        
        case 14: return ;   //bne

        case 21: return ;   //lb
        case 22: return ;   //lbu
        case 23: return ;   //lh
        case 24: return ;   //lhu
        case 25: return ;   //lui
        case 26: return ;   //lw
        case 27: return ;   //lwl
        case 28: return ;   //lwr

        case 36: return ;   //ori

        case 37: return ;   //sb
        case 38: return ;   //sh

        case 42: return ;   //slti
        case 43: return ;   //sltiu
        case 51: return ;   //sw

        case 53: return ;  //xori



        //--------J Instructions--------//
        case 17: return;;   //j
        case 19: return;   //jal
    }
}


//--------R Instructions--------//
void simulator::r_add(int instruction){     //WIP
    bool overflow = false;
    int rs = instruction & 0x3E00000;
    rs = rs >> 21;
    rs = regFile.get_reg(rs);               //src1

    int rt = instruction & 0x1F0000;
    rt = rt >> 16;
    rt = regFile.get_reg(rt);               //src2

    int rd = instruction & 0xF800;          //dest
    rd = rd >> 11;

    int result = rt+rs;

    //if both operands are same sign, set overflow if result sign is different
    if(((rs >> 31) == (rt >>31)) && (result>>31 != rs>>31)){
        overflow = true;
    }
    if(overflow){
        std::cerr<<"Exiting with error -10";
        std::exit(-10);
    }
    else{
        regFile.set_reg(result, rd);
    }
}

void simulator::r_addu(int instruction){
    int rs = instruction & 0x3E00000;
    rs = rs >> 21;
    rs = regFile.get_reg(rs);               //src1

    int rt = instruction & 0x1F0000;
    rt = rt >> 16;
    rt = regFile.get_reg(rt);               //src2

    int rd = instruction & 0xF800;          //dest
    rd = rd >> 11;

    regFile.set_reg(rt+rs, rd);             //no overflow
}

void simulator::r_and(int instruction){
    int rs = instruction & 0x3E00000;
    rs = rs >> 21;
    rs = regFile.get_reg(rs);               //src1

    int rt = instruction & 0x1F0000;
    rt = rt >> 16;
    rt = regFile.get_reg(rt);               //src2

    int rd = instruction & 0xF800;          //dest
    rd = rd >> 11;

    regFile.set_reg(rt & rs, rd);
}

void simulator::r_div(int instruction){     //WIP
    int rs = instruction & 0x3E00000;
    rs = rs >> 21;
    rs = regFile.get_reg(rs);               //src1

    int rt = instruction & 0x1F0000;
    rt = rt >> 16;
    rt = regFile.get_reg(rt);               //src2

    regFile.set_reg(rs/rt, );               //quotient into LO
    regFile.set_reg(rs%rt, );               //remainder into HI
}

void simulator::r_divu(int instruction){     //WIP
    int rs = instruction & 0x3E00000;
    rs = rs >> 21;
    rs = regFile.get_reg(rs);               //src1

    int rt = instruction & 0x1F0000;
    rt = rt >> 16;
    rt = regFile.get_reg(rt);               //src2

    regFile.set_reg(rs/rt, );               //quotient into LO
    regFile.set_reg(rs%rt, );               //remainder into HI
}

void simulator::r_or(int instruction){

    int rs = instruction>>21;
    rs = regFile.get_reg(rs);               //src1

    int rt = instruction>>16;
    rt = regFile.get_reg(rt & 0x5);         //src2

    int rd = instruction>>11;               //destination

    regFile.set_reg((rs|rt), (rd & 0x5));
}

void simulator::r_sub(int instruction){     //WIP
    bool overflow = false;
    int rs = instruction & 0x3E00000;
    rs = rs >> 21;
    rs = regFile.get_reg(rs);               //src1

    int rt = instruction & 0x1F0000;
    rt = rt >> 16;
    rt = regFile.get_reg(rt);               //src2

    int rd = instruction & 0xF800;          //dest
    rd = rd >> 11;

    int result = rs-rt;
    //detect overflow here
    int beforeMSB, afterMSB;
    beforeMSB = rs & 0x7FFFFFFF;
    afterMSB = result & 0x7FFFFFF;
    if(beforeMSB != afterMSB){
        overflow = true;
    }
    if(overflow){
        std::cerr<<"Exiting with error -10";
        std::exit(-10);
    }
    else{
        regFile.set_reg(result, rd);
    }
}

void simulator::r_subu(int instruction){    //WIP
    int rs = instruction & 0x3E00000;
    rs = rs >> 21;
    rs = regFile.get_reg(rs);               //src1

    int rt = instruction & 0x1F0000;
    rt = rt >> 16;
    rt = regFile.get_reg(rt);               //src2

    int rd = instruction & 0xF800;          //dest
    rd = rd >> 11;

    regFile.set_reg(rs-rt, rd);             //no overflow
}

void simulator::r_xor(int instruction){

    int rs = instruction>>21;
    rs = regFile.get_reg(rs);               //src1

    int rt = instruction>>16;
    rt = regFile.get_reg(rt & 0x5);         //src2

    int rd = instruction>>11;               //destination

    regFile.set_reg((rs^rt), (rd & 0x5));
}

//--------I Instructions--------//
void simulator::i_addi(int instruction){     //WIP
    bool overflow = false;
    int rs = instruction & 0x3E00000;
    rs = rs >> 21;
    rs = regFile.get_reg(rs);               //src1

    int rt = instruction & 0x1F0000;
    rt = rt >> 16;
    rt = regFile.get_reg(rt);               //src2

    int imm = instruction & 0xFFFF;          //dest

    int result = rs + imm;
    //if both operands are same sign, set overflow if result sign is different
    if(((rs >> 31) == (rt >>31)) && (result>>31 != rs>>31)){
        overflow = true;
    }
    if(overflow){
        std::cerr<<"Exiting with error -10";
        std::exit(-10);
    }
    else
        regFile.set_reg(result, rt);
}

void simulator::i_addiu(int instruction){
    int rs = instruction & 0x3E00000;
    rs = rs >> 21;
    rs = regFile.get_reg(rs);               //src1

    int rt = instruction & 0x1F0000;
    rt = rt >> 16;
    rt = regFile.get_reg(rt);               //dest

    int imm = instruction & 0xFFFF;

    regFile.set_reg(rs + imm, rt);
}

void simulator::i_andi(int instruction){    //WIP
    int rs = instruction & 0x3E00000;
    rs = rs >> 21;
    rs = regFile.get_reg(rs);               //src1

    int rt = instruction & 0x1F0000;
    rt = rt >> 16;
    rt = regFile.get_reg(rt);               //dest

    int imm = instruction & 0xFFFF;         //0 EXTENSION?

    regFile.set_reg(rs & imm, rt);
}

void simulator::i_ori(int instruction){
    int rs = instruction>>21;
    rs = rs & 0x5;
    rs = regFile.get_reg(rs);               //src1

    int rt = instruction>>16;               //destination

    int immediate = instruction & 0xF;               

    regFile.set_reg((rs|immediate), (rt & 0x5));
}

void simulator::i_xori(int instruction){

    int rs = instruction>>21;
    rs = rs & 0x5;
    rs = regFile.get_reg(rs);               //src1

    int rt = instruction>>16;               //destination

    int immediate = instruction & 0xF;               

    regFile.set_reg((rs^immediate), (rt & 0x5));
}

//--------J Instructions--------//

void simulator::j_j(int instruction){ //WIP - IS THIS CORRECT?
    int rs = instruction >> 21;
    rs = regFile.get_reg(rs);
    programCounter = rs;

    //ERROR HANDLING?
}



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