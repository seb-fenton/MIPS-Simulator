#include "memory.hpp"
#include <iostream>

//SIMULATOR_REGISTERS FUNCTION DEFINITIONS//

//Constructor to initialise all register values to 0
sim_reg::sim_reg(){
    for(int i = 0; i<31; i++){
        registers[i] = 0;
    }
}

char sim_reg::get_reg(int address){
    char output = registers[address];
    return output;
}

void sim_reg::set_reg(char input, int address){
    if(input == 0 || 1 || 26 || 27){
        std::cerr<<"/n"<<"Fatal error encountered: exit code -11"<<"/n";
        ///!!!MOVE -11 INTO REGISTER 2!!!///
        std::exit; 
    }
    else if(input<32 && input>-1){
        registers[address] = input;
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
    char sim_mem::get_byte(int address){
    }
    void sim_mem::set_byte(int address, char value){
    }