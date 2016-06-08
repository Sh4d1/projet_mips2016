#include "../include/instructions.h"
#include "../include/operations.h"
#include "../include/mem.h"
#include "../include/gpr.h"

/* execute ou désassemle l'instruction à l'adresse PC */
void parse_instruction(uint32_t inst, bool dasm)
{
    /* on recupère l'opcode */
    uint32_t opcode = inst >> 26;

    /* on avance PC de 4 */
    advance_PC();

    /* opcode qui vaut 0 donc instruction de type R */
    if (opcode == SPECIAL) {
        /* on recupère func */
        uint32_t func = inst << 26;
        func = func >> 26;

        /* on récupère ts, rt, rd et sa */
        uint8_t rs = (inst >> 21) & 0x1F;
        uint8_t rt = (inst >> 16) & 0x1F;
        uint8_t rd = (inst >> 11) & 0x1F;
        uint8_t sa = (inst >> 6) & 0x1F;

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
                default: /* instruction non implémentée */
                    printf("Instruction de type R non reconnue\n");
            }
        }
    } else { /* cas des instructions I et J */

        /* on récupère rs, rt, imm et instr_index */
        uint8_t rs = (inst >> 21) & 0x1F;
        uint8_t rt = (inst >> 16) & 0x1F;
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
                default: /* instruction non implémentée */
                    printf("Instruction de type I ou J non reconnue\n");
            }
        }
    }
}

