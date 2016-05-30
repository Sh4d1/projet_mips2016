#include "instructions.h"
#include "include/operations.h"

int main(int argc, char *argv[]) {
    /* code */
    return 0;
}

void parse_instruction(uint32_t inst)
{
    if ((inst >> 26) == SPECIAL) { /* R-type instruction */
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

            default:
        }


    }


}
