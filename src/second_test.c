#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../include/shell.h"


int main(int argc, char *argv[])
{
    shell_loop();

	printf("\nThat's all folks!\n");
    return EXIT_SUCCESS;
}
