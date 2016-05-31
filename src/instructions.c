#include "../include/instructions.h"
#include "../include/operations.h"
#include "../include/mem.h"

// uint8_t function_index[] = { ADD, ADDU, AND, DIV, JR,
//     MFHI, MFLO, MULT, OR, SLL, SRL, SLT, SUB, SYSCALL, XOR}
//
// char *function_str[] = { "ADD", "ADDU", "AND", "DIV", "JR",
//     "MFHI", "MFLO", "MULT", "OR", "SLL", "SRL", "SLT", "SUB",
//     "SYSCALL", "XOR"}




void parse_instruction(uint32_t inst, bool dasm)
{
    // on récupère l'opcode
    uint32_t opcode = inst >> 26;

    /* opcode qui vaut 0 donc instruction de type R */
    if (opcode == SPECIAL) {
        /* on recupère func */
        uint32_t func = inst << 26;
        func = func >> 26;

        /* on récupère ts, rt, rd et sa */
        uint8_t rs = (inst>>21) & 0x1F;
        uint8_t rt = (inst>>16) & 0x1F;
        uint8_t rd = (inst>>11) & 0x1F;
        uint8_t sa = (inst>>6) & 0x1F;

        /* si on veut juste désassembler */
        if (dasm) {
            print_R_dasm(func, rd, rs, rt, sa);
        } else { /* sinon on regarde le champ func */
            switch (func) {
                case ADD:
                    add(rd, rs, rt);
                    break;
                case ADDU:
                    addu(rd, rs, rt);
                    break;
                case AND:
                    and(rd, rs, rt);
                    break;
                case DIV:
                    divi(rs, rt);
                    break;
                case JR:
                    jr(rs);
                    break;
                case MFHI:
                    mfhi(rd);
                    break;
                case MFLO:
                    mflo(rd);
                    break;
                case MULT:
                    mult(rs, rt);
                    break;
                case OR:
                    or(rd, rs, rt);
                    break;
                case SLL:
                    sll(rd, rt, sa);
                    break;
                case SLT:
                    slt(rd, rs, rt);
                    break;
                case SRL:
                    srl(rd, rt, sa);
                    break;
                case SUB:
                    sub(rd, rs, rt);
                    break;
                case SYSCALL:
                    syscall();
                    break;
                case XOR:
                    xor(rd, rs, rt);
                    break;
                default:
                    printf("Instruction de type R non reconnue\n");
            }
        }


    } else { /* cas des instructions I et J */

        /* on récupère rs, rt, imm et instr_index */
        uint8_t rs = (inst>>21) & 0x1F;
        uint8_t rt = (inst>>16) & 0x1F;
        int16_t imm = inst & 0xFFFF;
        uint32_t instr_index = (inst & 0x3FFFFFF);

        /* si on veut juste désassembler */
        if (dasm) {
            print_I_J_dasm(opcode, rs, rt, imm, instr_index);
        } else { /* sinon on regarde opcode */
            switch (opcode) { /* traitons les types I en premier */
                case ADDI:
                    addi(rt, rs, imm);
                    break;
                case ADDIU:
                    addiu(rt, rs, imm);
                    break;
                case BEQ:
                    beq(rs, rt, imm);
                    break;
                case BGTZ:
                    bgtz(rs, imm);
                    break;
                case BLEZ:
                    blez(rs, imm);
                    break;
                case BNE:
                    bne(rs, rt, imm);
                    break;
                case LB:
                    lb(rt, rs, imm);
                    break;
                case LBU:
                    lbu(rt, rs, imm);
                    break;
                case LUI:
                    lui(rt, imm);
                    break;
                case LW:
                    lw(rt, rs, imm);
                    break;
                case ORI:
                    ori(rt, rs, imm);
                    break;
                case SB:
                    sb(rt, rs, imm);
                    break;
                case SW:
                    sw(rt, rs, imm);
                    break;
                default: /* les types I ont été traité, passons aux J
                    /* J-type instruction */
                    printf("I or J not done yet\n");
            }
        }
    }
}

