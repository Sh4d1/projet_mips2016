#ifndef _GPR_
#define _GPR_

#include <stdint.h>

struct reg {
        char name[4];
        uint32_t value;
};

struct reg GPR[32];

// initialise les registres
void init_GPR();

// retourne la valeur d'un registre
uint32_t get_register_value(uint8_t index);

// ecrit une valeur dans un registre
void set_register_value(uint8_t index, uint32_t value);

#endif /* end of include guard: _GPR_ */
