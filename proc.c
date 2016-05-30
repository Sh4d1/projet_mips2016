#include <stdio.h>
#include "include/operations.h"
#include "include/gpr.h"

int main()
{
        init_GPR();
        set_register_value(8, 8);
        set_register_value(10, 10);
        addi(8, 10, 12);
        printf("%d\n", get_register_value(8));
        return 0;
}
