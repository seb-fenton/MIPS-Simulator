#include "memory.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdio.h>

//SIMULATOR_REGISTERS FUNCTION DEFINITIONS
//Constructor to initialise all register values to 0
sim_reg::sim_reg(){
    for(int i = 0; i<32; i++){
        reg[i] = 0;
    }
}

int sim_reg::get_reg(int regNum) const{
    int output = reg[regNum];
    return output;
}

void sim_reg::set_reg(int input, int regNum){
    /*if(input == 1 || 26 || 27){
        std::cerr<<"/n"<<"Fatal error encountered: exit code -11"<<"/n";
        std::exit(-11); 
    }*/
    if(input == 0){
        std::cerr<<"Write to $0. No action taken";
    }
    else if(input<32 && input>0){
        reg[regNum] = input;
    }
    else{
        std::cerr<<"\n"<<"Register Out of Range exit code -11"<<"\n";
        std::exit(-11);
    }
}

//WIP: THERE IS CERTAIN ALLOWED ACCESS TO HI AND LO AFTER A MULT/DIV
int sim_reg::get_hi() const{
    return hi;
}

void sim_reg::set_hi(int input){
    hi = input;
}

int sim_reg::get_lo() const{
    return lo;
}

void sim_reg::set_lo(int input){
    lo = input;
}


//SIMULATOR_MEMORY FUNCTION DEFINITIONS//
sim_mem::sim_mem(int LengthOfBinary, char* Memblock, bool& InputSuccess){
    //initialise data memory to zero
    addr_null.resize(0x4);          
    addr_instr.resize(0x1000000);
    addr_data.resize(0x4000000);
    addr_getc.resize(0x4);
    addr_putc.resize(0x4);

   
    int Address = 0x10000000;                    //load binary into executable memory
    for(int i = 0; i<LengthOfBinary; i++){
        char InputValue = Memblock[i];          //value to be input in set_byte command; reset to 0 each time
        sim_mem::set_instruc_byte(Address, InputValue, InputSuccess);
        if(InputSuccess==false)                 //exit if input failed
            i=LengthOfBinary;       
        Address = Address + 0x1;                //iterate up the memory stack to write in the next instruction byte
    }
}

int sim_mem::addressmap(int &address) const{
    if(0 <= address && address < 4)
        return 0; // 0 for null
    else if ((0x10000000 <= address) && (address < 0x11000000)){
        address = address - 0x10000000;
        return 1; //1 for inst
    }
    else if ((0x20000000 <= address) && (address < 0x24000000)){
        address = address - 0x20000000;
        return 2; //2 for data
    }  
    else if ((0x30000000 <= address) && (address < 0x30000004)){
        address = address - 0x30000000;
        return 3; //3 for getc
    }
    else if ((0x30000004 <= address) && (address < 0x30000008)){
        address = address - 0x30000004;
        return 4; //4 for putc
    }
    else return -1;
}

char sim_mem::get_byte(int address) const{
    int check = sim_mem::addressmap(address);
    /*Memory exceptions (-11): 
    0. Reading from addr_null
    4. Reading from write-only memory zone addr_putc
    -1. Address out of range or blank areas*/
    if(check == 0 || check == 4 || check == -1){
        //read = false;
        //char error = 0x00;
        //return error;
        std::exit(-11);
        
    }
    else{
        if(check == 1)  return addr_instr[address];
        if(check == 2)  return addr_data[address];
        if(check == 3){
            if(address == 3)
                return getchar();//only request an 8-bit value on the lsb (WIP is this syntax ok?)
            else
                return addr_getc[address];
        }
    }
}

void sim_mem::set_byte(int address, char value){
    int check = sim_mem::addressmap(address);
    /*Memory exceptions (-11): 
    0. Writing to addr_null
    1. Writing to instruction Memory
    3. Writing to read-only memory zone addr_getc
    -1. Address out of range or Blank Areas*/
    if(check == 0 || check == 1 || check == 3 || check ==-1){
        //write = false; 
        std::exit(-11);
    }
    else {
        if(check == 2)  addr_data[address] = value;
        if(check == 4){
            if(address == 3)
                putchar(value);         //WIP
        } 
    }
}

void sim_mem::set_instruc_byte(int address, char value, bool &success){
    if ((0x10000000 <= address) && (address < 0x11000000)){     //CHECK ADDRESS IN RANGE
        address = address - 0x10000000;                         //Subtract Instruction Offset
        addr_instr[address] = value;                            //Assign byte
        success = true;
        return;
    }
    else{
        success = false;
        return;
    }
}