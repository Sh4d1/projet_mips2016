#include "mem.h"

int main(int argc, char **argv)
{
    init_memory();
    print_memory();
    free_memory();
    return 0;
}
#define MEM_SIZE 65536
void init_memory()
{
    memory = malloc(MEM_SIZE * sizeof(struct memory_case));
    for (uint32_t i = 0; i < MEM_SIZE; i++) {
        memory[i].adress = i;
        memory[i].value = i;
    }
}

void print_memory()
{
    for (uint32_t i = 0; i < MEM_SIZE; i++) {
        printf("0x%04x : 0x%02x\n", memory[i].adress, memory[i].value);
    }
}

void free_memory()
{
    free(memory);
}
