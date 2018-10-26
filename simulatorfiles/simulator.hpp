#pragma once
#include "memory.hpp"

class simulator{
    private:
        int programCounter;
        sim_reg regFile;
        sim_mem memory;

    public:
        simulator(int LengthOfBinary, char* Memblock, bool& InputSuccess) : memory(LengthOfBinary, Memblock, InputSuccess);
        bool finished_sim();
        void fetch(sim_mem &memory);
        
        char classification(int instruct);

        //R INSTRUCTIONS//
        int R_add();

        //I INSTRUCTIONS//

        
        //J INSTRUCTIONS//  

};
