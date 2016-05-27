#include <stdio.h>
#include "integer.h"


int main(int argc, char const *argv[]) {
        struct integer *entier = create_integer(BYTE, 250, false);
        print(entier, 2);
        print(entier, 16);
        free(entier);
        return 0;
}
