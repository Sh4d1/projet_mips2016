#include <stdio.h>

#include "include/operations.h"
#include "include/gpr.h"
#include "include/mem.h"

int main()
{
    init_memory(0x1000000);
    name = 0;
    (name) ? : name++;
    free_memory();
    return 0;
}
