#include "simulator.hpp"

//UNSURE HOW TO CREATE OBJECTS IN THIS CONSTRUCTOR
//regFile needs no initialisation since it has a explicitly defined default constructor. memory thus needs one.
simulator::simulator(int LengthOfBinary, char* Memblock, bool& InputSuccess) : memory(LengthOfBinary, Memblock, InputSuccess){
    programCounter = 0x10000000;

    pcOffSet = 0;
    branch = false;
    delayedOp = false;
}

bool simulator::finished_sim(){ //WIP, resolves if the simulator is done.

}

void simulator::updatePC(){    //WIP
    
    if(branch && delayedOp){                    //UNPREDICTABLE BEHAVIOUR
        //do something architecturally convenient
        return;
    }                     
    
    else if(branch){                                 //if it was a branch, set up a delayed branch
        programCounter = programCounter + 4;    //stage for delayed branch
        delayedOp = true;                       //forewarning for next cycle
        branch = false;                         //reset branch
        return;
    }

    else if(delayedOp){
        programCounter = programCounter + pcOffSet;
        pcOffSet = 0;
        delayedOp = false;
        return;
    }
    else
        programCounter = programCounter + 4;    //if not a branch, proceed as per normal;

}

void simulator::update_exit_code(int& exitCode){
    exitCode = (regFile.get_reg(2)) & 0xFF;
}

int simulator::fetch(){
    int instruction = 0;
    for(int i=0; i<4; i++){                             //fetch and append 4 bytes to create a full 32 byte instruction
        bool nullbool;
        int temp = memory.get_byte((programCounter + i), nullbool);
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
            break;
        case 4: simulator::r_addu(instruction);  //ADDU
            break;             
        case 5: simulator::r_and(instruction);   //AND
            break;             

        case 15: simulator::r_div(instruction);   //DIV
             break;            
        case 16: simulator::r_divu(instruction);   //DIVU
            break;             
        case 18:    //JALR
            break;            
        case 20:    //JR
            break;

        case 29: simulator::r_mfhi(instruction);   //MFHI
            break; 
        case 30: simulator::r_mflo(instruction);   //MFLO
            break; 
        case 31: simulator::r_mthi(instruction);   //MTHI
            break;
        case 32: simulator::r_mtlo(instruction);   //MTLO
            break;

        case 33:    //MULT
            break;
        case 34:    //MULTU
            break;

        case 35: simulator::r_or(instruction);   //OR
            break;

        case 39: simulator::r_sll(instruction);   //SLL
            break;
        case 40: simulator::r_sllv(instruction);   //SLLV
            break;
        case 41: //simulator::r_slt(instruction);   //SLT
            break;
        case 44: //simulator::r_sltu(instruction);   //SLTU
            break;

        case 45: simulator::r_sra(instruction);   //SRA
            break;
        case 46: simulator::r_srav(instruction);   //SRAV
            break;
        case 47: simulator::r_srl(instruction);   //SRL
            break;
        case 48: simulator::r_srlv(instruction);   //SRLV
            break;

        case 49: simulator::r_sub(instruction);   //SUB
            break;
        case 50: simulator::r_subu(instruction);  //SUBU
            break;
        case 52: simulator::r_xor(instruction);   //XOR
            break;

        //--------I Instructions--------//

        case 2: simulator::i_addi(instruction);    //addi
            break;
        case 3: simulator::i_addiu(instruction);    //addiu
            break;
        case 6: simulator::i_andi(instruction);    //andi
            break;
        case 7: ;    //beq
            break;

        case 8:              //bgez
            break;
        case 9:              //bgezal
            break;
        case 10:  ;   //bgtz
            break;
        case 11: return ;   //blez
            break;
        case 12:             //bltz
            break;
        case 13:             //bltzal
            break;

        
        
        case 14: return ;   //bne
            break;

        case 21: simulator::i_lb(instruction);   //lb
            break;
        case 22: simulator::i_lbu(instruction);   //lbu
            break;
        case 23: simulator::i_lh(instruction);;   //lh
            break;
        case 24: simulator::i_lhu(instruction);;   //lhu
            break;
        case 25: simulator::i_lui(instruction);;   //lui
            break;
        case 26: return ;   //lw
            break;
        case 27: return ;   //lwl
            break;
        case 28: return ;   //lwr
            break;

        case 36: simulator::i_ori(instruction);  //ori
            break;

        case 37: return ;   //sb
            break;
        case 38: return ;   //sh
            break;

        case 42: return ;   //slti
            break;
        case 43: return ;   //sltiu
            break;
        case 51: return ;   //sw
            break;

        case 53: return simulator::i_xori(instruction);;  //xori
            break;

        //--------J Instructions--------//
        case 17: return;   //j
            break;
        case 19: return;   //jal
            break;
    }
}


