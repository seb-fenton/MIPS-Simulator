#include <iostream>

int main(int argc, char* argv[]){

    //initialise variable fileName to 
    std::string fileName;

    if (argc > 1) {
        fileName = argv[1];
        std::cout << fileName << std::endl;
    } 
    
    else {
        //ISSUE!!! 
        std::cerr<<"ERROR: -01????"<<std::endl;
        std::exit(0);
    }
}