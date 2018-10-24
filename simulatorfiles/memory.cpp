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
    //initialise data memory to zero
    addr_null.resize(0x4); //size 4             
    addr_instr.resize(0x1000000); //0x1000000
    addr_data.resize(0x4000000); //0x4000000
    addr_getc.resize(0x4);//4
    addr_putc.resize(0x4);//4
    //load binary into executable memory

}
void sim_mem::printmem(){
    std::cout << "\nMemsizes:";
    std::cout << std::endl << addr_null.size();
    std::cout << std::endl << addr_instr.size();
    std::cout << std::endl << addr_data.size();
    std::cout << std::endl << addr_getc.size();
    std::cout << std::endl << addr_putc.size();

}
//Checks for a valid address, then subtracts the starting address and maps the appropriate memory region
//via the return character.
int sim_mem::addressmap(int &address) const{
    if(0 <= address && address < 4)
        return 0; // 0 for addr
    else if (0x10000000 <= address && address < 0x11000000){
        address = address - 0x10000000;
        return 1; //1 for inst
    }
    else if (0x20000000 <= address && address < 0x24000000){
        address = address - 0x20000000;
        return 2; //2 for data
    }  
    else if (0x30000000 <= address && address < 0x30000004){
        address = address - 0x30000000;
        return 3; //3 for getc
    }
    else if (0x30000004 <= address && address < 0x30000008){
        address = address - 0x30000004;
        return 4; //4 for putc
    }
    else return -1;
}
//TODO: DELETE PRINTING STATEMENTS AND RESTORE EXIT CODE
char sim_mem::get_byte(int address) const{
    int check = sim_mem::addressmap(address);
    /*Memory exceptions (-11): 
    0. Reading from addr_null
    4. Reading from write-only memory zone addr_putc
    -1. Address out of range or blank areas*/
    if(check == 0 || check == 4 || check == -1){
        switch(check){
        case 0: return 'a';
        case 4: return 'b';
        case -1: return 'c';
        }
        //std::exit(-11);
    }
        
    //ACTUAL FUNCTION
    //map numerical address to the correct array
    switch(check){
        default: break;
        case 1: return addr_instr[address];
        case 2: return addr_data[address];
        case 3: return addr_getc[address];
    }
}

//TODO: DELETE PRINTING STATEMENTS AND RESTORE EXIT CODE
void sim_mem::set_byte(int address, char value, bool &success){
    int check = sim_mem::addressmap(address);
    /*Memory exceptions (-11): 
    0. Writing to addr_null
    1. Writing to instruction Memory
    3. Writing to read-only memory zone addr_getc
    -1. Address out of range or Blank Areas
    */
    if(check == 0 || check == 1 || check == 3 || check ==-1)
        success = false; //std::exit(-11);
    else 
        success = true;

    //Actual function inserts value into address if no errors are thrown
    switch(check){
        default: break;
        case 2: addr_data[address] = value;
        case 4: addr_getc[address] = value;
    }
}