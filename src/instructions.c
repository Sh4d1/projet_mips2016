#include "../include/instructions.h"
#include "../include/operations.h"



void parse_instruction(uint32_t inst)
{
    uint32_t opcode = inst >> 26;
    if (opcode == SPECIAL) { /* R-type instruction */
        uint32_t func = inst << 26;
        func = func >> 26;

        uint8_t rs = (inst>>21) & 0x1F;
        uint8_t rt = (inst>>16) & 0x1F;
        uint8_t rd = (inst>>11) & 0x1F;
        uint8_t sa = (inst>>6) & 0x1F;

        print_R_dasm(func, rd, rs, rt, sa);
        switch (func) {
            case ADD:
                add(rd, rs, rt);
                break;
            case ADDU:
                addu(rd, rs, rt);
                break;
            default:
                printf("Instruction de type R non reconnue\n");
        }
    } else { /* I or J type instruction */
        uint8_t rs = (inst>>21) & 0x1F;
        uint8_t rt = (inst>>16) & 0x1F;
        int16_t imm = inst & 0xFFFF;
        uint32_t instr_index = (inst & 0x3FFFFFF);

        print_I_J_dasm(opcode, rs, rt, imm, instr_index);

        switch (opcode) { /* I-type instruction */
            case ADDI:
                addi(rt, rs, imm);
                break;
            case ADDIU:
                addiu(rt, rs, imm);
            default:
                /* J-type instruction */
                printf("I or J not done yet\n");
        }
    }


}

void print_R_dasm(uint32_t code, uint8_t rd, uint8_t rs, uint8_t rt, uint8_t sa)
{
    switch (code) {
        case ADD:
            printf("ADD $%u, $%u, $%u\n", rd, rs, rt);
            break;

        default:
            printf("R not finished :) \n");
    }
}

void print_I_J_dasm(uint32_t code, uint8_t rs, uint8_t rt, int16_t imm, uint32_t instr_index)
{
    switch (code) {
        case ADDI:
            printf("ADDI $%u, $%u, %u\n", rt, rs, imm);
            break;
        default:
            printf("I or J not finished :) \n");
    }
}
