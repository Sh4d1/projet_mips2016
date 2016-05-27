#include "gpr.h"
#define GPR_LENGTH 32

// initialise les registres
void init_GPR()
{
        for (uint8_t i = 0; i < GPR_LENGTH; i++) {
                GPR[i].value = 0;
        }
}

// retourne la valeur d'un registre
uint32_t get_register_value(uint8_t index)
{
        return GPR[index].value;
}

// ecrit une valeur dans un registre
void set_register_value(uint8_t index, uint32_t value)
{
        GPR[index].value = value;
}
