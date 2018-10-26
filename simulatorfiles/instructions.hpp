#pragma once
#include "memory.hpp"

class instructionMap{
    private:
        int instructionNum;

    public:
        void map_instruction(sim_mem &memory, int &programCounter){
            //get 32 bytes out of instruction memory

            //check opcode
        }
        void fetchnDecode(sim_mem &memory);
        char classification();

        //R INSTRUCTIONS//
        int R_add();

        //I INSTRUCTIONS//

        
        //J INSTRUCTIONS//  

};
