#ifndef _OPERATIONS_
#define _OPERATIONS_

#include <inttypes.h>
#include <stdbool.h>

/* realise une extension de 16 a 32 bits */
uint32_t extend(uint16_t value, bool sign);

/* renvoie le nieme bit en partant de 0 avec decalage */
uint64_t get_bit(uint64_t value, uint8_t n, uint8_t pos);

/* leve l'exception de depassement */
void overflow(uint32_t value1, uint32_t value2, bool sub);

/* instructions arithmetiques */
void add(uint8_t rd, uint8_t rs, uint8_t rt);
void addu(uint8_t rd, uint8_t rs, uint8_t rt);
void addi(uint8_t rt, uint8_t rs, uint16_t imm);
void addiu(uint8_t rt, uint8_t rs, uint16_t imm);
void sub(uint8_t rd, uint8_t rs, uint8_t rt);
void mult(uint8_t rs, uint8_t rt);
void divi(uint8_t rs, uint8_t rt);

/* instructions logiques */
void and(uint8_t rd, uint8_t rs, uint8_t rt);
void or(uint8_t rd, uint8_t rs, uint8_t rt);
void ori(uint8_t rd, uint8_t rs, uint16_t imm);
void xor(uint8_t rd, uint8_t rs, uint8_t rt);

/* instructions de decalage et set */
void sll(uint8_t rd, uint8_t rt, uint8_t sa);
void srl(uint8_t rd, uint8_t rt, uint8_t sa);
void lui(uint8_t rt, uint16_t imm);
void slt(uint8_t rd, uint8_t rs, uint8_t rt);

/* instructions r/w memoire */
void lw(uint8_t rt, uint8_t base, uint16_t offset);
void sw(uint8_t rt, uint8_t base, uint16_t offset);
void lb(uint8_t rt, uint8_t base, uint16_t offset);
void lbu(uint8_t rt, uint8_t base, uint16_t offset);
void sb(uint8_t rt, uint8_t base, uint16_t offset);
void mfhi(uint8_t rd);
void mflo(uint8_t rd);

/* instructions de branchement, saut et controle */
void b(uint16_t offset);
void beq(uint8_t rs, uint8_t rt, uint16_t offset);
void bne(uint8_t rs, uint8_t rt, uint16_t offset);
void bgtz(uint8_t rs, uint16_t offset);
void blez(uint8_t rs, uint16_t offset);
void j(uint32_t instr_index);
void jal(uint32_t instr_index);
void jr(uint8_t rs);
void syscall();

/* pseudo-instructions */
void nop();
void move(uint8_t rt, uint8_t rs);
void li15(uint8_t rt, uint32_t imm);
void li16(uint8_t rt, uint32_t imm);
void li32(uint8_t rt, uint32_t imm);
void la();

#endif /* end of include guard: _OPERATIONS_ */
