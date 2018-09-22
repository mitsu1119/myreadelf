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
 * printSections
 */
void myElf::printSections() {
    Elf64_Shdr *shdr;
    std::cout << "Sections:" << std::endl;
    for(int i=0; i < this->ehdr->e_shnum; i++) {
        shdr = (Elf64_Shdr *)(this->head + this->ehdr->e_shoff + this->ehdr->e_shentsize * i);
        std::cout << "    " << (char *)(this->head + this->shstr->sh_offset + shdr->sh_name) << std::endl;
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
