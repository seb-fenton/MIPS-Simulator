#pragma once

class simulator_registers{
    public:
        simulator_registers();
        char get_register(int address);
        void set_register(char input, int address);

    private:
        char registers [31];
};

class simulator_memory{

};
