#include "memory.hpp"

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
        if(regNum == 0){
        }
        else if(regNum<32 && regNum>0){
            reg[regNum] = input;
        }
        else{
            std::exit(-11);
        }
    }

    void sim_reg::lwl_set_reg(int input, int regNum, int moduAmount){
        if(regNum == 0){
        }
        else if(regNum<32 && regNum>0){
            switch(moduAmount){     //see a similar table in the spec explanation of lwl
                case 0: reg[regNum] = reg[regNum] & 0x00000000;
                        break;
                case 1: reg[regNum] = reg[regNum] & 0x000000FF;
                        break;
                case 2: reg[regNum] = reg[regNum] & 0x0000FFFF;
                        break;
                case 3: reg[regNum] = reg[regNum] & 0x00FFFFFF;
                        break;
            }
            reg[regNum] = reg[regNum] | input;
        }
        else{
            std::exit(-11);
        }
    }

    void sim_reg::lwr_set_reg(int input, int regNum, int moduAmount){
        if(regNum == 0){
        }
        else if(regNum<32 && regNum>0){
            switch(moduAmount){         //see a similar table in the spec explanation of lwr
                case 0: reg[regNum] = reg[regNum] & 0xFFFFFF00;
                        break;
                case 1: reg[regNum] = reg[regNum] & 0xFFFF0000;
                        break;
                case 2: reg[regNum] = reg[regNum] & 0xFF000000;
                        break;
                case 3: reg[regNum] = reg[regNum] & 0x00000000;
                        break;
            }
            reg[regNum] = reg[regNum] | input;
        }
        else{
            std::exit(-11);
        }
    }

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
        //allocate to length of binary chars
        addr_instr.resize(LengthOfBinary);
        io_clear();

        int Address = 0x10000000;                    //load binary into executable memory
        for(int i = 0; i<LengthOfBinary; i++){
            char InputValue = Memblock[i];          //value to be input in set_byte command; reset to 0 each time
            sim_mem::set_instruc_byte(Address, InputValue, InputSuccess);
            if(InputSuccess==false)                 //exit if input failed
                i=LengthOfBinary;
            Address = Address + 0x1;                //iterate up the memory stack to write in the next instruction byte
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

    void sim_mem::io_read(){
        int io_input;
        io_input = getchar(); //eof 0xFFFFFFFF, else 0x000000XX

        if(io_input == EOF) io_input = -1;

        addr_getc[0] = (io_input >> 24) & 0xff;
        addr_getc[1] = (io_input >> 16) & 0xff;
        addr_getc[2] = (io_input >> 8) & 0xff;
        addr_getc[3] = io_input & 0xff;

        

    }
    void sim_mem::io_write(){
        putchar(addr_putc[3]);
        io_clear();
    }
    void sim_mem::io_clear(){
        //clear getc
        addr_getc[0] = 0;
        addr_getc[1] = 0;
        addr_getc[2] = 0;
        addr_getc[3] = 0;
        //clear putc
        addr_putc[0] = 0;
        addr_putc[1] = 0;
        addr_putc[2] = 0;
        addr_putc[3] = 0;
    }

    int sim_mem::addressmap(int &address){
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

    char sim_mem::get_byte(int address){
        int check = sim_mem::addressmap(address);
        /*Memory exceptions (-11):
        0. Reading from addr_null
        4. Reading from write-only memory zone addr_putc
        -1. Address out of range or blank areas*/
        if(check == 0 || check == 4 || check == -1){
            std::exit(-11);
        }
        else{

            if(check == 1){
                if(address < addr_instr.size()) return addr_instr[address];
                else    return 0;
            }
            if(check == 2){
                if(addr_data.find(address) != addr_data.end())  return addr_data[address];
                else return 0;
            }  
            if(check == 3)  return addr_getc[address];
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
            std::exit(-11);
        }
        else{
            if(check == 2){
                if(addr_data.find(address) != addr_data.end())  addr_data.insert(std::pair<int, char> (address, value));
                else    addr_data[address] = value;
            }
            if(check == 4)  addr_putc[address] = value;
        }
    }
