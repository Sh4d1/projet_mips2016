#include "../include/instructions.h"
#include "../include/operations.h"
#include "../include/mem.h"
#include "../include/gpr.h"

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

    advance_PC();

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
                /* les types I ont été traités, passons aux J */
                /* J-type instruction */
                case J:
                    j(instr_index);
                    break;
                case JAL:
                    jal(instr_index);
                    break;
                default:
                    printf("I or J not recongnized opcode : %u\n", opcode);
            }
        }
    }
}

void print_R_dasm(uint32_t code, uint8_t rd, uint8_t rs, uint8_t rt, uint8_t sa)
{
    printf("0x%06x:\t%08x\t", get_PC_value()-4, get_word(get_PC_value()-4));
    switch (code) {
        case ADD:
            printf("ADD $%s, $%s, $%s\n", get_register_name(rd), get_register_name(rs), get_register_name(rt));
            break;
        case ADDU:
            printf("ADDU $%s, $%s, $%s\n", get_register_name(rd), get_register_name(rs), get_register_name(rt));
            break;
        case AND:
            printf("AND $%s, $%s, $%s\n", get_register_name(rd), get_register_name(rs), get_register_name(rt));
            break;
        case DIV:
            printf("DIV $%s, $%s\n",get_register_name(rs), get_register_name(rt));
            break;
        case JR:
            printf("JR $%s\n", get_register_name(rs));
            break;
        case MFHI:
            printf("MFHI $%s\n", get_register_name(rd));
            break;
        case MFLO:
            printf("MFLO $%s\n", get_register_name(rd));
            break;
        case MULT:
            printf("MULT $%s, $%s\n", get_register_name(rs), get_register_name(rt));
            break;
        case OR:
            if (rt == 0) {
                printf("MOVE $%s, $%s\n", get_register_name(rd), get_register_name(rs));
            } else {
                printf("OR $%s, $%s, $%s\n", get_register_name(rd), get_register_name(rs), get_register_name(rt));
            }
            break;
        case SLL:
            if (rd == 0 && rt == 0 && sa == 0) {
                printf("NOP\n");
            } else {
                printf("SLL $%s, $%s, %u\n", get_register_name(rd), get_register_name(rt), sa);
            }
            break;
        case SLT:
            printf("SLT $%s, $%s, $%s\n", get_register_name(rd), get_register_name(rs), get_register_name(rt));
            break;
        case SRL:
            printf("SRL $%s, $%s, %u\n", get_register_name(rd), get_register_name(rt), sa);
            break;
        case SUB:
            printf("SUB $%s, $%s, $%s\n", get_register_name(rd), get_register_name(rs), get_register_name(rt));
            break;
        case SYSCALL:
            printf("SYSCALL\n");
            break;
        case XOR:
            printf("XOR $%s, $%s, $%s\n", get_register_name(rd), get_register_name(rs), get_register_name(rt));
            break;
        default:
            printf("%x\n", get_word(get_PC_value()-4));
    }
}

