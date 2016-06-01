#include <stdio.h>

#include "include/operations.h"
#include "include/gpr.h"
#include "include/mem.h"

int main()
{
        init_GPR();
        init_memory(0x1000);
        set_register_value(5, 99);
        printf("%d\n", get_register_value(5));
        sw(5, 0xE00, 0);
        lw(6, 0xE00, 0);
        printf("%d\n", get_register_value(6));
        return 0;
}
