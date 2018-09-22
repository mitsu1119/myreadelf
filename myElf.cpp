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

        ifs.close();
    }
}

myElf::~myElf() {
    delete[] head;
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
