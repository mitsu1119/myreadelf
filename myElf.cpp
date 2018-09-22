/* myElf.cpp */
#include "myElf.hpp"

/*
 * constructor, destructor
 */
myElf::myElf(char *fileName) {
    std::ifstream ifs(fileName);
    if(ifs.fail()) {
        std::cerr << "Error: Failed to open this file." << std::endl;
        abort();
    } else {
        std::string s((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
        this->head = new char[s.size()];
        s.copy(this->head, s.size());

        this->ehdr = (Elf64_Ehdr *)head;
        this->shstr = (Elf64_Shdr *)(head + this->ehdr->e_shoff + this->ehdr->e_shentsize * this->ehdr->e_shstrndx);

        ifs.close();
    }
}

myElf::~myElf() {
    delete[] head;
}

/*
 * getShdr
 */
Elf64_Shdr *myElf::getShdr(char *name) {
    Elf64_Shdr *shdr;
    char *sectionName;
    for(int i=0; i < this->ehdr->e_shnum; i++) {
        shdr = (Elf64_Shdr *)(this->head + this->ehdr->e_shoff + this->ehdr->e_shentsize * i);
        sectionName = (char *)(this->head + this->shstr->sh_offset + shdr->sh_name);
        if(!strcmp(sectionName, name)) break;
    }
    return shdr;
}

/*
 * print
 */
void myElf::printSections() {
    Elf64_Shdr *shdr;
    char *sectionName;
    std::cout << "Sections:" << std::endl;
    for(int i=0; i < this->ehdr->e_shnum; i++) {
        shdr = (Elf64_Shdr *)(this->head + this->ehdr->e_shoff + this->ehdr->e_shentsize * i);
        sectionName = (char *)(this->head + this->shstr->sh_offset + shdr->sh_name);
        printf("  [%d]   %s\n", i, sectionName);
    }
}

void myElf::printSymbols() {
    Elf64_Shdr *shdr;
    Elf64_Sym *symb;
    char *symname;
    std::cout << "Symbols:" << std::endl;

    // .strtab セクションの shdr を所得
    Elf64_Shdr *strtab = getShdr(".strtab");

    // 表示
    for(int i=0; i< this->ehdr->e_shnum; i++) {
        shdr = (Elf64_Shdr *)(this->head + this->ehdr->e_shoff + this->ehdr->e_shentsize * i);
        if(shdr->sh_type != SHT_SYMTAB) continue;
        for(int j=0; j < shdr->sh_size/shdr->sh_entsize; j++) {
            symb = (Elf64_Sym *)(this->head + shdr->sh_offset + shdr->sh_entsize * j);
            if(!symb->st_name) continue;
            symname = (char *)(this->head + strtab->sh_offset + symb->st_name);
            printf("  [%d]   %s\n", j, symname);
        }
        break;
    }
}

/*
 * isElf
 */
bool myElf::isElf() {
    if(this->ehdr->e_ident[EI_MAG0] == ELFMAG0 && this->ehdr->e_ident[EI_MAG1] == ELFMAG1 &&
        this->ehdr->e_ident[EI_MAG2] == ELFMAG2 && this->ehdr->e_ident[EI_MAG3] == ELFMAG3)
        return true;
    return false;
}
