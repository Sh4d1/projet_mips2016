#include <stdio.h>

#include "include/operations.h"
#include "include/gpr.h"
#include "include/mem.h"

int main()
{
    init_GPR();
    init_memory(0x1000);
    set_register_value(10, 10);
    set_register_value(5, 0x10);
    sw(10, 5, 0);

    return 0;
}
