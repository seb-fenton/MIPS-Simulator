#include "main.hpp"
#include "instructions.hpp"
#include "memory.hpp"
#include <iostream>
#include <string>
#include <map>



int main(int argc, char* argv[]){

    //std::map<std::string> function_map;

    std::string fileName;
    std::cout << argv[0] << std::endl;

    if (argc > 1) {
        fileName = argv[1];
        std::cout << fileName << std::endl;
    } else {
        //ISSUE!!! 
        std::cerr<<"ERROR: -01????"<<std::endl;
        std::exit(0);
    }




    return 0;
}