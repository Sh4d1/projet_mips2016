#include "../include/mem.h"
#include "../include/elf_reader.h"

// int main(int argc, char **argv)
// {
//     struct memory *mem = malloc(sizeof(struct memory));
//     init_memory(mem, 0x1000);
//     set_byte(mem, 0x100, 0x20);
//     set_half_word(mem, 0x102, 0x1234);
//     set_word(mem, 0x106, 0x12345678);
//     print_n_memory(*mem, 0x100, 10);
//     printf("%x\n", get_word(mem, 0x100));
//     //print_memory(*mem);
//     free_memory(mem);
//     return 0;
// }

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

void set_byte(uint32_t adress, uint8_t value)
{
    if (adress > memory.memory_size) {
        printf("DEAD\n");
        exit(EXIT_FAILURE);
    } else {
        memory.memory[adress].value = value;
    }
}

void set_half_word(uint32_t adress, uint16_t value)
{
    memory.memory[adress].value = (value & 0xFF00)>>8;
    memory.memory[adress+1].value = (value & 0x00FF);
}

void set_word(uint32_t adress, uint32_t value)
{
    memory.memory[adress].value = (value & 0xFF000000)>>24;
    memory.memory[adress+1].value = (value & 0x00FF0000)>>16;
    memory.memory[adress+2].value = (value & 0x0000FF00)>>8;
    memory.memory[adress+3].value = (value & 0x000000FF);
}

uint8_t get_byte(uint32_t adress)
{
    return memory.memory[adress].value;
}

uint16_t get_half_word(uint32_t adress)
{
    return (memory.memory[adress].value<<8) + memory.memory[adress+1].value;
}

uint32_t get_word(uint32_t adress)
{
    return (memory.memory[adress].value<<24) + (memory.memory[adress+1].value<<16) + (memory.memory[adress+2].value<<8) + memory.memory[adress+3].value;
}


void print_memory()
{
    for (uint32_t i = 0; i < memory.memory_size; i++) {
        printf("0x%04x : 0x%02x\n", i, memory.memory[i].value);
    }
}

void print_n_memory(uint32_t adress, uint32_t n)
{
    for (uint32_t i = adress; i < adress+n; i++) {
        printf("0x%04x : 0x%02x\n", i, memory.memory[i].value);
    }
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
    init_memory(0x1000000);

    for (uint32_t i = 0; i < text_size; i++) {
        set_byte(i, text_bytes[i]);
    }
}
