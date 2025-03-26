#include "DiningPhilosophersProblem.h"

#include <iostream>
#include <string>
#include <thread>

int main(int argc, char *argv[]) {
    if(argc != 2)
    {
        std::cout << "Number of philosophers must be given.\n";
        return 1;
    }
    
    DiningPhilosophersProblem din(std::stoi(argv[1]));
    din.run();

    return 0;
}
