#include <stdio.h>

#include "include/operations.h"
#include "include/gpr.h"
#include "include/mem.h"

int main()
{
    init_memory(0x1000000);
    set_n_string(0x1000, "bonjour", 7);
    get_string(0x1000, &bonjour);
    printf("%s\n", bonjour);
    set_register_value(A0, 0x1000);
    set_register_value(A1, 7);
    syscall();
    printf("%s\n", bonjour);
    free(bonjour);
    free_memory();
    return 0;
}
