#pragma once
#include "memory.hpp"
#include <iostream>

class simulator{
    private:
        int programCounter, pcOffSet;
        bool delayedOp, branch;
        sim_reg regFile;
        sim_mem memory;

    public:
        simulator(int LengthOfBinary, char* Memblock, bool& InputSuccess);
        void updatePC();
        bool finished_sim();

        int fetch();
        int decode(int instruction);
        int r_classification(int instruction);
        int branch_classification(int instruction);
        void execute(int instruction);

        //memory checking functions
        void diagnostics();
        void SetAccessCheck(bool &success);
        void GetAccessCheck(bool &success);
        void CheckMemZeroes(bool &success);
        void CheckBlankRegions(bool &success);
        
        //R INSTRUCTIONS//
        void r_add(int instruction);
        void r_addu(int instruction);
        void r_and(int instruction);
        void r_div(int instruction);    //not done
        void r_divu(int instruction);   //not done
        void r_mfhi(int instruction);
        void r_mflo(int instruction);
        void r_mthi(int instruction);
        void r_mtlo(int instruction);

        void r_or(int instruction);
        void r_sll(int instruction);
        void r_sllv(int instruction);
        void r_sra(int instruction);
        void r_srav(int instruction);
        void r_srl(int instruction);
        void r_srlv(int instruction);
        
        void r_sub(int instruction);
        void r_subu(int instruction);
        void r_xor(int instruction);

        //I INSTRUCTIONS//
        void i_addi(int instruction);
        void i_addiu(int instruction);
        void i_andi(int instruction);

        void i_beq(int instruction);
        void i_bgez(int instruction);
        void i_bgezal(int instruction);
        void i_bgtz(int instruction);
        void i_blez(int instruction);
        void i_bltz(int instruction);
        void i_bltzal(int instruction);
        void i_bne(int instruction);

        void i_lb(int instruction);
        void i_lbu(int instruction);
        void i_lh(int instruction);
        void i_lhu(int instruction);
        void i_lui(int instruction);
        void i_ori(int instruction);
        void i_xori(int instruction);
        
        //J INSTRUCTIONS//  
        void j_j(int instruction);
        void j_jal(int instruction);

};
