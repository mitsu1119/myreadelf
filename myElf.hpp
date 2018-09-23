/* myElf.hpp */
#include <fstream>
#include <iostream>
#include <elf.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>

class elfTable {
public:
    const char *phdrType2str(int type);
};

class myElf {
private:
    char *head;
    Elf64_Ehdr *ehdr;
    Elf64_Shdr *shdr;
    Elf64_Shdr *shstr;
    Elf64_Phdr *phdr;

    // セクション名からセクションヘッダを所得
    Elf64_Shdr *getShdr(const char *name);
public:
    bool isElf();
    void printSections();
    void printSegments();
    void printSymbols();

    myElf(char *fileName);
    ~myElf();
};
