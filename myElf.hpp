/* myElf.hpp */
#include <fstream>
#include <iostream>
#include <elf.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>

const char *OSABI2str(int OSABI);
const char *phdrType2str(int type);
const char *elfType2str(int elfType);
const char *elfMachine2str(int elfMachine);
const char *sectionType2str(int sectionType);

class myElf {
private:
    char *head;
    Elf64_Ehdr *ehdr;
    Elf64_Shdr *shdr;
    Elf64_Phdr *phdr;

    // セクション名からセクションヘッダを所得
    Elf64_Shdr *getShdr(const char *name);

    // shstrtab セクションの所得
    Elf64_Shdr *getShstrtab() {
        return (Elf64_Shdr *)((char *)this->shdr + this->ehdr->e_shentsize * this->ehdr->e_shstrndx);
    }

public:
    bool isElf();
    void printHeader();
    void printSections();
    void printSegments();
    void printSymbols();

    myElf(char *fileName);
    ~myElf();
};
