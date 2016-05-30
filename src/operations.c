#include "../include/mem.h"
#include "../include/operations.h"
#include "../include/gpr.h"

// instructions arithmetiques
void add(uint8_t rd, uint8_t rs, uint8_t rt)
{
        // TODO lever exception depacement
        addu(rd, rs, rt);
}

void addu(uint8_t rd, uint8_t rs, uint8_t rt)
{
        set_register_value(rd, get_register_value(rs) + get_register_value(rt));
}

void addi(uint8_t rt, uint8_t rs, uint16_t imm)
{
        // TODO lever exception depacement
        addiu(rt, rs, imm);
}

void addiu(uint8_t rt, uint8_t rs, uint16_t imm)
{
        set_register_value(rt, get_register_value(rs) + extend(imm, true));
}

void sub(uint8_t rd, uint8_t rs, uint8_t rt)
{
        // TODO lever exception depacement
        set_register_value(rd, get_register_value(rs) - get_register_value(rt));
}

void mult(uint8_t rs, uint8_t rt)
{
        uint64_t res = get_register_value(rs) * get_register_value(rt);
        set_LO_value(res);
        set_HI_value(res >> 32);
}

void div(uint8_t rs, uint8_t rt)
{
        set_LO_value(get_register_value(rs) / get_register_value(rt));
        set_HI_value(get_register_value(rs) % get_register_value(rt));
        return;
}

// instructions logiques
void and(uint8_t rd, uint8_t rs, uint8_t rt)
{
        set_register_value(rd, get_register_value(rs) & get_register_value(rt));
}

void or(uint8_t rd, uint8_t rs, uint8_t rt)
{
        set_register_value(rd, get_register_value(rs) | get_register_value(rt));
}

void ori(uint8_t rd, uint8_t rs, uint16_t imm)
{
        set_register_value(rd, get_register_value(rs) | extend(imm, false));
}

void xor(uint8_t rd, uint8_t rs, uint8_t rt)
{
        set_register_value(rd, get_register_value(rs) ^ get_register_value(rt));
}

// instructions de decalage et set
void sll(uint8_t rd, uint8_t rt, uint8_t sa)
{
        set_register_value(rd, get_register_value(rt) << sa);
}

void srl(uint8_t rd, uint8_t rt, uint8_t sa)
{
        set_register_value(rd, get_register_value(rt) >> sa);
}

void lui(uint8_t rt, uint16_t imm)
{
        set_register_value(rt, imm << 16);
}

void slt(uint8_t rd, uint8_t rs, uint8_t rt)
{
        set_register_value(rd, get_register_value(rs) < get_register_value(rt));
}

// instructions r/w memoire
void lw(uint8_t rt, uint8_t base, int16_t offset)
{
        set_register_value(rt, get_word(get_register_value(base) + extend(offset, true)));
}

// realise une extension de 16 a 32 bit
uint32_t extend(uint16_t value, bool sign)
{
        uint8_t bitfort = value >> 15;
        if (sign && bitfort) {
                return 0xFFFF0000 | value;
        } else {
                return value;
        }
}
