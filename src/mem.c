#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../include/mem.h"
#include "../include/elf_reader.h"
#include "../include/gpr.h"
#include "../include/framebuffer_s.h"
#include "../include/instructions.h"

/* initialise la mémoire */
void init_memory(uint32_t mem_size, bool framebuffer)
{
    if (mem_size >= 0xFFFF0600) {
        fprintf(stderr, "Taille de la mémoire trop grande.\n");
        exit(EXIT_FAILURE);
    }

    // initialisation de la memoire
    memory.memory = malloc(mem_size * sizeof(uint8_t));
    if (!memory.memory) {
        fprintf(stderr, "Erreur allocation de la mémoire.\n");
        exit(EXIT_FAILURE);
    }
    memory.memory_size = mem_size;
    for (uint32_t i = 0; i < mem_size; i++) {
        set_byte(i, 0);
    }

    // initialisation du framebuffer si besoin
    if (framebuffer) {
        memory.framebuffer = framebuffer_init_display();
    }

    // initialisation des registres et du pointeur de pile
    init_GPR();
    set_register_value(29, mem_size - ((mem_size % 4) + 4));
}

/* verifie la validite d'une adresse */
bool check_address(uint32_t address, uint8_t alignment)
{
    bool bad_address = false;
    if (address < 0xFFFF0600) {
        if (address > get_memory_size()) {
            fprintf(stderr, "0x%08x :  Adresse inexistante.\n", address);
            bad_address = true;
        }
    } else if (!memory.framebuffer) {
        fprintf(stderr, "0x%08x : Le framebuffer n'est pas activé.\n", address);
        bad_address = true;
    }
    if (address % alignment) {
        fprintf(stderr, "0x%08x : Adresse non alignée.\n", address);
        bad_address = true;
    }

    if (bad_address) {
        if (!exitMask) exit(EXIT_FAILURE);
        return false;
    }
    return true;
}

/* change la section text */
void set_text_section(uint8_t *bytes, size_t size, uint32_t address, uint8_t align)
{
    text.address = address;
    if (address % align != 0) {
        text.address += (address % align);
    }
    text.size = size;
    for (uint32_t i = 0; i < size; i++) {
        set_byte(i+address, bytes[i]);
    }
}

/* change la section data */
void set_data_section(uint8_t *bytes, size_t size, uint32_t address, uint8_t align)
{
    data.address = address;
    if (address % align != 0) {
        data.address += (address % align);
    }
    data.size = size;
    for (uint32_t i = 0; i < size; i++) {
        set_byte(i+address, bytes[i]);
    }
}

/* change la section bss */
void set_bss_section(size_t size, uint32_t address, uint8_t align)
{
    bss.address = address;
    if (address % align != 0) {
        bss.address += (address % align);
    }
    bss.size = size;
}

/* accesseurs des sections text et data */
uint32_t get_text_address()
{
    return text.address;
}

uint32_t get_text_size()
{
    return text.size;
}

uint8_t *get_text_bytes()
{
    return memory.memory + text.address;
}

void set_text_address(uint32_t address)
{
    text.address = address;
}

uint32_t get_data_address()
{
    return data.address;
}

uint32_t get_data_size()
{
    return data.size;
}

uint8_t *get_data_bytes()
{
    return memory.memory + data.address;
}

void set_data_address(uint32_t address)
{
    data.address = address;
}

uint32_t get_text_end()
{
    return text.address + text.size;
}

uint32_t get_data_end()
{
    return data.address + data.size;
}

/* retourne la taille de .bss */
uint32_t get_bss_size()
{
    return bss.size;
}

/* retourne la fin de .bss */
uint32_t get_bss_end()
{
    return bss.address + bss.size;
}

/* determine si value tient sur un octet */
bool is_byte(uint32_t value) {
    return !(value >> 8);
}

/* determine si value tient sur un demi-mot */
bool is_half_word(uint32_t value) {
    return !(value >> 16);
}

/* change la valeur d'un octet en mémoire */
void set_byte(uint32_t address, uint8_t value)
{
    if (check_address(address, 1)) {
        if (address < 0xFFFF0600) {
            memory.memory[address] = value;
        } else {
            memory.framebuffer[address - 0xFFFF0600] = value;
        }
    }
}

/* change la valeur d'un demi-mot en mémoire */
void set_half_word(uint32_t address, uint16_t value)
{
    if (check_address(address, 2)) {
        set_byte(address, value >> 8);
        set_byte(address + 1, value);
    }
}

/* change la valeur d'un mot en mémoire */
void set_word(uint32_t address, uint32_t value)
{
    if (check_address(address, 4)) {
        set_half_word(address, value >> 16);
        set_half_word(address + 2, value);
    }
}

/* place une chaine de charactere en memoire */
void set_n_string(uint32_t address, char *string, uint32_t size)
{
    uint32_t i = 0;
    for (i = 0; i < size; i++) {
        set_byte(address + i, string[i]);
    }
    set_byte(address + i, 0);
}

/* recupere la valeur d'un octet en mémoire */
uint8_t get_byte(uint32_t address)
{
    if (check_address(address, 1)) {
        return (address < 0xFFFF0600) ? memory.memory[address] : memory.framebuffer[address - 0xFFFF0600];
    }
    return 0;
}

/* recupere la valeur d'un demi-mot en mémoire */
uint16_t get_half_word(uint32_t address)
{
    if (check_address(address, 2)) {
        return (get_byte(address) << 8) + get_byte(address + 1);
    }
    return 0;
}

/* recupere la valeur d'un mot en mémoire */
uint32_t get_word(uint32_t address)
{
    if (check_address(address, 4)) {
        return (get_half_word(address) << 16) + get_half_word(address + 2);
    }
    return 0;
}

