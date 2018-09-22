#include <iostream>
#include "myElf.hpp"

int main(int args, char *argv[]) {
    myElf mf(argv[1]);

    if(!mf.isElf()) {
        std::cerr << "This file is not ELF." << std::endl;
        return -1;
    }

    mf.printSections();

    return 0;
}
