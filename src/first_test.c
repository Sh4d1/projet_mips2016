#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../include/elf_reader.h"
#include "../include/mem.h"
#include "../include/gpr.h"
#include "../include/instructions.h"


int main(int argc, char *argv[])
{

    init_GPR();
    set_register_value(2, 11);
    set_register_value(3, 9);

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
    for (uint32_t i = 0; i < text_size; i += 4) {
        uint32_t word = text_bytes[i+3] + (text_bytes[i+2]<<8) + (text_bytes[i+1]<<16) + (text_bytes[i]<<24);
        parse_instruction(word);
        //printf("%08x\n",word);
    }


    printf("%u\n", get_register_value(10));
    printf("%u\n", get_register_value(11));
    printf("%u\n", get_register_value(12));
    close_elf(elf);

	printf("\nThat's all folks!\n");
    return EXIT_SUCCESS;
}
