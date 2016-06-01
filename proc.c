#include <stdio.h>

#include "include/operations.h"
#include "include/gpr.h"
#include "include/mem.h"

int main()
{
    init_GPR();
    init_memory(0x1000000);
    set_string(0x1000, "hello");
    printf("%s\n", get_string(0x1000));
    free_memory();
    return 0;
}
