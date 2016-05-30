#ifndef _OPERATIONS_
#define _OPERATIONS_

#include <inttypes.h>
#include <stdbool.h>

// instructions arithmetiques
void add(uint8_t rd, uint8_t rs, uint8_t rt);
void addu(uint8_t rd, uint8_t rs, uint8_t rt);
void addi(uint8_t rt, uint8_t rs, int16_t imm);
void addiu(uint8_t rt, uint8_t rs, int16_t imm);
void add(uint8_t rd, uint8_t rs, uint8_t rt);
void mult(uint8_t rs, uint8_t rt);
void div(uint8_t rs, uint8_t rt);

// instructions logiques
void and(uint8_t rd, uint8_t rs, uint8_t rt);
void or(uint8_t rd, uint8_t rs, uint8_t rt);
void ori(uint8_t rd, uint8_t rs, int16_t imm);
void xor(uint8_t rd, uint8_t rs, uint8_t rt);

// instructions de decalage et set
void sll(uint8_t rd, uint8_t rt, uint8_t sa);
void srl(uint8_t rd, uint8_t rt, uint8_t sa);
void lui(uint8_t rt, uint16_t imm);
void slt(uint8_t rd, uint8_t rs, uint8_t rt);

// instructions r/w memoire
void lw(uint8_t rt, uint32_t base, int16_t offset);

// realise une extension de 16 a 32 bit
uint32_t sign_extend(uint16_t value, bool sign);

#endif /* end of include guard: _OPERATIONS_ */
