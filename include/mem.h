#ifndef _MEM_
#define _MEM_

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>

/* structure d'une case mémoire */
struct memory_case {
    uint8_t value;
    bool r;
    bool w;
    bool x;
};

/* structure de la mémoire, un tableau de memory_case, plus la taille */
struct memory {
    struct memory_case *memory;
    uint32_t memory_size;
};

struct memory memory;


/* initialise la mémoire */
void init_memory(uint32_t mem_size);

/* change la valeur d'un octet en mémoire */
void set_byte(uint32_t adress, uint8_t value);

/* change la valeur d'un demi mot en mémoire */
void set_half_word(uint32_t adress, uint16_t value);

/* change la valeur d'un mot en mémoire */
void set_word(uint32_t adress, uint32_t value);

uint8_t get_byte(uint32_t adress);

uint16_t get_half_word(uint32_t adress);

uint32_t get_word(uint32_t adress);

/* affiche la mémoire */
void print_memory();

void print_n_memory(uint32_t adress, uint32_t n);

/* libère la mémoire */
void free_memory();


#endif /* end of include guard: _MEM_ */
