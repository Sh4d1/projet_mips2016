#include <stdio.h>
#include <stdlib.h>

#include "include/framebuffer_s.h"

int main() {
    uint8_t *premier_pixel = framebuffer_init_display();
    int32_t index = 0;
    while (index >= 0) {
        printf("Ecire un numero de pixel entre 0 et (320 * 200) - 1 :\nNégatif pour terminer.\n");
        scanf("%d", &index);
        // uint32_t fin = index + 320;
        // for (uint32_t i = index; i < fin; i++) {
        //     if (index < 320 * 200) {
        //         premier_pixel[index] = 0xFF; //blanc
        //     }
        //     index++;
        // }
        if(index>=0) premier_pixel[index] = 0xFF;
    }
    framebuffer_close_display();
    return 0;
}
