#pragma once
#include "memory.hpp"
#include <iostream>

class simulator{
    private:
        int programCounter;
        sim_reg regFile;
        sim_mem memory;

    public:
        //is the initializer list needed here?
        //fixed attempt?
        simulator(int LengthOfBinary, char* Memblock, bool& InputSuccess);
        bool finished_sim();
        int fetch(sim_mem &memory, int pc);
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
        void r_add(int instruction);    //WIP
        void r_addu(int instruction);
        void r_and(int instruction);    //WIP
        void r_div(int instruction);    //WIP
        void r_divu(int instruction);   //WIP
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
        void i_ori(int instruction);
        void i_xori(int instruction);

        //J INSTRUCTIONS//  
        void j_j(int instruction);

};
