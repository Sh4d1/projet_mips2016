#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "../include/gpr.h"

// initialise les registres
void init_GPR()
{
    GPR[0].value = 0;
    for (uint8_t i = 1; i < GPR_LENGTH; i++) {
        set_register_value(i, 0);
        strncpy(GPR[i].name, reg_names[i], strlen(reg_names[i]));
    }
    PC.value = 0;
}

/* verifie la validite d'un registre */
void check_register(uint8_t index)
{
    if (index > 31) {
        fprintf(stderr, "Le registre %u n'existe pas.\n", index);
        exit(EXIT_FAILURE);
    }
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

void print_a_gpr(char *name) {
    uint8_t i = get_register_index(name);
    printf("Reg %-4s ($%02u): ", GPR[i].name, i);
    printf("0x%08x\n", GPR[i].value);
}

// retourne l'index du registre
uint8_t get_register_index(char *name)
{
        name = (strncmp("$", name, 1)) ? name : name + 1;
        if (isNumeric(name)) {
            uint8_t index = strtol(name, NULL, 10);
            return index;
        } else {
            for (uint8_t i = 0; i < GPR_LENGTH; i++) {
                if (!strcmp(name, GPR[i].name)) {
                    return i;
                }
            }
            printf("Le registre %s n'existe pas.\n", name);
            exit(EXIT_FAILURE);
        }

}

// retourne la valeur d'un registre
uint32_t get_register_value(uint8_t index)
{
        check_register(index);
        return GPR[index].value;
}

uint32_t get_register_value_by_name(char *name)
{
        return get_register_value(get_register_index(name));
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
    check_register(index);
    if (index) {
        GPR[index].value = value;
    }
}

void set_register_value_by_name(char *name, uint32_t value)
{
    set_register_value(get_register_index(name), value);
}

void set_PC_value(uint32_t value)
{
        PC.value = value;
}

void advance_PC()
{
    set_PC_value(get_PC_value() + 4);
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
