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
        set_register_value(10, 10);
        sw(10, 0x2000000);

        return 0;
}
