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

        this->ehdr = (Elf64_Ehdr *)this->head;
        this->shdr = (Elf64_Shdr *)(this->head + this->ehdr->e_shoff);
        this->shstr = (Elf64_Shdr *)((char *)this->shdr + this->ehdr->e_shentsize * this->ehdr->e_shstrndx);
        this->phdr = (Elf64_Phdr *)(this->head + this->ehdr->e_phoff);

        ifs.close();
    }
}

myElf::~myElf() {
    delete[] head;
}

/*
 * getShdr
 */
Elf64_Shdr *myElf::getShdr(const char *name) {
    Elf64_Shdr *shdrL = this->shdr;
    char *sectionName;
    for(int i=0; i < this->ehdr->e_shnum; i++) {
        sectionName = (char *)(this->head + this->shstr->sh_offset + shdrL->sh_name);
        if(!strcmp(sectionName, name)) break;
        shdrL = (Elf64_Shdr *)((char *)shdrL + this->ehdr->e_shentsize);
    }
    return shdrL;
}

/*
 * print
 */
void myElf::printSections() {
    Elf64_Shdr *shdrL = this->shdr;
    char *sectionName;
    std::cout << "Sections:" << std::endl;
    for(int i=0; i < this->ehdr->e_shnum; i++) {
        sectionName = (char *)(this->head + this->shstr->sh_offset + shdrL->sh_name);
        printf("  [%d]   %s\n", i, sectionName);
        shdrL = (Elf64_Shdr *)((char *)shdrL + this->ehdr->e_shentsize);
    }
}

void myElf::printSegments() {
    Elf64_Phdr *phdrL = this->phdr;
    elfTable search;
    std::cout << "Segments:" << std::endl;
    for(int i=0; i< this->ehdr->e_phnum; i++) {
        std::cout << search.phdrType2str(phdrL->p_type) << std::endl;
        phdrL = (Elf64_Phdr *)((char *)phdrL + this->ehdr->e_phentsize);
    }
}

void myElf::printSymbols() {
    Elf64_Shdr *shdrL;
    Elf64_Sym *symb;
    char *symname;
    std::cout << "Symbols:" << std::endl;

    // .strtab セクションの shdr を所得
    Elf64_Shdr *strtab = getShdr(".strtab");

    for(int i=0; i< this->ehdr->e_shnum; i++) {
        shdrL = (Elf64_Shdr *)(this->head + this->ehdr->e_shoff + this->ehdr->e_shentsize * i);
        if(shdrL->sh_type != SHT_SYMTAB) continue;
        for(int j=0; j < shdrL->sh_size/shdrL->sh_entsize; j++) {
            symb = (Elf64_Sym *)(this->head + shdrL->sh_offset + shdrL->sh_entsize * j);
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
