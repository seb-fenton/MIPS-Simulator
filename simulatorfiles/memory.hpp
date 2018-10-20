#pragma once

class sim_reg{
    public:
        sim_reg();
        int get_reg(int address);
        void set_reg(int input, int address);

    private:
        int reg[31];
};

class sim_mem{
    private:
    
        char selector;
    //should we use INT for the 32 bit spaces? null, getc, putc.
        char addr_null[4];          //0-3
        //0x4 to 0x0FFF FFFF
        char addr_instr[0x1000000]; //0x1000 0000 to 0x10FF FFFF
        //0x1100 0000 to 0x11FF FFFF

        char addr_data[0x4000000];  //0x2000 0000 to 0x23FF FFFF
        //0x2400 0000 to 0x2FFF FFFF

        char addr_getc[4]; //0x3000 0000 to 0x3000 0003 
        //READ ONLY, gets input via stdin

        char addr_putc[4]; //0x3000 0004 to 0x3000 0007 
        //WRITE ONLY, output through stdout?

        //0x3000 0008 0xFFFF FFFF

    public:
        sim_mem();
        void addressmap(int &address);
        void get_byte(int address);
        void set_byte(int address, char value);
};