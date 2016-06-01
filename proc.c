#include <stdio.h>

#include "include/operations.h"
#include "include/gpr.h"
#include "include/mem.h"

#define INT32_MAX 2147483647
#define UINT32_MAX 4294967295

int main()
{
        init_GPR();
        init_memory(0x1000);
        set_register_value(10, INT32_MAX);
        set_register_value(11, 2);
        mult(10, 11);
        printf("%d\n", get_LO_value());
        printf("%d\n", get_HI_value());

        return 0;
}
