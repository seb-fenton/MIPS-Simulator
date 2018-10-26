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
        simulator(int LengthOfBinary, char* Memblock, bool& InputSuccess); //-this doesnt work : memory(LengthOfBinary, Memblock, InputSuccess);
        bool finished_sim();
        int fetch(sim_mem &memory, int pc);
        int decode(int instruction);
        char classification(int instruct);

        //memory checking functions
        void diagnostics();
        void SetAccessCheck(bool &success);
        void GetAccessCheck(bool &success);
        void CheckMemZeroes(bool &success);
        void CheckBlankRegions(bool &success);
        
        //R INSTRUCTIONS//

        //I INSTRUCTIONS//
        
        //J INSTRUCTIONS//  

};
