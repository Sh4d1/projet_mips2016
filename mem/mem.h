#ifndef _MEM_
#define _MEM_

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

struct memory_case {
    uint32_t adress;
    uint8_t value;
};

struct memory_case *memory;

/* initialise la mémoire */
void init_memory();
void print_memory();
void free_memory();


#endif /* end of include guard: _MEM_ */