void print_R_dasm(uint32_t code, uint8_t rd, uint8_t rs, uint8_t rt, uint8_t sa)
{
    printf("0x%06x:\t%08x\t", get_PC_value(), get_word(get_PC_value()));
    switch (code) {
        case ADD:
            printf("ADD $%u, $%u, $%u\n", rd, rs, rt);
            break;
        case AND:
            printf("AND $%u, $%u, $%u\n", rd, rs, rt);
            break;
        case DIV:
            printf("DIV $%u, $%u\n",rs, rt);
            break;
        case JR:
            printf("JR $%u\n", rs);
            break;
        case MFHI:
            printf("MFHI $%u\n", rd);
            break;
        case MFLO:
            printf("MFLO $%u\n", rd);
            break;
        case MULT:
            printf("MULT $%u, $%u\n", rs, rt);
            break;
        case OR:
            printf("OR $%u, $%u, $%u\n", rd, rs, rt);
            break;
        case SLL:
            printf("SLL $%u, $%u, %u\n", rd, rt, sa);
            break;
        case SLT:
            printf("SLT $%u, $%u, $%u\n", rd, rs, rt);
            break;
        case SRL:
            printf("SRL $%u, $%u, %u\n", rd, rt, sa);
            break;
        case SUB:
            printf("SUB $%u, $%u, $%u\n", rd, rs, rt);
            break;
        case SYSCALL:
            printf("SYSCALL\n");
            break;
        case XOR:
            printf("XOR $%u, $%u, $%u\n", rd, rs, rt);
            break;
        default:
            printf("R %u\n", code);
    }
}

void print_I_J_dasm(uint32_t code, uint8_t rs, uint8_t rt, int16_t imm, uint32_t instr_index)
{
    printf("0x%06x:\t%08x\t", get_PC_value(), get_word(get_PC_value()));
    switch (code) {
        case ADDI:
            printf("ADDI $%u, $%u, %u\n", rt, rs, imm);
            break;
        case ADDIU:
            printf("ADDIU $%u, $%u, %u\n", rt, rs, imm);
            break;
        case BEQ:
            printf("BEQ $%u, $%u, %u\n", rs, rt, imm);
            break;
        case BGTZ:
            printf("BGTZ $%u, %u\n", rs, imm);
            break;
        case BLEZ:
            printf("BLEZ $%u, %u\n", rs, imm);
            break;
        case BNE:
            printf("BNE $%u, $%u, %u\n", rs, rt, imm);
            break;
        case LB:
            printf("LB $%u, %u($%u)\n", rt, imm, rs);
            break;
        case LBU:
            printf("LBU $%u, %u($%u)\n", rt, imm, rs);
            break;
        case LUI:
            printf("LUI $%u, %u\n", rt, imm);
            break;
        case LW:
            printf("LW $%u, %u($%u)\n", rt, imm, rs);
            break;
        case ORI:
            printf("ORI $%u, $%u, %u\n", rt, rs, imm);
            break;
        case SB:
            printf("SB $%u, %u($%u)\n", rt, imm, rs);
            break;
        case SW:
            printf("SW $%u, %u($%u)\n", rt, imm, rs);
            break;
        default:
            printf("IJ %u\n", code);
    }
}

void run()
{
    set_PC_value(0);
    while(get_word(get_PC_value()) != 0) {
        //printf("%u\n", get_PC_value());
        uint32_t word = get_word(get_PC_value());
        parse_instruction(word, false);
        set_PC_value(get_PC_value()+4);
    }
    set_PC_value(0);
}

void dasm()
{
    while (get_word(get_PC_value()) != 0) {
        //printf("%u\n", get_PC_value());
        uint32_t word = get_word(get_PC_value());
        parse_instruction(word, true);
        set_PC_value(get_PC_value()+4);
    }
}
