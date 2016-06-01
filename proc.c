#include <stdio.h>

#include "include/operations.h"
#include "include/gpr.h"

int main()
{
        init_GPR();
        init_memory(0x1000);
        printf("%x\n", get_register_value(29));
        return 0;
}
