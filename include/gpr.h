#ifndef _GPR_
#define _GPR_

#include <inttypes.h>

#define GPR_LENGTH 32
#define ZERO 0
#define AT 1
#define V0 2
#define V1 3
#define A0 4
#define A1 5
#define A2 6
#define A3 7
#define T0 8
#define T1 9
#define T2 10
#define T3 11
#define T4 12
#define T5 13
#define T6 14
#define T7 15
#define S0 16
#define S1 17
#define S2 18
#define S3 19
#define S4 20
#define S5 21
#define S6 22
#define S7 23
#define T8 24
#define T9 25
#define K0 26
#define K1 27
#define GP 28
#define SP 29
#define FP 30
#define RA 31


static const char* const reg_names[] = {        "zero",
                                                "at",
                                                "v0", "v1",
                                                "a0", "a1", "a2", "a3",
                                                "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
                                                "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
                                                "t8", "t9",
                                                "k0", "k1",
                                                "gp",
                                                "sp",
                                                "fp",
                                                "ra"                                            };

struct reg {
        char name[4];
        uint32_t value;
};

// 32 registres
struct reg GPR[GPR_LENGTH];
// registres speciaux
struct reg PC, HI, LO;

// initialise les registres
void init_GPR();

/* verifie la validite d'un registre */
void check_register(uint8_t index);

// affiche tous les registres
void print_gpr();

void print_a_gpr(char *name);

// retourne l'index du registre
uint8_t get_register_index(char *name);

// retourne la valeur d'un registre
uint32_t get_register_value(uint8_t index);
uint32_t get_register_value_by_name(char *name);
uint32_t get_PC_value();
uint32_t get_HI_value();
uint32_t get_LO_value();

// ecrit une valeur dans un registre
void set_register_value(uint8_t index, uint32_t value);
void set_register_value_by_name(char *name, uint32_t value);
void set_PC_value(uint32_t value);
void set_HI_value(uint32_t value);
void set_LO_value(uint32_t value);

void advance_PC();

uint32_t isNumeric(char *s);

#endif /* end of include guard: _GPR_ */
