#include "simulator.hpp"

simulator::simulator(int LengthOfBinary, char* Memblock, bool& InputSuccess) : memory(LengthOfBinary, Memblock, InputSuccess){
    programCounter = 0x10000000;
    pcOffSet = 0;
    jump = false;
    branch = false;
    delayedBranch = false;
    delayedJump = false;
}

bool simulator::finished_sim(){ //WIP, resolves if the simulator is done.
    //condition 1: pc has jumped to zero
    if(programCounter == 0) return true;
    else    return false;
}

void simulator::updatePC(){    //WIP if branch in delayed slot, just follows latest branch.
    programCounter = programCounter + 4;

    if(branch || jump){                             
        if(branch)      delayedBranch = true;       //stage delayed branch/jump for next cycle
        else if(jump)   delayedJump = true;         //redundant elif in case something is BOTH a branch and jump(impossible)

        branch = false;
        jump = false;    
        return;
    }

    else if(delayedBranch || delayedJump){
        if(delayedBranch)   programCounter = programCounter + pcOffSet - 4;
        else if(delayedJump)    programCounter = pcOffSet;

        delayedBranch = false;
        delayedJump = false;
        pcOffSet = 0;
    }
}

void simulator::update_exit_code(int& exitCode){
    exitCode = (regFile.get_reg(2)) & 0xFF;
}