/* recupere la taile de la mémoire */
uint32_t get_memory_size()
{
    return memory.memory_size;
}

/* recupere un pointeur sur le premier octet du framebuffer */
uint8_t *get_framebuffer()
{
    return memory.framebuffer;
}

/* recupere une chaine de charactere en memoire */
void get_string(uint32_t address, char **string) {
    uint32_t string_length = 0;
    while (get_byte(address + string_length++));
    *string = malloc((string_length + 1) * sizeof(char));
    if (!string) {
        fprintf(stderr, "Erreur allocation get_string.\n");
        exit(EXIT_FAILURE);
    }
    for (uint32_t i = 0; i < string_length; i++) {
        (*string)[i] = get_byte(address + i);
    }
}

/* affiche la mémoire entre 2 adresses */
void display_memory_between(uint32_t address1, uint32_t address2)
{
    switch_exitMask();
    if (check_address(address1, 1) && check_address(address2, 1)) {
        uint32_t offset = address2 - address1;
        for (uint32_t i = 0; i <= offset; i++) {
            if (!(i % 16)) {
                printf("0x%06x: ", address1 + i);
            }
            printf("%02x ", get_byte(address1 + i));
            if (!((i + 1) % 16)) {
                printf("\n");
            }
        }
        if ((offset + 1) % 16) {
            printf("\n");
        }
    }
    switch_exitMask();
}

/* libère la mémoire */
void free_memory()
{
    if (memory.framebuffer) {
        framebuffer_close_display();
    }
    free(memory.memory);
    for (size_t i = 0; i < table_sym.size; i++) {
        free(table_sym.sym[i].nom);
    }
    free(table_sym.sym);
    free_bp();
}

/* charge le fichier file en mémoire + symbole + relocation */
void file_to_memory(char *file)
{
    struct elf_descr *elf;
    /* pour la section text */
    uint32_t text_addr = 0;
    uint8_t *text_bytes = NULL;
    size_t text_size = 0;
    uint8_t text_align = 0;

    /* pour la section data */
    uint32_t data_addr = 0;
    uint8_t *data_bytes = NULL;
    size_t data_size = 0;
    uint8_t data_align = 0;

    /* pour la section bss*/
    uint32_t bss_addr = 0;
    uint8_t bss_align = 0;
    size_t bss_size = 0;

    elf = read_elf(file);
    if (elf == NULL) {
        printf("Erreur de lecture ELF\n");
        exit(EXIT_FAILURE);
    }

    /* on récupere les diférentes section */
    get_text_section(elf, &text_bytes, &text_size, &text_addr, &text_align);
    get_data_section(elf, &data_bytes, &data_size, &data_addr, &data_align);
    get_bss_section(elf, &bss_size, &bss_addr, &bss_align);

    Elf32_Sym *symtab;
    size_t sym_size;
    char *strtab = NULL;
    size_t str_size;
    get_string_table(elf, &strtab, &str_size);
    get_symbol_table(elf, &symtab, &sym_size);


    /* si c'est un fichier éxecutable,
        on récupère le point d'entré et les
        différentes section données */
    if (get_elf_type(elf) == ET_EXEC) {
        set_PC_value(get_entry_point(elf));
        set_text_section(text_bytes, text_size, text_addr, text_align);
        set_data_section(data_bytes, data_size, data_addr, data_align);
        set_bss_section(bss_size, bss_addr, bss_align);
        traduit_table_symboles(symtab, sym_size, strtab, &(table_sym.sym), &(table_sym.size));
    } else if (get_elf_type(elf) == ET_REL) {
    /* si on a un fichier relogeable, on le reloge */
        set_PC_value(get_text_address());
        set_text_section(text_bytes, text_size, get_text_address(), text_align);

        if (get_data_address() == 0) {
            uint32_t nb_pages = 1 + (get_text_end() / 0x1000);
            set_data_address(nb_pages * 0x1000);
        }
        set_data_section(data_bytes, data_size, get_data_address(), data_align);
        set_bss_section(bss_size, get_data_end(), bss_align);
        reloge_symboles(symtab, sym_size, strtab, text.address, data.address, bss.address, &(table_sym.sym), &(table_sym.size));

        reloge_text(elf);
        reloge_data(elf);

    } else { /* sinon erreur ? */
        fprintf(stderr, "Erreur ELF\n");
    }

    close_elf(elf);
}

/* reloge les section text et data */
void reloge_text(struct elf_descr *elf)
{
    Elf32_Rel *text_data = NULL;
    size_t rel_text_size = 0;
    get_rel_text_section(elf, &text_data, &rel_text_size);
    reloge_section(text.address, get_text_bytes(), text_data, rel_text_size, table_sym.sym);
}

void reloge_data(struct elf_descr *elf)
{
    Elf32_Rel *data_data = NULL;
    size_t rel_data_size = 0;
    get_rel_data_section(elf, &data_data, &rel_data_size);
    reloge_section(data.address, get_data_bytes(), data_data, rel_data_size, table_sym.sym);
}

/* recupere un symbole depuis addr */
char* get_sym_from_address(uint32_t addr)
{
    for (size_t i = 0; i < table_sym.size; i++) {
        if (table_sym.sym[i].adresse == addr
        && strcmp(table_sym.sym[i].nom, ".text")
        && strcmp(table_sym.sym[i].nom, ".data")
        && strcmp(table_sym.sym[i].nom, ".bss")
        && strcmp(table_sym.sym[i].nom, "")) {
            return table_sym.sym[i].nom;
        }
    }
    return NULL;
}
