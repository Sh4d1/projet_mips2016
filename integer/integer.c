#include <math.h>
#include "integer.h"

// cree un entier
struct integer *create_integer(uint32_t length, uint32_t value, bool sign) {
        struct integer *entier = malloc(sizeof(struct integer));
        entier->length = length;
        entier->value = value;
        entier->sign = sign;
        return entier;
}

// affiche l'entier dans la base specifiee
void print(struct integer *entier, uint32_t base) {
        switch (base) {
        case 2: {
                uint32_t mask = 1;
                mask <<= entier->length - 1;
                for (uint32_t i = entier->length; i > 0; i--, mask >>= 1) {
                        printf("%u", (entier->value & mask) >> (i - 1));
                }
                printf("\n");
                break;
        }
        case 16: {
                uint32_t mask = 15;
                mask <<= entier->length - 4;
                printf("Ox");
                for (uint32_t i = entier->length / 4; i > 0; i--, mask >>= 4) {
                        printf("%x", (entier->value & mask) >> (4*(i - 1)));
                }
                printf("\n");
                break;
        }
        default:
                printf("Affichage impossible : base non supportée.\n");
        }
}
