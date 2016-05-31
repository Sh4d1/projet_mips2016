#include <stdio.h>

#include "include/operations.h"
#include "include/gpr.h"

int main()
{
        init_GPR();
        set_register_value(2, -2147483645);
        set_register_value(10, -10);
        sub(2, 2, 10);
        printf("%d\n", get_register_value(2));
        return 0;
}
