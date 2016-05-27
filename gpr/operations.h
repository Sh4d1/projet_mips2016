#ifndef _OPERATIONS_
#define _OPERATIONS_

#include <stdint.h>
#include <stdbool.h>

// famille add
void add(uint8_t rd, uint8_t rs, uint8_t rt);
void addu(uint8_t rd, uint8_t rs, uint8_t rt);
void addi(uint8_t rt, uint8_t rs, uint16_t imm);
void addiu(uint8_t rt, uint8_t rs, uint16_t imm);

// realise une extension de 16 a 32 bit
uint32_t sign_extend(uint16_t value, bool sign);

#endif /* end of include guard: _OPERATIONS_ */
