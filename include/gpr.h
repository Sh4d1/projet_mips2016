#ifndef _GPR_
#define _GPR_

#include <inttypes.h>
#include <stdbool.h>

/**
 * @brief Module de gestion des registres.
 */


/* indices des registres */
enum {
    ZERO,
    AT,
    V0, V1,
    A0, A1, A2, A3,
    T0, T1, T2, T3, T4, T5, T6, T7,
    S0, S1, S2, S3, S4, S5, S6, S7,
    T8, T9,
    K0, K1,
    GP,
    SP,
    FP,
    RA
};

/* nom des registres */
static char *reg_names[] = {
    "zero",
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
    "ra"
};

/* structure d'un registre */
struct reg {
        char name[4];
        uint32_t value;
};

/* 32 registres */
#define GPR_LENGTH 32
struct reg GPR[GPR_LENGTH];

/* registres speciaux */
struct reg PC, HI, LO;

bool exitMask;

/* initialise les registres */
void init_GPR();

/* inverse le masque */
void switch_exitMask();

/* verifie la validite d'un registre */
void check_register(uint8_t index);

/* affiche tous les registres */
void print_gpr();

/* retourne l'index du registre */
uint8_t get_register_index(char *name);

/* retourne la valeur d'un registre */
uint32_t get_register_value(uint8_t index);
uint32_t get_register_value_by_name(char *name);
uint32_t get_PC_value();
uint32_t get_HI_value();
uint32_t get_LO_value();

/* ecrit une valeur dans un registre */
void set_register_value(uint8_t index, uint32_t value);
void set_register_value_by_name(char *name, uint32_t value);
void set_PC_value(uint32_t value);
void set_HI_value(uint32_t value);
void set_LO_value(uint32_t value);

/* avance le registre PC de 4 */
void advance_PC();

/* determine si une chaine est un nombre */
uint32_t isNumeric(char *s);

/* recupere le nom d'un registre */
char *get_register_name(uint8_t index);

/* affiche un registre */
void print_a_gpr(char *name);

/* charge une valeur dans un registre */
void set_a_gpr(char *name, uint32_t value);

#endif /* end of include guard: _GPR_ */
