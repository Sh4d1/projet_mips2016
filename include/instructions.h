#ifndef _INSTRUCTIONS_
#define _INSTRUCTIONS_

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief Module de gestion des instructions, du désassemblage et des points d'arrêt
 */

/* indices du champ function */
enum function {
    ADD = 32,
    ADDU = 33,
    AND = 36,
    DIV = 26,
    JR = 8,
    MFHI = 16,
    MFLO = 18,
    MULT = 24,
    OR = 37,
    SLL = 0,
    SRL = 2,
    SLT = 42,
    SUB = 34,
    SYSCALL = 12,
    XOR = 38
};

/* indices du champ opcode */
enum opcode {
    SPECIAL = 0,
    ADDI = 8,
    ADDIU = 9,
    BEQ = 4,
    BGTZ = 7,
    BLEZ = 6,
    BNE = 5,
    J = 2,
    JAL = 3,
    LB = 32,
    LBU = 36,
    LUI = 15,
    LW = 35,
    ORI = 13,
    SB = 40,
    SW = 43
};

/* structure d'une liste de points d'arrêt */
struct bp {
    uint32_t address;
    struct bp *next;
};

/* la liste de point d'arrêt */
static struct bp *breakpoint = NULL;

/* permet d'executer ou de désassembler une instruction */
void parse_instruction(uint32_t inst, bool dasm);

/* affiche une instruction de type R désassemblée */
void print_R_dasm(uint32_t code, uint8_t rd, uint8_t rs, uint8_t rt, uint8_t sa);

/* affiche une instruction de type I ou J désassemblée */
void print_I_J_dasm(uint32_t code, uint8_t rs, uint8_t rt, int16_t imm, uint32_t instr_index);

/* execute le programme à partir de address */
void run(uint32_t address);

/* execute une ligne à partir de PC, saute un JAL */
void run_line();

/* execute une ligne à partir de PC, sans sauter de JAL */
void runi_line();

/* désassemble n lignes */
void dasm_line(uint32_t n);

/* désassemble la partie .text */
void dasm_text();

/* désassemble la partie .data */
void dasm_data();

/* désassemble la partie .bss */
void dasm_bss();

/* ajoute un point d'arrêt à l'adresse addr */
void add_bp(uint32_t addr);

/* supprime le point d'arrêt à l'adresse addr */
void rm_bp(uint32_t addr);

/* affiche les points d'arrêt */
void display_bp();

/* supprime la liste des points d'arrêt */
void free_bp();

/* determine si une adresse est aussi un point d'arrêt */
bool is_bp(uint32_t addr);

#endif /* end of include guard: _INSTRUCTIONS_ */
