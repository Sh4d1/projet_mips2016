#include <stdio.h>

#include "include/operations.h"
#include "include/gpr.h"
#include "include/mem.h"

int main()
{
    init_memory(0x1000000);
    set_string(0x1000, "bonjour", 7);
    char *bonjour = NULL;
    get_string(0x1000, &bonjour);
    printf("%s\n", bonjour);
    free(bonjour);
    free_memory();
    return 0;
}
