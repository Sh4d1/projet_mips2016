#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../include/elf_reader.h"


int main(int argc, char *argv[])
{

    struct elf_descr *elf;
    uint32_t text_addr = 0;
    uint8_t *text_bytes = NULL;
    size_t text_size = 0;
    uint8_t text_align = 0;

    if (argc != 2) {
        printf("Usage: %s <fichier_elf.o> \n", argv[0]);
        return EXIT_SUCCESS;
    }

    printf("Lecture du fichier %s...\n", argv[1]);
    elf = read_elf(argv[1]);
    if (elf == NULL) {
        printf("Erreur de lecture ELF\n");
        return EXIT_FAILURE;
    }

    get_text_section(elf, &text_bytes, &text_size, &text_addr, &text_align);


    

    close_elf(elf);

	printf("\nThat's all folks!\n");
    return EXIT_SUCCESS;
}
