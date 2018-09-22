/* myElf.hpp */
#include <fstream>
#include <iostream>
#include <elf.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>

class myElf {
private:
    char *head;
    Elf64_Ehdr *ehdr;
    Elf64_Shdr *shstr;

    // セクション名からセクションヘッダを所得
    Elf64_Shdr *getShdr(char *name);
public:
    bool isElf();
    void printSections();
    void printSymbols();

    myElf(char *fileName);
    ~myElf();
};