/* affiche l'instruction de type R à l'adresse PC */
void print_R_dasm(uint32_t code, uint8_t rd, uint8_t rs, uint8_t rt, uint8_t sa)
{
    /* on affiche la valeur de PC, ainsi que la mémoire à cette adresse */
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

/* affiche l'instruction de type I ou J à l'adresse PC */
void print_I_J_dasm(uint32_t code, uint8_t rs, uint8_t rt, int16_t imm, uint32_t instr_index)
{
    /* on affiche la valeur de PC, ainsi que la mémoire à cette adresse */
    printf("0x%06x:\t%08x\t", get_PC_value()-4, get_word(get_PC_value()-4));
    switch (code) {
        case ADDI:
            printf("ADDI $%s, $%s, %d\n", get_register_name(rt), get_register_name(rs), imm);
            break;
        case ADDIU:
            if (rs == 0) {
                printf("LI $%s, %d\n", get_register_name(rt), imm);
            } else {
                printf("ADDIU $%s, $%s, %d\n", get_register_name(rt), get_register_name(rs), imm);
            }
            break;
        case BEQ:
            if (rs == 0 && rt == 0) {
                printf("B 0x%hx <%s>\n", imm, get_sym_from_address(get_PC_value() + (extend(imm, true) << 2)));
            } else {
                printf("BEQ $%s, $%s, 0x%hx <%s>\n", get_register_name(rs), get_register_name(rt), imm, get_sym_from_address(get_PC_value() + (extend(imm, true) << 2)));
            }
            break;
        case BGTZ:
            printf("BGTZ $%s, 0x%hx <%s>\n", get_register_name(rs), imm, get_sym_from_address(get_PC_value() + (extend(imm, true) << 2)));
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

/* execute le code à l'adresse address, jusqu'à la fin de la section text */
void run(uint32_t address)
{
    /* on charge address dans PC */
    set_PC_value(address);
    /* si il y a un point d'arrêt à cette adresse */
    bool bp_first_addr = false;
    if (is_bp(get_PC_value())) {
        /* on l'execute quand meme */
        bp_first_addr = true;
    }

    /* tant que PC est dans la section text */
    while(get_PC_value() < get_text_end()) {
        /* si on arrive sur un point d'arrêt, qui n'est pas le point de départ */
        if (is_bp(get_PC_value()) && !bp_first_addr) {
            break; /* on rend la main à l'utilisateur */
        }
        /* sinon on execute l'instrution à l'adresse de PC */
        bp_first_addr = false;
        uint32_t word = get_word(get_PC_value());
        parse_instruction(word, false);
    }
}

/* execute l'instruction à l'addresse PC
    si l'on rencontre un JAL, on execute la fonction */
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

/* execute l'instruction à l'addresse PC */
void runi_line()
{
    uint32_t word = get_word(get_PC_value());
    parse_instruction(word, false);
}

/* desassemble n lignes à parir de PC */
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

/* désassemble la section text */
void dasm_text()
{
    if (get_text_size() != 0) {
        uint32_t pc_back = get_PC_value();
        set_PC_value(text.address);
        printf("\nDessasemblage de la section .text : 0x%x\n", get_PC_value());
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
}

/* désassemble la section data */
void dasm_data()
{
    if (get_data_size() != 0) {
        uint32_t pc_back = get_PC_value();
        set_PC_value(data.address);
        printf("\nDessasemblage de la section .data : 0x%x\n", get_PC_value());
        while(get_PC_value() < get_data_end()) {
            char *sym = get_sym_from_address(get_PC_value());
            if (sym) {
                printf("<%s>:\n", sym);
            }
            printf("0x%06x:\t%08x\t\n", get_PC_value(), get_word(get_PC_value()));
            //parse_instruction(word, true);
            advance_PC();
        }
        set_PC_value(pc_back);
    }
}

/* désassemble la section bss */
void dasm_bss()
{
    if (get_bss_size() != 0) {
        uint32_t pc_back = get_PC_value();
        set_PC_value(bss.address);
        printf("\nDessasemblage de la section .bss : 0x%x\n", get_PC_value());
        while(get_PC_value() < get_bss_end()) {
            char *sym = get_sym_from_address(get_PC_value());
            if (sym) {
                printf("<%s>:\n", sym);
                printf("0x%06x:\t%08x\t\n", get_PC_value(), get_word(get_PC_value()));
            }

            //parse_instruction(word, true);
            advance_PC();
        }
        set_PC_value(pc_back);
        printf("...\n");
    }
}

/* ajoute un point d'arrêt à l'adresse addr */
void add_bp(uint32_t addr)
{
    struct bp *new = malloc(sizeof(struct bp));
    if (!new) {
        fprintf(stderr, "Erreur allocation point d'arrêt.\n");
        exit(EXIT_FAILURE);
    }
    new->address = addr;
    new->next = NULL;
    if (!breakpoint) {
        breakpoint = new;
    } else {
        struct bp *tmp = breakpoint;
        while (tmp->next) {
            tmp = tmp->next;
        }
        tmp->next = new;
    }
}

/* supprime le point d'arrêt à l'adresse addr */
void rm_bp(uint32_t addr)
{
    if (!breakpoint) {
        printf("Liste des points d'arrêt vide, pas de suppression possible.\n");
    } else {
        struct bp *tmp = breakpoint;
        struct bp *delete = tmp;
        if (delete->address == addr) {
            breakpoint = breakpoint->next;
        } else {
            while (delete && delete->address != addr) {
                tmp = delete;
                delete = delete->next;
            }
            if (!delete) {
                printf("Element non trouvé dans la liste des points d'arrêt, pas de suppression.\n");
            } else {
                tmp->next = delete->next;
            }
        }
        free(delete);
    }
}

/* affiche les points d'arrêt */
void display_bp()
{
    if (breakpoint) {
        struct bp *tmp = breakpoint;
        uint32_t i = 1;
        while (tmp) {
            printf("Breakpoint n°%d : 0x%08x\n", i, tmp->address);
            tmp = tmp->next;
            i++;
        }
    } else {
        printf("Liste des points d'arrêt vide.\n");
    }
}

/* libere la liste des points d'arrêt */
void free_bp()
{
    struct bp *tmp = breakpoint;
    while (tmp) {
        tmp = tmp->next;
        free(breakpoint);
        breakpoint = tmp;
    }
}

/* determine si une addresse est un point d'arrêt */
bool is_bp(uint32_t addr)
{
    if (!breakpoint) {
        return false;
    } else {
        struct bp *tmp = breakpoint;
        while(tmp) {
            if (tmp->address == addr) {
                return true;
            }
            tmp = tmp->next;
        }
        return false;
    }
}
