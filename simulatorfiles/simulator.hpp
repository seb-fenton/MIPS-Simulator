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
        simulator(int LengthOfBinary, char* Memblock, bool& InputSuccess) : memory(LengthOfBinary, Memblock, InputSuccess);
        bool finished_sim();
        void fetch(sim_mem &memory);
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
