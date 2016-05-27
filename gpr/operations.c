#include "operations.h"
#include "gpr.h"

// add
void add(uint8_t rd, uint8_t rs, uint8_t rt)
{
        // TODO lever exception depacement
        addu(rd, rs, imm);
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
