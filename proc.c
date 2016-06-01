#include <stdio.h>

#include "include/operations.h"
#include "include/gpr.h"
#include "include/mem.h"

int main()
{
    init_GPR();
    init_memory(0x1000000);
    set_string(0x1000, "bonjour");
    set_register_value_by_name("v0", 5);
    set_register_value_by_name("a0", 0x1000);
    syscall();
    printf("%u", get_register_value('v'));
    free_memory();
    return 0;
}
