#ifndef _GPR_
#define _GPR_

#include <inttypes.h>

#define GPR_LENGTH 32

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

uint32_t isNumeric(char *s);


#endif /* end of include guard: _GPR_ */
