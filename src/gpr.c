#include <stdio.h>
#include "../include/gpr.h"

// initialise les registres
void init_GPR()
{
        for (uint8_t i = 0; i < GPR_LENGTH; i++) {
                strcpy(GPR[i].name, reg_names[i]);
        }
        for (uint8_t i = 0; i < GPR_LENGTH; i++) {
                printf("%s", GPR[i].name);
        }
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
