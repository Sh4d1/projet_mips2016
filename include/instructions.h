#ifndef _INSTRUCTIONS_
#define _INSTRUCTIONS_

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>


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

struct bp {
    uint32_t address;
    struct bp *next;
};

static struct bp *breakpoint = NULL;

void parse_instruction(uint32_t inst, bool dasm);

void print_R_dasm(uint32_t code, uint8_t rd, uint8_t rs, uint8_t rt, uint8_t sa);

void print_I_J_dasm(uint32_t code, uint8_t rs, uint8_t rt, int16_t imm, uint32_t instr_index);

void run(uint32_t address);

void run_line();

void runi_line();

void dasm_line(uint32_t n);

void dasm();

void add_bp(uint32_t addr);

void rm_bp(uint32_t addr);

void display_bp();

void free_bp();

#endif /* end of include guard: _INSTRUCTIONS_ */