int simulator::fetch(){
    int check = programCounter;               //CHECK that PC is in an executable area
    check = memory.addressmap(check);
    int instruction = 0;
    if(check > 1)
        std::exit(-11);                             //11: executing ADDRESS that cannot be executed. different from 12

    else{
        for(int i=0; i<4; i++){                     //fetch and append 4 bytes to create a full 32 byte instruction
            int temp = memory.get_byte((programCounter + i));
            temp = temp & 0xFF;
            temp = temp << (8*(3-i));
            instruction = instruction | temp;
        }
        return instruction;
    }
    
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
                    break;
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

void simulator::execute(int index, int instruction){
    switch(index){
        //--------R Instructions--------//
        case 1: simulator::r_add(instruction);
            break;
        case 4: simulator::r_addu(instruction);
            break;             
        case 5: simulator::r_and(instruction);
            break;             
        case 15: simulator::r_div(instruction);
             break;            
        case 16: simulator::r_divu(instruction);
            break;             
        case 18: simulator::r_jalr(instruction);
            break;            
        case 20: simulator::r_jr(instruction);
            break;
        case 29: simulator::r_mfhi(instruction);
            break; 
        case 30: simulator::r_mflo(instruction);
            break; 
        case 31: simulator::r_mthi(instruction);
            break;
        case 32: simulator::r_mtlo(instruction);
            break;
        case 33: simulator::r_mult(instruction);
            break;
        case 34: simulator::r_multu(instruction);
            break;
        case 35: simulator::r_or(instruction);
            break;
        case 39: simulator::r_sll(instruction);
            break;
        case 40: simulator::r_sllv(instruction);
            break;
        case 41: simulator::r_slt(instruction);
            break;
        case 44: simulator::r_sltu(instruction);
            break;
        case 45: simulator::r_sra(instruction);
            break;
        case 46: simulator::r_srav(instruction);
            break;
        case 47: simulator::r_srl(instruction);
            break;
        case 48: simulator::r_srlv(instruction);
            break;
        case 49: simulator::r_sub(instruction);
            break;
        case 50: simulator::r_subu(instruction);
            break;
        case 52: simulator::r_xor(instruction);
            break;

        //--------I Instructions--------//
        case 2: simulator::i_addi(instruction);
            break;
        case 3: simulator::i_addiu(instruction);
            break;
        case 6: simulator::i_andi(instruction);
            break;
        case 7: simulator::i_beq(instruction);
            break;
        case 8: simulator::i_bgez(instruction);
            break;
        case 9: simulator::i_bgezal(instruction);
            break;
        case 10: simulator::i_bgtz(instruction);
            break;
        case 11: simulator::i_blez(instruction);
            break;
        case 12: simulator::i_bltz(instruction);
            break;
        case 13: simulator::i_bltzal(instruction);
            break;
        case 14: simulator::i_bne(instruction);
            break;
        case 21: simulator::i_lb(instruction);
            break;
        case 22: simulator::i_lbu(instruction);
            break;
        case 23: simulator::i_lh(instruction);
            break;
        case 24: simulator::i_lhu(instruction);
            break;
        case 25: simulator::i_lui(instruction);
            break;
        case 26: simulator::i_lw(instruction);
            break;
        case 27: simulator::i_lwl(instruction);
            break;
        case 28: simulator::i_lwr(instruction);
            break;
        case 36: simulator::i_ori(instruction);
            break;
        case 37: simulator::i_sb(instruction);
            break;
        case 38: simulator::i_sh(instruction);
            break;
        case 42: simulator::i_slti(instruction);
            break;
        case 43: simulator::i_sltiu(instruction);
            break;
        case 51: simulator::i_sw(instruction);
            break;
        case 53: simulator::i_xori(instruction);
            break;

        //--------J Instructions--------//
        case 17: simulator::j_j(instruction);
            break;
        case 19: simulator::j_jal(instruction);
            break;
    }
}


//--------R Instructions--------//
    void simulator::r_add(int instruction){
        bool overflow = false;
        int rs = (instruction & 0x3E00000) >> 21;
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
            //std::cerr<<"Exiting with error -10";
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
    void simulator::r_div(int instruction){     //WIP THIS CAN'T BE IT
        int rs = instruction & 0x3E00000;
        rs = rs >> 21;
        rs = regFile.get_reg(rs);               //src1

        int rt = instruction & 0x1F0000;
        rt = rt >> 16;
        rt = regFile.get_reg(rt);               //src2
        if(rt == 0)
            std::exit(-10);                     //arithmetic overflow if divide by 0

        regFile.set_lo(rs/rt);                  //quotient into LO
        regFile.set_hi(rs%rt);                  //remainder into HI
    }
    void simulator::r_divu(int instruction){     //WIP
        uint rs = instruction & 0x3E00000;
        rs = rs >> 21;
        rs = regFile.get_reg(rs);               //src1

        uint rt = instruction & 0x1F0000;
        rt = rt >> 16;
        rt = regFile.get_reg(rt);               //src2
        if(rt == 0)
            std::exit(-10);                     //arithmetic overflow if divide by 0

        uint quotient = rs/rt;
        uint remainder = rs%rt;
        regFile.set_lo(quotient);               //quotient into LO
        regFile.set_hi(remainder);              //remainder into HI
    }
    void simulator::r_jalr(int instruction){    //WIP
        simulator::r_jr(instruction);
        int rd = (instruction & 0x0000F800) >> 11;

        regFile.set_reg(programCounter+8, rd);
    }
    void simulator::r_jr(int instruction){      //WIP
        int rs = (instruction >> 21) & 0x1F;  //fetch register number
        rs = regFile.get_reg(rs);                   //fetch register value

        jump = true;                                //set jump (for delayed branching)
        pcOffSet = rs;                              //set regval into pcOffSet
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
    void simulator::r_mult(int instruction){    //MULT WIP

        bool overflow = false;
        int rs = (instruction & 0x3E00000) >> 21;
        int64_t multplc = regFile.get_reg(rs);               //src1    

        int rt = instruction & 0x1F0000;
        rt = rt >> 16;
        int64_t multplr = regFile.get_reg(rt);               //src2

        int64_t result = rt*rs;
        int tophalf = result >> 32;
        int btmhalf = result & 0xffffffff;
        //compare both operand signs
        if((multplr >> 31) == (multplc >> 31))
            if(tophalf >> 31 != multplr >> 31)
                overflow = true;

        //if different, expect inverted 
        if((multplr >> 31) != (multplc >> 31))
            if(tophalf > 0)
                overflow = true;

        if(overflow){
            //std::cerr<<"Exiting with error -10";
            std::exit(-10);
        }
        else{
            regFile.set_hi(tophalf);
            regFile.set_lo(btmhalf);
        }
    }
    void simulator::r_multu(int instruction){   //MULT WIP
        int rs = (instruction & 0x3E00000) >> 21;
        int64_t multplc = regFile.get_reg(rs);               //src1    

        int rt = instruction & 0x1F0000;
        rt = rt >> 16;
        int64_t multplr = regFile.get_reg(rt);               //src2

        int64_t result = rt*rs;
        int tophalf = result >> 32;
        int btmhalf = result & 0xffffffff;

        regFile.set_hi(tophalf);
        regFile.set_lo(btmhalf);
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
    void simulator::r_slt(int instruction){

        int rs = ((instruction>>21) & 0x1F);
        rs = regFile.get_reg(rs);

        int rt = ((instruction>>16) & 0x1F);
        rt = regFile.get_reg(rs);

        int comparison;

        if(rs<rt){comparison = 1;}
        else{comparison = 0;}

        int rd = ((instruction>>11) & 0x1F);
        regFile.set_reg(comparison, rd);
    }
    void simulator::r_sltu(int instruction){

        int rs = ((instruction>>21) & 0x1F);
        unsigned int rsu = regFile.get_reg(rs);

        int rt = ((instruction>>16) & 0x1F);
        unsigned int rtu = regFile.get_reg(rs);

        int comparison;

        if(rsu<rtu){comparison = 1;}
        else{comparison = 0;}

        int rd = ((instruction>>11) & 0x1F);
        regFile.set_reg(comparison, rd);
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
            //std::cerr<<"Exiting with error -10";
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
    void simulator::i_parse(int instruction, int& rs, int& rt, short signed int& imm){
        rs = (instruction >> 21) & 0x1F;
        rt = (instruction >> 16) & 0x1F;
        imm = instruction & 0xFFFF;
    }

    int simulator::sign_extend(short signed int imm){
        int output;
        if(imm<0){
            output = 0xFFFF0000 | imm;
        }
        else{
            output = imm;
        }
        return output;
    }

    void simulator::i_addi(int instruction){
        bool overflow = false;
        int rs, rt;
        short signed int imm;
        i_parse(instruction, rs, rt, imm);

        rs = regFile.get_reg(rs);               //value of rs

        int result = rs + imm;
        //if both operands are same sign, set overflow if result sign is different
        if(((rs >> 31) == (imm >> 15)) && (result>>31 != rs>>31)){
            overflow = true;
        }
        if(overflow){
            //std::cerr<<"Exiting with error -10";
            std::exit(-10);
        }
        else
            regFile.set_reg(result, rt);
    }
    void simulator::i_addiu(int instruction){
        int rs, rt;
        short signed int imm;
        i_parse(instruction, rs, rt, imm);
        rs = regFile.get_reg(rs);               //value

        int signExtImm = sign_extend(imm);
        int result = rs + signExtImm;

        regFile.set_reg(result, rt);
    }
    void simulator::i_andi(int instruction){
        int rs, rt;
        short signed int dummy;
        int imm = instruction & 0xFFFF;         //0 extended
        i_parse(instruction, rs, rt, dummy);
        
        rs = regFile.get_reg(rs);               //src1

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
    
    
    /***GETC WIP***/
        void simulator::i_lb(int instruction){
            signed short int offset = instruction & 0xFFFF;     //offset

            int base = instruction>>21;                         //base
            base = base & 0xFF;
            base = regFile.get_reg(base);

            int memoryAddress = base + offset;                  //effectiveaddr

            int rt = (instruction>>16) & 0x1F;

            char byte = memory.get_byte(memoryAddress);
            unsigned char ucbyte = memory.get_byte(memoryAddress);

            int output;
            if(byte < 0)    output = ucbyte | 0xFFFFFF00;               //unsigned is used as ucbyte is cast to integer when added to an integer
            else            output = byte;                              //casting works fine if byte>=0
                         
            regFile.set_reg(output, rt);
        }

        void simulator::i_lbu(int instruction){
            signed short int offset = instruction & 0xFFFF;          //immediate
            int base = instruction>>21;                              //base
            base = base & 0x1F;
            base = regFile.get_reg(base);

            int memoryAddress = base + offset;                      //effectiveaddr

            int rt = (instruction>>16) & 0x1F;                      //destination rt

            
            char byte = memory.get_byte(memoryAddress);
            int castedByte = byte;

            regFile.set_reg(castedByte, rt);
        }

        void simulator::i_lh(int instruction){
            signed short int offset = instruction & 0xFFFF;     //immediate

            int test = offset % 2;
            if(test != 0){                                      //test for memory access restriction on load halfword
                std::cerr<<"Memory offset unaligned in load halfword. Exiting with bad access error" << std::endl;
                std::exit(-11);
            }

            int base = (instruction>>21) & 0xFF;                //base address
            base = regFile.get_reg(base);

            int memoryAddress = base + offset;

            int rt = (instruction>>16) & 0x1F;                      //destination rt

            signed short int hword = memory.get_byte(memoryAddress);
            hword = hword<<8;
            hword = hword + memory.get_byte(memoryAddress + 1);

            unsigned short int uhword = memory.get_byte(memoryAddress);
            uhword = uhword<<8;
            uhword = uhword + memory.get_byte(memoryAddress + 1);

            int output;

            if(hword < 0)   output = uhword | 0xFFFF0000;               //unsigned is used as ucbyte is cast to integer when added to an integer
            else            output = hword;                              //casting works fine if byte>=0
            
            regFile.set_reg(output, rt);
        }
        void simulator::i_lhu(int instruction){
            signed short int offset = instruction & 0xFFFF;     //address src1

            int test = offset>>15;
            if(test==1){                                        //test for memory access restriction on load halfword
                std::cerr<<"Memory offset unaligned in load halfword. Exiting with bad access error"<<std::endl;
                std::exit(-11);
            }

            int base = (instruction>>21) & 0xFF;                //base address
            base = regFile.get_reg(base);

            int memoryAddress = base + offset;

            int output = memory.get_byte(memoryAddress);
            output = output + memory.get_byte(memoryAddress + 1);

            int registerAddress = instruction>>16;
            regFile.set_reg(output, (registerAddress & 0x1F));
        }
        void simulator::i_lui(int instruction){

            int offset = instruction & 0xFFFF; 
            offset = offset << 16;

            int rt = (instruction >> 16) & 0x1F;

            regFile.set_reg(offset, rt);

        }
        void simulator::i_lw(int instruction){
            signed short int offset = instruction & 0xFFFF;
            int base = (instruction>>21) & 0x1F;                                    //address src2
            base = base ;

            int test = offset % 4;
            if(test != 0){                                        //test for memory access restriction on load word
                std::cerr<<"Memory offset unaligned in load word. Exiting with bad access error"<<std::endl;
                std::exit(-11);
            }

            

            int memoryAddress = base + offset;

            int input = memory.get_byte(memoryAddress);
            input = input<<8;

            int temp = memory.get_byte(memoryAddress + 1);
            input = input + temp;
            input = input<<8;
            
            temp = memory.get_byte(memoryAddress + 2);
            input = input + temp;
            input = input<<8;

            temp = memory.get_byte(memoryAddress + 3);
            input = input + temp;

            int rt = instruction>>16;
            regFile.set_reg(input, rt&0x1F);
            
        }
        void simulator::i_lwl(int instruction){ //NOT DONE WIP
            signed short int offset = instruction & 0xFFFF;

            int base = (instruction >> 21) & 0x1F;
            base = regFile.get_reg(base);

            int address = base + offset;

            int rt = (instruction >> 16) & 0x1F;

            int b1 = memory.get_byte(address);
            b1 = b1 << 24;
            int b2 = memory.get_byte(address + 1);
            b2 = b2 << 16;

            int input = b1 | b2;

            regFile.lwl_set_reg(rt, input);
        }
        void simulator::i_lwr(int instruction){ //NOT DONE WIP
            signed short int offset = instruction & 0xFFFF;

            int base = (instruction >> 21) & 0x1F;
            base = regFile.get_reg(base);

            int address = base + offset;

            int rt = (instruction >> 16) & 0x1F;

            int b1 = memory.get_byte(address);
            int b2 = memory.get_byte(address - 1);
            b2 = b2 << 8;

            int input = b2 | b1;

            regFile.lwl_set_reg(rt, input);
        }

    /***END GETC WIP***/

    void simulator::i_ori(int instruction){
        int rs = instruction>>21;
        rs = rs & 0x1F;
        rs = regFile.get_reg(rs);               //src1

        int rt = instruction>>16;               //destination

        int immediate = instruction & 0xF;               

        regFile.set_reg((rs|immediate), (rt & 0x1F));
    }

    /***PUTC***/
        void simulator::i_sb(int instruction){
            signed short int offset = instruction & 0xFFFF;     //address src1

            int base = (instruction>>21) & 0xFF;                         //address src2
            base = regFile.get_reg(base);

            int rt = (instruction >> 16) & 0x1F;
            rt = regFile.get_reg(rt);
            char input = rt & 0xFF;

            int effectiveAddr = base + offset;

            memory.set_byte(effectiveAddr, input);

            if(effectiveAddr >= 0x30000004 && effectiveAddr < 0x30000008){  //if it's an i/o write
                memory.io_write(effectiveAddr);                             //output the lsb of the store operation
                memory.io_clear();
            }
        }
        void simulator::i_sh(int instruction){
            signed short int offset = instruction & 0xFFFF;     //immediate offset
            int base = (instruction>>21) & 0xFF;                //base
            base = regFile.get_reg(base);

            int memoryAddress = base + offset;                  //effectiveaddr

            int test = offset % 2;
            if(test != 0){                                        //test for memory access restriction on load halfword
                std::cerr<<"Memory offset unaligned in set halfword. Exiting with bad access error"<<std::endl;
                std::exit(-11);
            }

            int rt = (instruction>>16) & 0x1F;

            int hword = regFile.get_reg(rt); //get hword
            hword = hword & 0xFFFF;

            char msb = (hword>>8) & 0xFF;                                 //msb to be loaded into memory first
            memory.set_byte((memoryAddress), msb);

            char lsb = hword & 0xFF;                             //lsb then loaded into memory
            memory.set_byte((memoryAddress + 1), msb);        

            if(memoryAddress == 0x30000004 || memoryAddress == 0x30000006){ //if it's an i/o write
                memory.io_write(memoryAddress + 1);                         //output the lsb of the store operation
                memory.io_clear();
            }
        }
        void simulator::i_sw(int instruction){
            signed short int offset = instruction & 0xFFFF;                 //offset
            int base = instruction>>21;                                     //base
            base = base & 0x1F;

            int memoryAddress = base + offset;

            int test1 = offset % 4;
            if(test1 != 0){                                //test for memory access restriction on load word
                std::cerr<<"Memory offset unaligned in set word. Exiting with bad access error"<<std::endl;
                std::exit(-11);
            }

            int rt = (instruction>>16) & 0x1F;
            int word = regFile.get_reg((rt));             //retrieve word

            
            char b1 = word>>24;                                             //msb
            memory.set_byte((memoryAddress), b1);

            char b2 = ((word>>16)&0xFF);
            memory.set_byte((memoryAddress + 1), b2);

            char b3 = ((word>>8)&0xFF);
            memory.set_byte((memoryAddress + 2), b3);

            char b4 = ((word&0xFF));                                        //lsb
            memory.set_byte((memoryAddress + 3), b4);     

            if(memoryAddress == 0x30000004){        //if its a valid i/o write
                memory.io_write(memoryAddress + 3); //output the lsb of the store operation
                memory.io_clear();
            }
        }
    /***PUTC***/

    void simulator::i_slti(int instruction){
        int rs = (instruction>>21) & 0x1F;
        rs = regFile.get_reg(rs);

        signed short int immediate = instruction & 0xFFFF;

        int comparison;

        if(rs<immediate){
            comparison = 1;
        }
        else{
            comparison = 0;
        }

        int rt = (instruction>>16) & 0x1F;
        regFile.set_reg(comparison, rt);

    }
    void simulator::i_sltiu(int instruction){ //WIP - confused by documentation
        int rs = (instruction>>21) & 0x1F;
        rs = regFile.get_reg(rs);

        unsigned int immediate = instruction & 0xFFFF; //ISSUE AREA

        unsigned int temp;
        int immediatetest = immediate >> 15;
        if(immediatetest==1){

            temp = 0;
            for (int i = 0; i<16; i++){
                temp = temp << 1;
                temp = temp + 1;
            }
            temp = temp << 16;
            immediate = temp | immediate;
        }

        int comparison;

        if(rs<immediate){
            comparison = 1;
        }
        else{
            comparison = 0;
        }

        int rt = ((instruction>>16) & 0x1F);
        regFile.set_reg(comparison, rt);
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
    void simulator::j_j(int instruction){ //WIP
        int instr = (instruction & 0x03FFFFFF) << 2; //extract lower 26 bits
        jump = true;
        pcOffSet = instr;
    }

    void simulator::j_jal(int instruction){
        simulator::j_j(instruction);            //jump
        regFile.set_reg(programCounter+8, 31);  //and link
    }

//BEFORE RUNNING DIAGNOSTICS, change std::exit in get_byte to return check.

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
    simulator::GetAccessCheck(successfultest);
    simulator::CheckBlankRegions(successfultest);
    if(successfultest)
        std::cout << "\nMemory reads as intended\n";
    else
        std::cout << "\nMemory function failure\n";
}

void simulator::CheckMemZeroes(bool &success){  //checks readable regions for zeroes
    for(int i=0x10000000; i<0x11000000; i++){ //instr
        if(memory.get_byte(i) != 0)
            success = false;
    }

    for(int i=0x20000000; i<0x24000000; i++){ //data
        if(memory.get_byte(i) != 0)
            success = false;
    }

    for(int i=0x30000000; i<0x30000004; i++){ //getc
        if(memory.get_byte(i) != 0)
            success = false;
    }
}

void simulator::GetAccessCheck(bool &success){ //check that unreadable regions are unreadable
    char readbyte;
    for(int i=0x0; i<0x4; i++){ //null
        readbyte = memory.get_byte(i);
        if(readbyte != 0)
            success = false;
    }

    for(int i=0x30000004; i<0x30000008; i++){ //putc
        readbyte = memory.get_byte(i);
        if(readbyte != 4)
            success = false;
    }
}

void simulator::CheckBlankRegions(bool &success){
    char readbyte;
    for(int i=0x4; i<0x10000000; i++){ //null
        readbyte = memory.get_byte(i);
        if(readbyte != -1)
            success = false;
    }
    
    for(int i=0x11000000; i<0x20000000; i++){ //null
        readbyte = memory.get_byte(i);
        if(readbyte != -1)
            success = false;
    }
    
    for(int i=0x24000000; i<0x30000000; i++){ //null
        readbyte = memory.get_byte(i);
        if(readbyte != -1)
            success = false;
    }
    
    for(int i=0x30000008; i<=0xFFFFFFFF; i++){ //null
        readbyte = memory.get_byte(i);
        if(readbyte != -1)
            success = false;
    }
}