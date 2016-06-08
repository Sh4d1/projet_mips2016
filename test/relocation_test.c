#include <string.h>

#include "include/relocation.h"
#include "include/elf_reader.h"

#include <stdio.h>

int main(int argc, char *argv[]) {

    struct elf_descr *elf;

    elf = read_elf(argv[1]);
    if (elf == NULL) {
        printf("Erreur de lecture ELF\n");
        return 0;
    }
    char *strtab = NULL;
    size_t size = 0;

    Elf32_Sym *symtab = NULL;
    size_t sym_size = 0;

    get_string_table(elf, &strtab, &size);
    get_symbol_table(elf, &symtab, &sym_size);

    struct symbole *ts = NULL;
    size_t nb_sym = 0;


    //traduit_table_symboles(symtab, sym_size, strtab, &ts, &nb_sym);
    reloge_symboles(symtab, sym_size, strtab, 0x40000, 0x41000, 0x42000, &ts, &nb_sym);

    for (int i = 0; i < nb_sym; i++) {
        printf("%s : %x\n", ts[i].nom, ts[i].adresse);
    }

    for (size_t i = 0; i < nb_sym; i++) {
        free(ts[i].nom);
    }
    free(ts);
    close_elf(elf);

    return 0;
}