//--------R Instructions--------//
void simulator::r_add(int instruction){
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

    //OVERFLOW CHECKING

    regFile.set_lo(rs/rt);                //quotient into LO
    regFile.set_hi(rs%rt);                  //remainder into HI
}

void simulator::r_divu(int instruction){     //WIP
    int rs = instruction & 0x3E00000;
    rs = rs >> 21;
    rs = regFile.get_reg(rs);               //src1

    int rt = instruction & 0x1F0000;
    rt = rt >> 16;
    rt = regFile.get_reg(rt);               //src2

    regFile.set_lo(rs/rt);               //quotient into LO
    regFile.set_hi(rs%rt);               //remainder into HI
}

void simulator::r_mfhi(int instruction){
    int rt = instruction>>11;                     //src
    regFile.set_reg(regFile.get_hi(), rt);        //destination 
}

void simulator::r_mflo(int instruction){
    int rt = instruction>>11;                     //src
    regFile.set_reg(regFile.get_lo(), rt);        //destination 
}  

void simulator::r_mthi(int instruction){
    int rt = instruction>>21;                   //src
    regFile.set_hi(regFile.get_reg(rt));        //destination 
}                   

void simulator::r_mtlo(int instruction){
    int rt = instruction>>21;                   //src
    regFile.set_lo(regFile.get_reg(rt));        //destination 
}

void simulator::r_or(int instruction){

    int rs = instruction>>21;
    rs = regFile.get_reg(rs);               //src1

    int rt = instruction>>16;
    rt = regFile.get_reg(rt & 0x1F);         //src2

    int rd = instruction>>11;               //destination

    regFile.set_reg((rs|rt), (rd & 0x1F));
}

void simulator::r_sll(int instruction){

    int rt = instruction>>16;          
    rt = regFile.get_reg(rt);            //src1

    int sa = instruction>>6;            //shifting amount
    sa = sa & 0x1F;

    int rd = instruction>>11;           //destination

    regFile.set_reg((rt<<sa), (rd & 0x1F));

}

void simulator::r_sllv(int instruction){

    int rs = instruction>>21;           
    rs = regFile.get_reg(rs & 0x1F);     //src1

    int rt = instruction>>16;           // src2
    rt = regFile.get_reg(rt & 0x1F);

    int rd = instruction>>11;           //destination

    regFile.set_reg((rt<<rs), (rd & 0x1F));

}

void simulator::r_sra(int instruction){

    int rt = instruction>>16;          
    rt = regFile.get_reg(rt);            //src1

    bool signExtend = false;

    if(rt>>31 == 1){
        signExtend = true;
    }

    int sa = instruction>>6;            //shifting amount
    sa = sa & 0x1F;

    rt = rt>>sa;
    if(signExtend){
        int temp;
        for(int i = 0; i<sa; i++){
            temp = temp+1;
            temp = temp << 1;
        }
        temp = temp << (32-sa);
        rt = rt + temp;
    }

    int rd = instruction>>11;           //destination

    regFile.set_reg((rt), (rd & 0x1F));
}

