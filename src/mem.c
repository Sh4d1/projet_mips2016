#include "../include/mem.h"
#include "../include/elf_reader.h"
#include "../include/gpr.h"

void init_memory(uint32_t mem_size)
{
    memory.memory = calloc(mem_size, sizeof(struct memory_case));
    memory.memory_size = mem_size;
    for (uint32_t i = 0; i < mem_size; i++) {
        memory.memory[i].value = 0;
        memory.memory[i].r = true;
        memory.memory[i].w = true;
        memory.memory[i].x = true;
    }
    // initialisation du pointeur de pile
    set_register_value(29, mem_size - 4);
}

/* verifie la validite d'une adresse */
void check_address(uint32_t address, uint8_t alignment)
{
    if (address > memory.memory_size) {
        printf("Adresse inexistante.\n");
        exit(EXIT_FAILURE);
    }
    if (address % alignment) {
        printf("%u\n", address);
        printf("Adresse non alignée.\n");
        exit(EXIT_FAILURE);
    }
}

void set_byte(uint32_t address, uint8_t value)
{
    check_address(address, 1);
    memory.memory[address].value = value;
}

void set_half_word(uint32_t address, uint16_t value)
{
    check_address(address, 2);
    memory.memory[address].value = (value & 0xFF00) >> 8;
    memory.memory[address+1].value = (value & 0x00FF);
}

void set_word(uint32_t address, uint32_t value)
{
    check_address(address, 4);
    memory.memory[address].value = (value & 0xFF000000) >> 24;
    memory.memory[address+1].value = (value & 0x00FF0000) >> 16;
    memory.memory[address+2].value = (value & 0x0000FF00) >> 8;
    memory.memory[address+3].value = (value & 0x000000FF);
}

/* place une chaine de charactere en memoire */
void set_string(uint32_t address, char *string)
{
    printf("%s\n", string);
}

uint8_t get_byte(uint32_t address)
{
    check_address(address, 1);
    return memory.memory[address].value;
}

uint16_t get_half_word(uint32_t address)
{
    check_address(address, 2);
    return (memory.memory[address].value << 8) + memory.memory[address + 1].value;
}

uint32_t get_word(uint32_t address)
{
    check_address(address, 4);
    return (memory.memory[address].value << 24) + (memory.memory[address + 1].value << 16) + (memory.memory[address + 2].value << 8) + memory.memory[address + 3].value;
}

uint32_t get_memory_size()
{
    return memory.memory_size;
}

void print_memory()
{
    for (uint32_t i = 0; i < memory.memory_size; i++) {
        printf("0x%04x : 0x%02x\n", i, memory.memory[i].value);
    }
}

void print_n_memory(uint32_t address, uint32_t n)
{
    for (uint32_t i = address; i < address+n; i++) {
        printf("0x%04x : 0x%02x\n", i, memory.memory[i].value);
    }
}

void display_memory(uint32_t address)
{
    printf("0x%06x: ", address);
    for (uint32_t i = address; i < address + 16; i++) {
        printf("%02x ", memory.memory[i].value);
    }
    printf("\n");
}

void diplay_memory_between(uint32_t address1, uint32_t address2)
{
    uint32_t offset = address2 - address1;
    for (uint32_t i = 0; i <= offset; i++) {
        if (!(i%16)) {
            printf("0x%06x: ", address1+i);
        }
        printf("%02x ", memory.memory[address1 + i].value);
        if (!((i+1)%16)) {
            printf("\n");
        }
    }
    if ((offset+1)%16) {
        printf("\n");
    }
}

uint32_t get_address_from_string(char *address)
{
    return strtoul(address, NULL, 16);
}

void free_memory()
{
    free(memory.memory);
}

void file_to_memory(char *file)
{
    struct elf_descr *elf;
    uint32_t text_addr = 0;
    uint8_t *text_bytes = NULL;
    size_t text_size = 0;
    uint8_t text_align = 0;

    elf = read_elf(file);
    if (elf == NULL) {
        printf("Erreur de lecture ELF\n");
        exit(EXIT_FAILURE);
    }
    get_text_section(elf, &text_bytes, &text_size, &text_addr, &text_align);

    for (uint32_t i = 0; i < text_size; i++) {
        set_byte(i, text_bytes[i]);
    }
}
