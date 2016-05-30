#ifndef _INSTRUCTIONS_
#define _INSTRUCTIONS_

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>


enum function = {
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

enum opcode = {
    SPECIAL = 0,
    ADDI = 16,
    ADDIU = 17,
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




#endif /* end of include guard: _INSTRUCTIONS_ */
