/* myElf.hpp */
#include <fstream>
#include <iostream>
#include <elf.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>

class myElf {
private:
    char *head;
    Elf64_Ehdr *ehdr;
    Elf64_Shdr *shstr;
public:
    bool isElf();
    void printSections();

    myElf(char *fileName);
    ~myElf();
};
