#include "instructions.h"
#include "include/operations.h"

int main(int argc, char *argv[]) {
    /* code */
    return 0;
}

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

        switch (opcode) { /* I-type instruction */
            case ADDI:
                addi(rt, rs, imm);
                break;
            case ADDIU:
                addiu(rt, rs, imm);
            default:
                /* J-type instruction */
                uint32_t instr_index = inst & 0x3FFFFFF;
                printf("I or J not done yet\n");
        }
    }


}
