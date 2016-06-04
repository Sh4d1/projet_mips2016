#ifndef _MEM_
#define _MEM_

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>

#include "relocation.h"

/* structure d'une case mémoire */
struct memory_case {
    uint8_t value;
    bool r;
    bool w;
    bool x;
};

/* structure de la mémoire, un tableau de memory_case, plus la taille */
struct memory {
    //struct memory_case *memory;
    uint8_t *memory;
    uint32_t memory_size;
    uint8_t *framebuffer;
};

struct section {
    uint32_t address;
    uint32_t size;
};

struct table_symboles {
    struct symbole *sym;
    uint32_t size;
};


struct memory memory;
struct section text, data, bss;
struct table_symboles table_sym;

/* initialise la mémoire */
void init_memory(uint32_t mem_size, bool framebuffer);

/* verifie la validite d'une adresse */
void check_address(uint32_t address, uint8_t alignment);

/* change la section text */
void set_text_section(uint8_t *bytes, size_t size, uint32_t address, uint8_t align);

/* change la section data */
void set_data_section(uint8_t *bytes, size_t size, uint32_t address, uint8_t align);

/* change la section bss */
void set_bss_section(size_t size, uint32_t address, uint8_t align);

/* accesseurs des sections text et data */
uint32_t get_text_end();
uint32_t get_text_address();
uint32_t get_text_size();
uint8_t *get_text_bytes();
void set_text_address(uint32_t address);
uint32_t get_data_end();
uint32_t get_data_address();
uint32_t get_data_size();
uint8_t *get_data_bytes();
void set_data_address(uint32_t address);

/* determine si value tient sur un octet */
bool is_byte(uint32_t value);

/* determine si value tient sur un demi-mot */
bool is_half_word(uint32_t value);

/* change la valeur d'un octet en mémoire */
void set_byte(uint32_t address, uint8_t value);

/* change la valeur d'un demi mot en mémoire */
void set_half_word(uint32_t address, uint16_t value);

/* change la valeur d'un mot en mémoire */
void set_word(uint32_t address, uint32_t value);

/* place une chaine de charactere en memoire */
void set_n_string(uint32_t address, char *string, uint32_t size);

/* recupere la valeur d'un octet en mémoire */
uint8_t get_byte(uint32_t address);

/* recupere la valeur d'un demi-mot en mémoire */
uint16_t get_half_word(uint32_t address);

/* recupere la valeur d'un mot en mémoire */
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

void reloge(struct elf_descr *elf);
void reloge_text(struct elf_descr *elf);
void reloge_data(struct elf_descr *elf);

#endif /* end of include guard: _MEM_ */
