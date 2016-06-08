#ifndef _MEM_
#define _MEM_

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>

#include "relocation_s.h"

/* structure de la mémoire, un tableau de memory_case, plus la taille,et le framebuffer */
struct memory {
    uint8_t *memory;
    uint32_t memory_size;
    uint8_t *framebuffer;
};

/* structure d'une section */
struct section {
    uint32_t address;
    uint32_t size;
};

/* strucutre de la table des symboles */
struct table_symboles {
    struct symbole *sym;
    size_t size;
};

/* decalaration de la memoire, des sections .text, .data et .bss
et de la table des symboles */
struct memory memory;
struct section text, data, bss;
struct table_symboles table_sym;

/* initialise la mémoire */
void init_memory(uint32_t mem_size, bool framebuffer);

/* verifie la validite d'une adresse */
bool check_address(uint32_t address, uint8_t alignment);

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

/* retourne la taille de .bss */
uint32_t get_bss_size();

/* retourne la fin de .bss */
uint32_t get_bss_end();

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

/* place une chaine de charactere en memoire, de taille size */
void set_n_string(uint32_t address, char *string, uint32_t size);

/* recupere la valeur d'un octet en mémoire */
uint8_t get_byte(uint32_t address);

/* recupere la valeur d'un demi-mot en mémoire */
uint16_t get_half_word(uint32_t address);

/* recupere la valeur d'un mot en mémoire */
uint32_t get_word(uint32_t address);

/* recupere la taile de la mémoire */
uint32_t get_memory_size();

/* recupere un pointeur sur le premier octet du framebuffer */
uint8_t *get_framebuffer();

/* recupere une chaine de charactere en memoire */
void get_string(uint32_t address, char **string);

/* affiche la mémoire */
void display_memory_between(uint32_t address1, uint32_t address2);

/* libère la mémoire */
void free_memory();

/* charge le fichier file en mémoire + symbole + relocation */
void file_to_memory(char *file);

/* reloge les section text et data */
void reloge_text(struct elf_descr *elf);
void reloge_data(struct elf_descr *elf);

/* recupere un symbole depuis addr */
char *get_sym_from_address(uint32_t addr);

#endif /* end of include guard: _MEM_ */
