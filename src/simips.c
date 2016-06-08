#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "../include/gpr.h"
#include "../include/mem.h"
#include "../include/shell.h"

void init_simips_argv(int argc, char **argv)
{
    uint32_t mem_size = 0x1000000;
    uint32_t text_address = 0x400000;
    uint32_t data_address = 0;
    bool fb = false;
    bool t = false;
    bool d = false;
    for (int32_t i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-s") == 0) {
            mem_size = strtoul(argv[i+1], NULL, 16);
            i++;
        }
        if (strcmp(argv[i], "-t") == 0) {
            text_address = strtoul(argv[i+1], NULL, 16);
            i++;
            t = true;
        }
        if (strcmp(argv[i], "-d") == 0) {
            data_address = strtoul(argv[i+1], NULL, 16);
            i++;
            d = true;
        }
        if(strcmp(argv[i], "-fb") == 0) {
            fb = true;
        }
    }
    if (d && !t) {
        data_address = 0;
        printf("Option -d non prise en compte\n");
    }
    if (text_address >= mem_size) {
        fprintf(stderr, "Erreur : L'adresse de la section text n'est pas dans la mémoire.\n");
        exit(EXIT_FAILURE);
    }
    init_memory(mem_size, fb);
    set_text_address(text_address);
    set_data_address(data_address);
}

int main(int argc, char **argv)
{
    init_simips_argv(argc, argv);

    shell_loop();
    free_memory();

    return 0;
}