void simulator::r_srav(int instruction){

    int rt = instruction>>16; 
    rt = rt & 0x1F;         
    rt = regFile.get_reg(rt);            //src1

    bool signExtend = false;

    if(rt>>31 == 1){
        signExtend = true;
    }

    int rs = instruction>>21;            //shifting amount
    rs = regFile.get_reg(rs);

    rt = rt>>rs;
    if(signExtend){
        int temp;
        for(int i = 0; i<rs; i++){
            temp = temp+1;
            temp = temp << 1;
        }
        temp = temp << (32-rs);
        rt = rt + temp;
    }

    int rd = instruction>>11;           //destination

    regFile.set_reg((rt), (rd & 0x1F));
}

void simulator::r_srl(int instruction){

    int rt = instruction>>16;          
    rt = regFile.get_reg(rt);            //src1

    int sa = instruction>>6;            //shifting amount
    sa = sa & 0x1F;

    int rd = instruction>>11;           //destination

    regFile.set_reg((rt>>sa), (rd & 0x1F));

}

void simulator::r_srlv(int instruction){

    int rs = instruction>>21;           
    rs = regFile.get_reg(rs & 0x1F);     //src1

    int rt = instruction>>16;           // src2
    rt = regFile.get_reg(rt & 0x1F);

    int rd = instruction>>11;           //destination

    regFile.set_reg((rt>>rs), (rd & 0x1F));

}

