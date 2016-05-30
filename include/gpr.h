#ifndef _GPR_
#define _GPR_

#define GPR_LENGTH 32

struct reg {
        char name[4];
        uint32_t value;
};

const char * const reg_names[] = {      "zero",
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

struct reg GPR[GPR_LENGTH];

// initialise les registres
void init_GPR();

// retourne la valeur d'un registre
uint32_t get_register_value(uint8_t index);

// ecrit une valeur dans un registre
void set_register_value(uint8_t index, uint32_t value);

#endif /* end of include guard: _GPR_ */
