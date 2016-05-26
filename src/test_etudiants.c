#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "elf_reader.h"


int main(int argc, char *argv[])
{

    // Un mini-test qui charger un fichier elf et affiche les 4 premiers octets de sa section
    // (s'ils existent).
    // Regardez le Makfile pour voir comment lier le fichier .o distribué

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

    printf("\nInfos sur la section .text: adresse 0x%x, taille %lu, alignement %hhd\n",
           text_addr, text_size, text_align);
    printf("Les 4 premiers octets sont: ");
    if (text_size >= 4) {
        printf("%02x%02x%02x%02x ", text_bytes[0], text_bytes[1], text_bytes[2], text_bytes[3]);
        printf(" -- quelle est cette instruction? A vous!");
    } else {
        printf(" y'en a pas (assez)... dommage");
    }

    close_elf(elf);

	printf("\nThat's all folks!\n");
    return EXIT_SUCCESS;
}
