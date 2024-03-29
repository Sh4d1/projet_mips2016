#include <stdio.h>
#include <stdlib.h>

#include "../include/mem.h"
#include "../include/operations.h"
#include "../include/gpr.h"

// realise une extension de 16 a 32 bits */
uint32_t extend(uint16_t value, bool sign)
{
        uint8_t bitfort = value >> 15;
        return (sign && bitfort) ? 0xFFFF0000 | value : value;
}

/* renvoie le nieme bit en partant de 0 avec decalage */
uint64_t get_bit(uint64_t value, uint8_t n, uint8_t pos)
{
        return ((value >> n) & 1) << pos;
}

/* leve l'exception de depassement */
void overflow(uint32_t value1, uint32_t value2, bool sub)
{
        uint64_t x = value1;
        uint64_t y = value2;
        uint64_t res;
        x = get_bit(x, 31, 32) | x;
        y = get_bit(y, 31, 32) | y;
        res = (sub) ? x - y : x + y;
        if (get_bit(res, 32, 0) ^ get_bit(res, 31, 0)) {
                fprintf(stderr, "Dépassement.\n");
                exit(EXIT_FAILURE);
        }
}

/* instructions arithmetiques */
void add(uint8_t rd, uint8_t rs, uint8_t rt)
{
        // exception depacement
        overflow(get_register_value(rs), get_register_value(rt), false);
        addu(rd, rs, rt);
}

void addu(uint8_t rd, uint8_t rs, uint8_t rt)
{
        set_register_value(rd, get_register_value(rs) + get_register_value(rt));
}

void addi(uint8_t rt, uint8_t rs, uint16_t imm)
{
        // exception depacement
        overflow(get_register_value(rs), extend(imm, true), false);
        addiu(rt, rs, imm);
}

void addiu(uint8_t rt, uint8_t rs, uint16_t imm)
{
        set_register_value(rt, get_register_value(rs) + extend(imm, true));
}

void sub(uint8_t rd, uint8_t rs, uint8_t rt)
{
        // exception depacement
        overflow(get_register_value(rs), get_register_value(rt), true);
        set_register_value(rd, get_register_value(rs) - get_register_value(rt));
}

void mult(uint8_t rs, uint8_t rt)
{
        uint64_t res = (uint64_t)get_register_value(rs) * (uint64_t)get_register_value(rt);
        set_LO_value(res);
        set_HI_value(res >> 32);
}

void divi(uint8_t rs, uint8_t rt)
{
        if (get_register_value(rt) != 0) {
            set_LO_value(get_register_value(rs) / get_register_value(rt));
            set_HI_value(get_register_value(rs) % get_register_value(rt));
        } else {
            fprintf(stderr, "Erreur : division par 0. Fin du programme.\n");
            exit(EXIT_FAILURE);
        }

}

/* instructions logiques */
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

/* instructions de decalage et set */
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

/* instructions r/w memoire */
void lw(uint8_t rt, uint8_t base, uint16_t offset)
{
        set_register_value(rt, get_word(get_register_value(base) + extend(offset, true)));
}

void sw(uint8_t rt, uint8_t base, uint16_t offset)
{
        set_word(get_register_value(base) + extend(offset, true), get_register_value(rt));
}

void lb(uint8_t rt, uint8_t base, uint16_t offset)
{
        set_register_value(rt, extend(get_byte(get_register_value(base) + extend(offset, true)), true));
}

void lbu(uint8_t rt, uint8_t base, uint16_t offset)
{
        set_register_value(rt, get_byte(get_register_value(base) + extend(offset, true)));
}

void sb(uint8_t rt, uint8_t base, uint16_t offset)
{
        set_byte(get_register_value(base) + extend(offset, true), get_register_value(rt));
}

void mfhi(uint8_t rd) {
        set_register_value(rd, get_HI_value());
}

void mflo(uint8_t rd) {
        set_register_value(rd, get_LO_value());
}

/* instructions de branchement, saut et controle */
void b(uint16_t offset)
{
        set_PC_value(get_PC_value() + (extend(offset, true) << 2));
}

void beq(uint8_t rs, uint8_t rt, uint16_t offset)
{
        if (get_register_value(rs) == get_register_value(rt)) {
                b(offset);
        }
}

void bne(uint8_t rs, uint8_t rt, uint16_t offset)
{
        if (get_register_value(rs) != get_register_value(rt)) {
                b(offset);
        }
}

void bgtz(uint8_t rs, uint16_t offset)
{
        if ((int32_t)get_register_value(rs) > 0) {
                b(offset);
        }
}

void blez(uint8_t rs, uint16_t offset)
{
        if ((int32_t)get_register_value(rs) <= 0) {
                b(offset);
        }
}

void j(uint32_t instr_index)
{
        set_PC_value((0xF0000000 & get_PC_value()) | (instr_index << 2));
}

void jal(uint32_t instr_index)
{
        set_register_value(RA, get_PC_value() + 4);
        j(instr_index);
}

void jr(uint8_t rs)
{
        set_PC_value(get_register_value(rs));
}

void syscall()
{
        uint8_t service_code = get_register_value(V0);
        switch (service_code) {
        case 1:
            printf("%d\n", get_register_value(A0));
            break;
        case 4: {
            char *string = NULL;
            get_string(get_register_value(A0), &string);
            printf("%s\n", string);
            free(string);
            break;
        }
        case 5: {
            uint32_t integer = 0;
            char line[20];
            fgets(line, sizeof(line), stdin);
            sscanf(line, "%d", &integer);
            set_register_value(V0, integer);
            break;
        }
        case 8: {
            char line[100];
            char max_buffer[100];
            fgets(line, sizeof(line), stdin);
            sscanf(line, "%s", max_buffer);
            set_n_string(get_register_value(A0), max_buffer, get_register_value(A1));
            break;
        }
        case 10:
            exit(EXIT_SUCCESS);
            break;
        default:
            printf("Code de service inconnu.\n");
            exit(EXIT_FAILURE);
        }
}

/* pseudo-instructions */
void nop()
{
        sll(0, 0, 0);
}

void move(uint8_t rt, uint8_t rs)
{
        addu(rt, rs, 0);
}
