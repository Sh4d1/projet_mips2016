#include "../include/operations.h"
#include "../include/gpr.h"

// add
void add(uint8_t rd, uint8_t rs, uint8_t rt)
{
        // TODO lever exception depacement
        addu(rd, rs, rt);
}

// addu
void addu(uint8_t rd, uint8_t rs, uint8_t rt)
{
        GPR[rd].value = GPR[rs].value + GPR[rt].value;
}

// addi
void addi(uint8_t rt, uint8_t rs, uint16_t imm)
{
        // TODO lever exception depacement
        addiu(rt, rs, imm);
}

// addiu
void addiu(uint8_t rt, uint8_t rs, uint16_t imm)
{
        GPR[rt].value = GPR[rs].value + sign_extend(imm, true);
}

// sub
void sub(uint8_t rd, uint8_t rs, uint8_t rt)
{
        // TODO lever exception depacement
        GPR[rd].value = GPR[rs].value - GPR[rt].value;
}

// mult / div
void mult(uint8_t rs, uint8_t rt)
{
        return;
}

void div(uint8_t rs, uint8_t rt)
{
        return;
}

// realise une extension de 16 a 32 bit
uint32_t sign_extend(uint16_t value, bool sign)
{
        uint8_t bitfort = (0x8000 & value) >> 15;
        if (sign && bitfort) {
                return 0xFFFF0000 | value;
        } else {
                return 0x00000000 | value;
        }
}
