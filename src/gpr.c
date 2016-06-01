#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "../include/gpr.h"

// initialise les registres
void init_GPR()
{
        for (uint8_t i = 0; i < GPR_LENGTH; i++) {
                GPR[i].value = 0;
                strncpy(GPR[i].name, reg_names[i], strlen(reg_names[i]));
        }
        PC.value = 0;
}

// affiche tous les registres
void print_gpr()
{
    for (uint8_t i = 0; i < GPR_LENGTH; i++) {
        printf("Reg %-4s ($%02u): ", GPR[i].name, i);
        printf("0x%08x\t\t", GPR[i].value);
        if ((i+1) % 4 == 0) {
            printf("\n");
        }
    }
}

// retourne l'index du registre
uint8_t get_register_index(char *name)
{
        for (uint8_t i = 0; i < GPR_LENGTH; i++) {
                if (!strcmp(name, GPR[i].name)) {
                        return i;
                }
        }
        fprintf(stderr, "Le registre %s n'existe pas.\n", name);
        exit(EXIT_FAILURE);
}

// retourne la valeur d'un registre
uint32_t get_register_value(uint8_t index)
{
        if (index > 31) {
                fprintf(stderr, "Le registre %u n'existe pas.\n", index);
                exit(EXIT_FAILURE);
        }
        return GPR[index].value;
}

uint32_t get_register_value_by_name(char *name)
{
        return GPR[get_register_index(name)].value;
}

uint32_t get_PC_value()
{
        return PC.value;
}

uint32_t get_HI_value()
{
        return HI.value;
}

uint32_t get_LO_value()
{
        return LO.value;
}

// ecrit une valeur dans un registre
void set_register_value(uint8_t index, uint32_t value)
{
        if (index > 31) {
                fprintf(stderr, "Le registre %u n'existe pas.\n", index);
                exit(EXIT_FAILURE);
        }
        if (index != 0) {
                GPR[index].value = value;
        }
}

void set_register_value_by_name(char *name, uint32_t value)
{
        GPR[get_register_index(name)].value = value;
}

void set_PC_value(uint32_t value)
{
        PC.value = value;
}

void set_HI_value(uint32_t value)
{
        HI.value = value;
}

void set_LO_value(uint32_t value)
{
        LO.value = value;
}

uint32_t isNumeric(char *s)
{
    if (s == NULL || *s == '\0' || isspace(*s))
      return 0;
    char * p;
    strtod (s, &p);
    return *p == '\0';
}
