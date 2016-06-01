#include <stdio.h>

#include "include/operations.h"
#include "include/gpr.h"
#include "include/mem.h"

int main()
{
    init_GPR();
    init_memory(0x1000000);
    set_string(0x1000, "bonjour");
    char *hello = NULL;
    get_string(0x1000, &hello);
    printf("%s\n", hello);
    free(hello);
    set_register_value(10, 10);
    set_register_value_by_name("a0", 49);
    syscall();
    free_memory();
    return 0;
}
