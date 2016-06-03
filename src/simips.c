#include <stdlib.h>
#include <inttypes.h>

#include "../include/gpr.h"
#include "../include/mem.h"
#include "../include/shell.h"

void init_default_simips()
{
    init_GPR();
    init_memory(0x1000000);
}

void init_simips(uint32_t mem_size) {
    init_GPR();
    init_memory(mem_size);
}

void init_simips_argv(int argc, char **argv) {
    if (argc == 1) {
        init_default_simips();
    } else {
        uint8_t t = 0;
        uint8_t d = 0;
        uint8_t s = 0:
        for (uint32_t i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-s") == 0) {
                init_memory(strtoul(argv[i+1], NULL, 16));
                printf("0x%x\n", get_memory_size());
                i++;
                s = 1;
            }
            if (strcmp(argv[i], "-t") == 0) {
                set_text_address(strtoul(argv[i+1], NULL, 16));
                i++;
                t = 1;
            }
            if (strcmp(argv[i], "-d") == 0) {
                set_data_section(strtoul(argv[i+1], NULL, 16));
                i++;
                d = 1;
            }
        }
        if (d == 1 && t == 0) {
            set_data_section(0);
            printf("Option -d non prise en compte\n");
        }
        if (s == 0) {
            init_default_simips();
        }
    }
}

int main(int argc, char **argv) {
    init_simips_argv(argc, argv);

    shell_loop();
    free_memory();

    return 0;
}
