#pragma once
#include <vector>
class sim_reg{
    public:
        sim_reg();
        int get_reg(int regNum) const;
        void set_reg(int input, int regNum);

    private:
        int reg[31];
};

class sim_mem{
    private:
        int addressmap(int &address) const;
        void set_instruc_byte(int address, char value, bool &success);
        
        //CHANGE TO VECTORS.
        std::vector<char> addr_null; //size 4             
        std::vector<char> addr_instr; //0x1000000
        std::vector<char> addr_data; //0x4000000
        std::vector<char> addr_getc; //4
        std::vector<char> addr_putc;//4
        //0x4 to 0x0FFF FFFF            : Blank
        //0x1000 0000 to 0x10FF FFFF    : Instruction Space
        //0x1100 0000 to 0x11FF FFFF    : Blank
        //0x2000 0000 to 0x23FF FFFF    : Data Space
        //0x2400 0000 to 0x2FFF FFFF    : Blank
        //0x3000 0000 to 0x3000 0003    : GetC                  READ ONLY, input via stdin
        //0x3000 0004 to 0x3000 0007    : PutC                  Write Only, output via stdout
        //0x3000 0008 to 0xFFFF FFFF    : Blank

    public:
        sim_mem();
        sim_mem(int LengthOfBinary, char* Memblock, bool& InputSuccess);
        //NOT PASSED BY REFERENCE on purpose.
        char get_byte(int address, bool &read) const;
        void set_byte(int address, char value, bool &write);

};