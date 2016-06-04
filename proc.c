#include <stdio.h>

#include "include/operations.h"
#include "include/gpr.h"
#include "include/mem.h"

int main()
{
    init_memory(0x1000000, false);

    free_memory();
    return 0;
}