void simulator::r_sub(int instruction){
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
    //if both operands are same sign, set overflow if result sign is different
    if(((rs >> 31) != (rt >>31)) && (result>>31 != rs>>31)){
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

void simulator::r_subu(int instruction){
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
    rt = regFile.get_reg(rt & 0x1F);         //src2

    int rd = instruction>>11;               //destination

    regFile.set_reg((rs^rt), (rd & 0x1F));
}


//--------I Instructions--------//
void simulator::i_addi(int instruction){
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

void simulator::i_beq(int instruction){
    int rs = instruction & 0x3E00000;
    rs = rs >> 21;
    rs = regFile.get_reg(rs);               //src1

    int rt = instruction & 0x1F0000;
    rt = rt >> 16;
    rt = regFile.get_reg(rt);               //dest

    int imm = (instruction & 0xFFFF) << 2;
    if(imm >> 17 == 1)
        imm = imm | 0xFFFC0000;             //if signed, signed extension

    if(rs == rt){
        branch = true;
        pcOffSet = imm;
    }
}

void simulator::i_bgez(int instruction){
    int rs = instruction & 0x3E00000;
    rs = rs >> 21;
    rs = regFile.get_reg(rs);               //src1

    int imm = (instruction & 0xFFFF) << 2;
    if(imm >> 17 == 1)
        imm = imm | 0xFFFC0000;             //if signed, signed extension

    if(rs >= 0){
        branch = true;
        pcOffSet = imm;
    }
}

void simulator::i_bgezal(int instruction){
    int rs = instruction & 0x3E00000;
    rs = rs >> 21;
    rs = regFile.get_reg(rs);                   //src1

    int imm = (instruction & 0xFFFF) << 2;
    if(imm >> 17 == 1)
        imm = imm | 0xFFFC0000;                 //if signed, signed extension

    if(rs >= 0){
        branch = true;                          //branch
        pcOffSet = imm;
        regFile.set_reg(programCounter+8, 31);  //link I THINK
    }
}

void simulator::i_bgtz(int instruction){
    int rs = instruction & 0x3E00000;
    rs = rs >> 21;
    rs = regFile.get_reg(rs);               //src1

    int imm = (instruction & 0xFFFF) << 2;
    if(imm >> 17 == 1)
        imm = imm | 0xFFFC0000;             //if signed, signed extension

    if(rs > 0){
        branch = true;
        pcOffSet = imm;
    }
}

void simulator::i_blez(int instruction){
    int rs = instruction & 0x3E00000;
    rs = rs >> 21;
    rs = regFile.get_reg(rs);               //src1

    int imm = (instruction & 0xFFFF) << 2;
    if(imm >> 17 == 1)
        imm = imm | 0xFFFC0000;             //if signed, signed extension

    if(rs <= 0){
        branch = true;
        pcOffSet = imm;
    }
}

void simulator::i_bltz(int instruction){
    int rs = instruction & 0x3E00000;
    rs = rs >> 21;
    rs = regFile.get_reg(rs);               //src1

    int imm = (instruction & 0xFFFF) << 2;
    if(imm >> 17 == 1)
        imm = imm | 0xFFFC0000;             //if signed, signed extension

    if(rs < 0){
        branch = true;
        pcOffSet = imm;
    }
}

void simulator::i_bltzal(int instruction){
    int rs = instruction & 0x3E00000;
    rs = rs >> 21;
    rs = regFile.get_reg(rs);               //src1

    int imm = (instruction & 0xFFFF) << 2;
    if(imm >> 17 == 1)
        imm = imm | 0xFFFC0000;             //if signed, signed extension

    if(rs < 0){
        branch = true;
        pcOffSet = imm;
        regFile.set_reg(programCounter+8, 31);
    }
}

void simulator::i_bne(int instruction){
    int rs = instruction & 0x3E00000;
    rs = rs >> 21;
    rs = regFile.get_reg(rs);               //src1

    int rt = instruction & 0x1F0000;
    rt = rt >> 16;
    rt = regFile.get_reg(rt);               //dest

    int imm = (instruction & 0xFFFF) << 2;
    if(imm >> 17 == 1)
        imm = imm | 0xFFFC0000;             //if signed, signed extension

    if(rs != rt){
        branch = true;
        pcOffSet = imm;
    }
}

void simulator::i_lb(int instruction){

    signed short int offset = instruction & 0xFFFF;     //address src1

    int base = instruction>>21;             //address src2
    base = base & 0xFF;

    int memoryAddress = base + offset;

    bool nullbool;
    char byte = memory.get_byte(memoryAddress, nullbool);
    unsigned char ucbyte = memory.get_byte(memoryAddress, nullbool);

    int output;

    if(byte < 0){
        int signextend = 0xFFFFFF;
        signextend = signextend<<8;
        output = ucbyte + signextend;               //unsigned is used as ucbyte is cast to integer when added to an integer
    }
    else{
        output = byte;                              //casting works fine if byte>=0
    }

    int registerAddress = instruction>>16;
    regFile.set_reg(output, (registerAddress & 0x1F));
}

void simulator::i_lbu(int instruction){

    signed short int offset = instruction & 0xFFFF;          //address src1

    int base = instruction>>21;                              //address src2
    base = base & 0x1F;


    int memoryAddress = base + offset;
    bool nullbool;
    char byte = memory.get_byte(memoryAddress, nullbool);
    int castedByte = byte;

    int registerAddress = instruction>>16;
    regFile.set_reg(castedByte, (registerAddress & 0x1F));
}

void simulator::i_lh(int instruction){

    signed short int offset = instruction & 0xFFFF;     //address src1

    int test = offset>>15;
    if(test==1){                                        //test for memory access restriction on load halfword
        std::cerr<<"Memory offset unaligned in load halfword. Exiting with bad access error"<<std::endl;
        std::exit(-11);
    }

    int base = instruction>>21;             //address src2
    base = base & 0xFF;

    int memoryAddress = base + offset;

    bool nullbool;

    signed short int hword = memory.get_byte(memoryAddress, nullbool);
    hword = hword<<8;
    hword = hword + memory.get_byte(memoryAddress + 1, nullbool);

    unsigned short int uhword = memory.get_byte(memoryAddress, nullbool);
    uhword = uhword<<8;
    uhword = uhword + memory.get_byte(memoryAddress + 1, nullbool);

    int output;

    if(hword < 0){
        int signextend = 0xFFFF;
        signextend = signextend<<16;
        output = uhword + signextend;               //unsigned is used as ucbyte is cast to integer when added to an integer
    }
    else{
        output = hword;                              //casting works fine if byte>=0
    }

    int registerAddress = instruction>>16;
    regFile.set_reg(output, (registerAddress & 0x1F));
}

void simulator::i_lhu(int instruction){

    signed short int offset = instruction & 0xFFFF;     //address src1

    int test = offset>>15;
    if(test==1){                                        //test for memory access restriction on load halfword
        std::cerr<<"Memory offset unaligned in load halfword. Exiting with bad access error"<<std::endl;
        std::exit(-11);
    }

    int base = instruction>>21;                         //address src2
    base = base & 0xFF;

    int memoryAddress = base + offset;

    bool nullbool;
    int output = memory.get_byte(memoryAddress, nullbool);
    output = output + memory.get_byte(memoryAddress + 1, nullbool);

    int registerAddress = instruction>>16;
    regFile.set_reg(output, (registerAddress & 0x1F));
}

void simulator::i_lui(int instruction){

    int offset = instruction & 0xFFFF; 
    offset = offset << 16;

    int rt = instruction >> 16;

    regFile.set_reg(offset, (rt & 0x1F));

}

void simulator::i_ori(int instruction){
    int rs = instruction>>21;
    rs = rs & 0x1F;
    rs = regFile.get_reg(rs);               //src1

    int rt = instruction>>16;               //destination

    int immediate = instruction & 0xF;               

    regFile.set_reg((rs|immediate), (rt & 0x1F));
}

void simulator::i_sb(int instruction){
    signed short int offset = instruction & 0xFFFF;     //address src1

    int base = instruction>>21;                         //address src2
    base = base & 0xFF;

    bool nullbool;

    int rt = instruction>>16;
    rt = regFile.get_reg(rt&0x1F);
    char input = rt&0b11111111;

    memory.set_byte((base + offset), input, nullbool);
}

void simulator::i_sh(int instruction){
    signed short int offset = instruction & 0xFFFF;     //address src1

    int test = offset>>15;
    if(test==1){                                        //test for memory access restriction on load halfword
        std::cerr<<"Memory offset unaligned in load halfword. Exiting with bad access error"<<std::endl;
        std::exit(-11);
    }

    int base = instruction>>21;             //address src2
    base = base & 0xFF;

    int memoryAddress = base + offset;

    int registerAddress = instruction>>16;

    int hword = regFile.get_reg(registerAddress & 0x1F);        //get hword
    hword = hword & 0xFFFF;

    bool nullbool;

    char msb = hword>>8;                                        //msb to be loaded into memory first
    memory.set_byte((memoryAddress), msb, nullbool);

    char lsb = hword & 0xFF;                                    //lsb then loaded into memory
    memory.set_byte((memoryAddress + 1), msb, nullbool);        
}

void simulator::i_sw(int instruction){
    signed short int offset = instruction & 0xFFFF;

    int test1 = offset>>15;
    int test2 = ((offset>>14)&1);
    if((test1||test2)==1){                                        //test for memory access restriction on load word
        std::cerr<<"Memory offset unaligned in load halfword. Exiting with bad access error"<<std::endl;
        std::exit(-11);
    }

    int base = instruction>>21;                                    //address src2
    base = base & 0xFF;

    int memoryAddress = base + offset;

    int registerAddress = instruction>>16;
    int word = regFile.get_reg((registerAddress&0x1F));             //retrieve word

    bool nullbool;

    char b1 = word>>24;                                             //msb
    memory.set_byte((memoryAddress), b1, nullbool);

    char b2 = ((word>>16)&0xFF);
    memory.set_byte((memoryAddress + 1), b2, nullbool);

    char b3 = ((word>>8)&0xFF);
    memory.set_byte((memoryAddress + 2), b3, nullbool);

    char b4 = ((word&0xFF));                                        //lsb
    memory.set_byte((memoryAddress + 3), b4, nullbool);    
}

void simulator::i_xori(int instruction){

    int rs = instruction>>21;
    rs = rs & 0x1F;
    rs = regFile.get_reg(rs);               //src1

    int rt = instruction>>16;               //destination

    int immediate = instruction & 0xF;               

    regFile.set_reg((rs^immediate), (rt & 0x1F));
}

//--------J Instructions--------//
void simulator::j_j(int instruction){ //WIP - IS THIS CORRECT?
    int rs = instruction >> 21;
    rs = regFile.get_reg(rs);
    programCounter = rs;

    //ERROR HANDLING?
}

void simulator::j_jal(int instruction){

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