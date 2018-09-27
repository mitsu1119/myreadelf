/* elfTable.cpp */
#include "myElf.hpp"

const char *elfType2str(int elfType) {
    switch(elfType) {
    case ET_REL:         return "REL";
    case ET_EXEC:        return "EXEC";
    case ET_DYN:         return "DYN";
    case ET_CORE:        return "CORE";
    default:            return "Unknown...";
    }
}

const char *elfMachine2str(int elfMachine) {
    switch(elfMachine) {
    case EM_M32:        return "AT&T WE 32100";
    case EM_SPARC:      return "Sun Microsystems SPARC";
    case EM_386:        return "Intel 80386";
    case EM_68K:        return "Motorola 68000";
    case EM_88K:        return "Motorola 88000";
    case EM_860:        return "Intel 80860";
    case EM_MIPS:       return "MIPS RS3000";
    case EM_PARISC:     return "HP/PA";
    case EM_SPARC32PLUS:return "SPARC with enhanced instruction set";
    case EM_PPC:        return "PowerPC";
    case EM_PPC64:      return "PowerPC 64-bit";
    case EM_S390:       return "IBM S/390";
    case EM_ARM:        return "Advanced RISC Machines";
    case EM_SH:         return "Renesas SuperH";
    case EM_SPARCV9:    return "SPARC v9 64-bit";
    case EM_IA_64:      return "Intel Itanium";
    case EM_X86_64:     return "AMD x86-64";
    case EM_VAX:        return "DEC Vax";
    default:            return "Unknown...";
    }
}

const char *OSABI2str(int OSABI) {
    switch(OSABI) {
    case ELFOSABI_SYSV:         return "UNIX System V ABI"; // ELFOSABI_NONE
    case ELFOSABI_HPUX:         return "HP-UX ABI";
    case ELFOSABI_NETBSD:       return "NetBSD ABI";
    case ELFOSABI_LINUX:        return "Linux ABI";
    case ELFOSABI_SOLARIS:      return "Solaris ABI";
    case ELFOSABI_IRIX:         return "IRIX ABI";
    case ELFOSABI_FREEBSD:      return "FreeBSD ABI";
    case ELFOSABI_TRU64:        return "TRU64 UNIX ABI";
    case ELFOSABI_ARM:          return "ARM architecture ABI";
    case ELFOSABI_STANDALONE:   return "Stand-alone (embedded) ABI";
    default:                    return "Unknown...";
    }
}

const char *phdrType2str(int type) {
    switch(type) {
    case PT_NULL:       return "NULL";
    case PT_LOAD:       return "LOAD";
    case PT_DYNAMIC:    return "DYNAMIC";
    case PT_INTERP:     return "INTERP";
    case PT_NOTE:       return "NOTE";
    case PT_SHLIB:      return "SHLIB";
    case PT_PHDR:       return "PHDR";
    case PT_TLS:        return "TLS";
    case PT_NUM:        return "NUM";
    case PT_LOOS:       return "LOOS";
    case PT_GNU_EH_FRAME: return "GNU_EH_FRAME";
    case PT_GNU_STACK:  return "GNU_STACK";
    case PT_GNU_RELRO:  return "GNU_RELRO";
    default:            return "invalid";
    }
}

const char *sectionType2str(int sectionType) {
    switch(sectionType) {
    case SHT_NULL:      return "NULL";
    case SHT_PROGBITS:  return "PROGBITS";
    case SHT_SYMTAB:    return "SYMTAB";
    case SHT_STRTAB:    return "STRTAB";
    case SHT_RELA:      return "RELA";
    case SHT_HASH:      return "HASH";
    case SHT_DYNAMIC:   return "DYNAMIC";
    case SHT_NOTE:      return "NOTE";
    case SHT_NOBITS:    return "NOBITS";
    case SHT_REL:       return "REL";
    case SHT_SHLIB:     return "SHLIB";
    case SHT_DYNSYM:    return "DYNSYM";
    case SHT_INIT_ARRAY:return "INIT_ARRAY";
    case SHT_FINI_ARRAY:return "FINI_ARRAY";
    case SHT_GNU_HASH:  return "GNU_HASH";
    case SHT_GNU_verdef:return "GNU_VERDEF";
    case SHT_GNU_verneed:return "GNU_VERNEED";
    case SHT_GNU_versym:return "GNU_VERSYM";
    default:            return "Unknown...";
    }
}
