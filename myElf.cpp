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
    Elf64_Shdr *shstr = getShstrtab();
    char *sectionName;

    for(int i=0; i < this->ehdr->e_shnum; i++) {
        sectionName = (char *)(this->head + shstr->sh_offset + shdrL->sh_name);
        if(!strcmp(sectionName, name)) break;
        shdrL = (Elf64_Shdr *)((char *)shdrL + this->ehdr->e_shentsize);
    }
    return shdrL;
}

/*
 * print
 */

void myElf::printHeader() {
    std::cout << "ELF Header:" << std::endl;
    std::cout << "  Head:      ";
    for(int b: this->ehdr->e_ident) printf(" %02x", b);
    printf("\n");
    printf("  Class:      %s\n", (this->ehdr->e_ident[EI_CLASS] == ELFCLASS64) ? "ELF64" :  "ELF32");
    printf("  Data:       %s\n", (this->ehdr->e_ident[EI_DATA] == ELFDATA2LSB) ? "2's complement, little endian" : "2's complement, big endian");
    printf("  Version:    0x%x%s\n", this->ehdr->e_ident[EI_VERSION], (this->ehdr->e_ident[EI_VERSION] == EV_CURRENT) ? " (current)" : " (non current)");
    printf("  OS/ABI:     %s\n", OSABI2str(this->ehdr->e_ident[EI_OSABI]));
    printf("  ABIversion: 0x%x\n", this->ehdr->e_ident[EI_ABIVERSION]);
    printf("  Type:       %s\n", elfType2str(this->ehdr->e_type));
    printf("  Machine:    %s\n", elfMachine2str(this->ehdr->e_machine));
    printf("  Version:    0x%x%s\n",this->ehdr->e_version, (this->ehdr->e_version == EV_CURRENT) ? " (current)" : " (non current)");
    printf("  Entry point address:               0x%x\n", this->ehdr->e_entry);
    printf("  Start of program header:           0x%x\n", this->ehdr->e_phoff);
    printf("  Start of section header:           0x%x\n", this->ehdr->e_shoff);
    printf("  Flags:                             0x%x\n", this->ehdr->e_flags);
    printf("  Size of this header:               0x%x\n", this->ehdr->e_ehsize);
    printf("  Size of program headers:           0x%x\n", this->ehdr->e_phentsize);
    printf("  Number of program headers:         %d\n", this->ehdr->e_phnum);
    printf("  Size of section headers:           0x%x\n", this->ehdr->e_shentsize);
    printf("  Number of section headers:         %d\n", this->ehdr->e_shnum);
    printf("  Section header string table index: %d\n", this->ehdr->e_shstrndx);
}

void myElf::printSections() {
    Elf64_Shdr *shdrL = this->shdr;
    Elf64_Shdr *shstr = getShstrtab();
    char *sectionName;

    std::cout << "Sections:" << std::endl;
    for(int i=0; i < this->ehdr->e_shnum; i++) {
        sectionName = (char *)(this->head + shstr->sh_offset + shdrL->sh_name);
        printf("  [%d]  %s", i, sectionName);
        printf("  %s", sectionType2str(shdrL->sh_type));
        printf("  0x%016x", shdrL->sh_addr);
        printf("  0x%08x", shdrL->sh_offset);
        printf("  0x%016x", shdrL->sh_size);
        printf("  0x%016x", shdrL->sh_entsize);

        printf("\n");
        shdrL = (Elf64_Shdr *)((char *)shdrL + this->ehdr->e_shentsize);
    }
}

void myElf::printSegments() {
    Elf64_Phdr *phdrL = this->phdr;
    std::cout << "Segments:" << std::endl;

    for(int i=0; i < this->ehdr->e_phnum; i++) {
        printf("  %s", phdrType2str(phdrL->p_type));
        printf("  0x%016x", phdrL->p_offset);
        printf("  0x%016x", phdrL->p_vaddr);
        printf("  0x%016x", phdrL->p_paddr);
        printf("  0x%016x", phdrL->p_filesz);
        printf("  0x%016x", phdrL->p_memsz);
        printf("  %c", phdrL->p_flags & PF_R ? 'R' : ' ');
        printf("%c", phdrL->p_flags & PF_W ? 'W' : ' ');
        printf("%c", phdrL->p_flags & PF_X ? 'E' : ' ');
        printf("  0x%x", phdrL->p_align);

        printf("\n");
        phdrL = (Elf64_Phdr *)((char *)phdrL + this->ehdr->e_phentsize);
    }
    printf("\n");

    // show sections in any segments
    Elf64_Shdr *shdrL;
    Elf64_Shdr *shstr = getShstrtab();
    char *sectionName;
    int size;
    phdrL = this->phdr;
    for(int i=0; i < this->ehdr->e_phnum; i++) {
        // search section headers
        printf("  [%d]  ", i);

        shdrL = this->shdr;
        for(int j=0; j < this->ehdr->e_shnum; j++) {
            size = (shdrL->sh_type != SHT_NOBITS) ? shdrL->sh_size : 0;
            if(shdrL->sh_offset >= phdrL->p_offset && shdrL->sh_offset + size <= phdrL->p_offset + phdrL->p_filesz) {
                sectionName = getnameInShstrtab(shdrL->sh_name);
                printf("%s ", sectionName);
            }
            shdrL = (Elf64_Shdr *)((char *)shdrL + this->ehdr->e_shentsize);
        }
        printf("\n");
        phdrL = (Elf64_Phdr *)((char *)phdrL + this->ehdr->e_phentsize);
    }
}

void myElf::printSymbols() {
    std::cout << "Symbols:" << std::endl;

    Elf64_Shdr *strtab = getShdr(".strtab");
    Elf64_Shdr *symtab = getShdr(".symtab");

    Elf64_Sym *symb;
    char *symbname;

    symb = (Elf64_Sym *)(this->head + symtab->sh_offset);
    for(int i=0; i < symtab->sh_size/symtab->sh_entsize; i++) {
        if(symb->st_name) {
            symbname = (char *)(this->head + strtab->sh_offset + symb->st_name);
            printf("  [%d]   %s\n", i, symbname);
        }
        symb = (Elf64_Sym *)((char *)symb +symtab->sh_entsize);
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
