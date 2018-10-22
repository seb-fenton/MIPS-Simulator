#include "memory.hpp"
#include <iostream>

//SIMULATOR_REGISTERS FUNCTION DEFINITIONS
//Constructor to initialise all register values to 0
sim_reg::sim_reg(){
    for(int i = 0; i<31; i++){
        reg[i] = 0;
    }

}

int sim_reg::get_reg(int regNum) const{
    char output = reg[regNum];
    return output;
}

void sim_reg::set_reg(int input, int regNum){
    if(input == 0 || 1 || 26 || 27){
        std::cerr<<"/n"<<"Fatal error encountered: exit code -11"<<"/n";
        ///!!!MOVE -11 INTO REGISTER 2!!!///
        std::exit(-11); 
    }
    else if(input<32 && input>-1){
        reg[regNum] = input;
    }
    else{
        std::cerr<<"/n"<<"Fatal error encountered: exit code -11"<<"/n";
        ///!!!MOVE -11 INTO REGISTER 2!!!///
        std::exit(-11);
    }
}

//SIMULATOR_MEMORY FUNCTION DEFINITIONS//
//constructor
sim_mem::sim_mem(){
    //load binary into executable memory

    //initialise data memory to zero
}

//Checks for a valid address, then subtracts the starting address and maps the appropriate memory region
//via the return character.
char sim_mem::addressmap(int &address) const{
    if(0 <= address && address < 4)
        return 0; // 0 for addr
    if (0x10000000 <= address && address < 0x11000000){
        address = address - 0x10000000;
        return 1; //1 for inst
    }
    if (0x20000000 <= address && address < 0x24000000){
        address = address - 0x20000000;
        return 2; //2 for data
    }  
    if (0x30000000<= address && address < 0x30000004){
        address = address - 0x30000000;
        return 3; //3 for getc
    }
    if (0x30000004 <= address && address < 0x30000008){
        address = address - 0x30000004;
        return 4; //4 for putc
    }
    else return 'x';
}

char sim_mem::get_byte(int address) const{
    char check = sim_mem::addressmap(address);
    /*Memory exceptions (-11): 
    1. Reading from addr_null
    2. Reading from write-only memory zone addr_putc
    3. Address out of range or blank areas*/
    if(check == '0' || check == '4' || check == 'x')
        std::exit(-11);

    //ACTUAL FUNCTION
    //map numerical address to the correct array
    switch(check){
        case '1': return addr_instr[address];
        case '2': return addr_data[address];
        case '3': return addr_getc[address];
    }
}

void sim_mem::set_byte(int address, char value){
    char check = sim_mem::addressmap(address);
    /*Memory exceptions (-11): 
    1. Writing to addr_null
    2. Writing to instruction Memory
    3. Writing to write-only memory zone addr_putc
    4. Address out of range or Blank Areas
    */
    if(check == '0' || check == '1' || check == '3' || check =='x')
        std::exit(-11);
        
    //Actual function inserts value into address if no errors are thrown
    switch(check){
        case '2': addr_data[address] = value;
        case '4': addr_getc[address] = value;
    }
}