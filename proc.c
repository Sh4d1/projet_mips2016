#include <stdio.h>

#include "include/operations.h"
#include "include/gpr.h"

int main()
{
        init_GPR();
        set_register_value(8, 11);
        set_register_value(10, 1);
        sll(8, 8, 2);
        printf("%u\n", get_register_value(8));
        return 0;
}
