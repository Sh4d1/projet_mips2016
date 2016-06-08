#include "../include/relocation_s.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

void traduit_table_symboles(const Elf32_Sym *symtab, size_t symtab_size, const char *strtab, struct symbole **table_symboles, size_t *nb_symboles)
{
    *nb_symboles = symtab_size;
    *table_symboles = malloc(symtab_size*sizeof(struct symbole));
    for (size_t i = 0; i < symtab_size; i++) {
        char c = strtab[symtab[i].st_name];
        int j = 0;
        while (c != '\0') {
            j++;
            c = strtab[symtab[i].st_name+j];
        }
        char s[j];
        for (int h = 0; h < j; h++) {
            s[h] = strtab[symtab[i].st_name+h];
        }
        s[j] = '\0';
        if (j != 0) {
            (*table_symboles)[i].nom = malloc((j+1)*sizeof(char));
            strncpy((*table_symboles)[i].nom, s,j+1);
        } else {
            (*table_symboles)[i].nom = NULL;
        }
        (*table_symboles)[i].adresse = symtab[i].st_value;
    }
}


void reloge_symboles(const Elf32_Sym *symtab, size_t symtab_size, const char *strtab, uint32_t addr_text, uint32_t addr_data, uint32_t addr_bss, struct symbole **table_symboles, size_t *nb_symboles)
{
    *nb_symboles = symtab_size;
    *table_symboles = malloc(symtab_size*sizeof(struct symbole));
    for (size_t i = 0; i < symtab_size; i++) {
        char c = strtab[symtab[i].st_name];
        int j = 0;
        while (c != '\0') {
            j++;
            c = strtab[symtab[i].st_name+j];
        }
        char s[j];
        for (int h = 0; h < j; h++) {
            s[h] = strtab[symtab[i].st_name+h];
        }
        s[j] = '\0';
        if (j != 0) {
            (*table_symboles)[i].nom = malloc((j+1)*sizeof(char));
            strncpy((*table_symboles)[i].nom, s,j+1);
        } else {
            (*table_symboles)[i].nom = NULL;
        }
        if (symtab[i].st_shndx == symtab[1].st_shndx) {
            (*table_symboles)[i].adresse = symtab[i].st_value + addr_text;
        } else if (symtab[i].st_shndx == symtab[2].st_shndx) {
            (*table_symboles)[i].adresse = symtab[i].st_value + addr_data;
        } else if (symtab[i].st_shndx == symtab[3].st_shndx) {
            (*table_symboles)[i].adresse = symtab[i].st_value + addr_bss;
        } else {
            (*table_symboles)[i].adresse = symtab[i].st_value;
        }
    }
}

void reloge_section(uint32_t addr, uint8_t *data, const Elf32_Rel *rel_table, size_t rel_size, const struct symbole *table_symboles)
{

    for (size_t i = 0; i < rel_size; i++) {
        uint32_t P = addr + rel_table[i].r_offset;
        uint32_t A = *(data + rel_table[i].r_offset)<<24;
        A += *(data + rel_table[i].r_offset+1)<<16;
        A += *(data + rel_table[i].r_offset+2)<<8;
        A += *(data + rel_table[i].r_offset+3);
        uint32_t S = table_symboles[ELF32_R_SYM(rel_table[i].r_info)].adresse;
        uint32_t res;
        uint32_t AHI, ALO, AHL;
        switch(ELF32_R_TYPE(rel_table[i].r_info)) {
            case R_MIPS_32:
                *(data + rel_table[i].r_offset) = (A + S)>>24;
                *(data + rel_table[i].r_offset+1) = (A + S)>>16;
                *(data + rel_table[i].r_offset+2) = (A + S)>>8;
                *(data + rel_table[i].r_offset+3) = (A + S);
                break;
            case R_MIPS_26:
                A = (A << 6) >> 6;
                res = (((A<<2)|(P & 0xF0000000)) + S) >> 2;
                *(data + rel_table[i].r_offset) = (*(data + rel_table[i].r_offset) & 0xFC) + ((res>>24) & 0x3);
                *(data + rel_table[i].r_offset+1) = res>>16;
                *(data + rel_table[i].r_offset+2) = res>>8;
                *(data + rel_table[i].r_offset+3) = res;
                break;
            case R_MIPS_HI16:
                AHI = (A<<16)>>16;
                i++;
                A = *(data + rel_table[i].r_offset)<<24;
                A += *(data + rel_table[i].r_offset+1)<<16;
                A += *(data + rel_table[i].r_offset+2)<<8;
                A += *(data + rel_table[i].r_offset+3);
                ALO = (A<<16)>>16;
                AHL = (AHI<<16) + ALO;
                *(data + rel_table[i-1].r_offset+2) = (((AHL+S) >> 16)&0x0000FFFF)>>8;
                *(data + rel_table[i-1].r_offset+3) = ((AHL+S) >> 16)&0x0000FFFF;
                *(data + rel_table[i].r_offset+2) = (AHL+S)>>8;
                *(data + rel_table[i].r_offset+3) = AHL+S;
                break;
            default:
                printf("Mode de relocation non supporté.\n");
        }


    }


}