void print_I_J_dasm(uint32_t code, uint8_t rs, uint8_t rt, int16_t imm, uint32_t instr_index)
{
    printf("0x%06x:\t%08x\t", get_PC_value()-4, get_word(get_PC_value()-4));
    switch (code) {
        case ADDI:
            printf("ADDI $%s, $%s, %d\n", get_register_name(rt), get_register_name(rs), imm);
            break;
        case ADDIU:
            printf("ADDIU $%s, $%s, %u\n", get_register_name(rt), get_register_name(rs), imm);
            break;
        case BEQ:
            if (rs == 0 && rt == 0) {
                printf("B 0x%hx <%s>\n", imm, get_sym_from_address(get_PC_value() + (extend(imm, true) << 2)));
            } else {
                printf("BEQ $%s, $%s, 0x%hx <%s>\n", get_register_name(rs), get_register_name(rt), imm, get_sym_from_address(get_PC_value() + (extend(imm, true) << 2)));
            }
            break;
        case BGTZ:
            printf("BGTZ $%s, 0X%hx <%s>\n", get_register_name(rs), imm, get_sym_from_address(get_PC_value() + (extend(imm, true) << 2)));
            break;
        case BLEZ:
            printf("BLEZ $%s, 0x%hx <%s>\n", get_register_name(rs), imm, get_sym_from_address(get_PC_value() + (extend(imm, true) << 2)));
            break;
        case BNE:
            printf("BNE $%s, $%s, 0x%hx <%s>\n", get_register_name(rs), get_register_name(rt), imm, get_sym_from_address(get_PC_value() + (extend(imm, true) << 2)));
            break;
        case LB:
            printf("LB $%s, %d($%s)\n", get_register_name(rt), imm, get_register_name(rs));
            break;
        case LBU:
            printf("LBU $%s, %d($%s)\n", get_register_name(rt), imm, get_register_name(rs));
            break;
        case LUI:
            printf("LUI $%s, 0x%hx\n", get_register_name(rt), imm);
            break;
        case LW:
            printf("LW $%s, %d($%s)\n", get_register_name(rt), imm, get_register_name(rs));
            break;
        case ORI:
            printf("ORI $%s, $%s, 0x%hx\n", get_register_name(rt), get_register_name(rs), imm);
            break;
        case SB:
            printf("SB $%s, %d($%s)\n", get_register_name(rt), imm, get_register_name(rs));
            break;
        case SW:
            printf("SW $%s, %d($%s)\n", get_register_name(rt), imm, get_register_name(rs));
            break;
        case J:
            printf("J 0x%x <%s>\n", (0xF0000000 & get_PC_value()) | (instr_index << 2), get_sym_from_address((0xF0000000 & get_PC_value()) | (instr_index << 2)));
            break;
        case JAL:
            printf("JAL 0x%x <%s>\n", (0xF0000000 & get_PC_value()) | (instr_index << 2), get_sym_from_address((0xF0000000 & get_PC_value()) | (instr_index << 2)));
            break;
        default:
            printf("%x\n", get_word(get_PC_value()-4));
    }
}

void run(uint32_t address)
{
    set_PC_value(address);
    while(get_PC_value() < get_text_end()) {
        uint32_t word = get_word(get_PC_value());
        parse_instruction(word, false);
    }
}

void run_line()
{
    uint32_t pc_back = get_PC_value();
    uint32_t word = get_word(get_PC_value());
    parse_instruction(word, false);
    if (pc_back + 8 == get_register_value(RA)) {
        while (get_PC_value() != pc_back + 8) {
            word = get_word(get_PC_value());
            parse_instruction(word, false);
        }
    }


}

void runi_line()
{
    uint32_t word = get_word(get_PC_value());
    parse_instruction(word, false);
}

void dasm_line(uint32_t n)
{
    uint32_t word;
    uint32_t pc_back = get_PC_value();
    for (uint32_t i = 0; i < n; i++) {
        char *sym = get_sym_from_address(get_PC_value());
        if (sym) {
            printf("<%s>\n", sym);
        }
        word = get_word(get_PC_value());
        parse_instruction(word, true);
    }
    set_PC_value(pc_back);
}


void dasm_text()
{
    uint32_t pc_back = get_PC_value();
    printf("\nDessasemblage de la section .text\n");
    set_PC_value(text.address);
    while(get_PC_value() < get_text_end()) {
        uint32_t word = get_word(get_PC_value());
        char *sym = get_sym_from_address(get_PC_value());
        if (sym) {
            printf("\n<%s>:\n", sym);
        }
        parse_instruction(word, true);
    }
    set_PC_value(pc_back);
}

void dasm_data()
{
    if (get_data_size() != 0) {
        uint32_t pc_back = get_PC_value();
        printf("\nDessasemblage de la section .data\n");
        set_PC_value(data.address);
        while(get_PC_value() < get_data_end()) {
            uint32_t word = get_word(get_PC_value());
            char *sym = get_sym_from_address(get_PC_value());
            if (sym) {
                printf("<%s>:\n", sym);
            }
            parse_instruction(word, true);
        }
        set_PC_value(pc_back);
    }
}
