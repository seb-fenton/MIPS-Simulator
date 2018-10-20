#include "memory.hpp"
#include <iostream>

//SIMULATOR_REGISTERS FUNCTION DEFINITIONS//

//Constructor to initialise all register values to 0
sim_reg::sim_reg(){
    for(int i = 0; i<31; i++){
        reg[i] = 0;
    }
}

int sim_reg::get_reg(int address){
    char output = reg[address];
    return output;
}

void sim_reg::set_reg(char input, int address){
    if(input == 0 || 1 || 26 || 27){
        std::cerr<<"/n"<<"Fatal error encountered: exit code -11"<<"/n";
        ///!!!MOVE -11 INTO REGISTER 2!!!///
        std::exit; 
    }
    else if(input<32 && input>-1){
        reg[address] = input;
    }
    else{
        std::cerr<<"/n"<<"Fatal error encountered: exit code -11"<<"/n";
        ///!!!MOVE -11 INTO REGISTER 2!!!///
        std::exit;
    }
}

    //SIMULATOR_MEMORY FUNCTION DEFINITIONS//
    //constructor
    sim_mem::sim_mem(){
        //load binary into executable memory

        //initialise data memory to zero
    }

    //functional methods
    void sim_mem::get_byte(int address, char &value){
        //Memory exception (-11): reading from write- only memory zone addr_putc
        if(writing to read only)
            break out
    }
    void sim_mem::set_byte(int address, char value){
        //Memory exception (-11): writing to read-only zone addr_getc
    }

    //CONFLICT AREA




    //OK HERES ALL MY NEW CODE BLADY BLAH