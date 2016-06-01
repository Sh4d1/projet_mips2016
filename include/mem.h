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

/* verifie la validite d'une adresse */
void check_address(uint32_t address, uint8_t alignment);

/* determine si value tiens sur un octet */
bool is_byte(uint32_t value);

/* determine si value tiens sur un demi mot */
bool is_half_word(uint32_t value);

/* change la valeur d'un octet en mémoire */
void set_byte(uint32_t address, uint8_t value);

/* change la valeur d'un demi mot en mémoire */
void set_half_word(uint32_t address, uint16_t value);

/* change la valeur d'un mot en mémoire */
void set_word(uint32_t address, uint32_t value);

/* place une chaine de charactere en memoire */
void set_string(uint32_t address, char *string);

uint8_t get_byte(uint32_t address);

uint16_t get_half_word(uint32_t address);

uint32_t get_word(uint32_t address);

uint32_t get_memory_size();

/* recupere une chaine de charactere en memoire */
void get_string(uint32_t address, char **string);

/* affiche la mémoire */
void print_memory();

void print_n_memory(uint32_t address, uint32_t n);

void display_memory(uint32_t address);

void diplay_memory_between(uint32_t address1, uint32_t address2);

uint32_t get_address_from_string(char *address);

/* libère la mémoire */
void free_memory();

void file_to_memory(char *file);

#endif /* end of include guard: _MEM_ */
