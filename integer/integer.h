#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BYTE 8
#define HWORD 16
#define WORD 32

struct integer {
        uint32_t length;
        uint32_t value;
        bool sign;
};

// cree un entier
struct integer *create_integer(uint32_t length, uint32_t value, bool sign);

// affiche l'entier dans la base specifiee
void print(struct integer *entier, uint32_t base);

// calcule l'oppose de l'entier
void opposite(struct integer *entier);

// realise une extension de signe de l'entier sur n bits
void extend(struct integer *entier, uint32_t n);
