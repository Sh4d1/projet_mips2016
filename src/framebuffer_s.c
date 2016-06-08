#include "../include/framebuffer_s.h"
#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <stdbool.h>

SDL_Thread *thread = NULL;
SDL_Surface *ecran = NULL;
SDL_Color colors[256];


int fb()
{

    while (true) {
        SDL_Flip(ecran);
        SDL_Delay(500);

        //SDL_UpdateRect(ecran, 0, 0, 0, 0);
    }
    return 0;
}


uint8_t *framebuffer_init_display()
{

    for(int i=0;i<256;i++){
          colors[i].r=i;
          colors[i].g=i;
          colors[i].b=i;
    }

    if (SDL_Init(SDL_INIT_VIDEO) == -1) // Démarrage de la SDL. Si erreur :
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError()); // Écriture de l'erreur
        exit(EXIT_FAILURE); // On quitte le programme
    }

    ecran = SDL_SetVideoMode(FRAMEBUFFER_W, FRAMEBUFFER_H, 8, SDL_HWPALETTE);



    // SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
    SDL_LockSurface(ecran);
    SDL_SetPalette(ecran, SDL_LOGPAL|SDL_PHYSPAL, colors, 0, 256);

    thread = SDL_CreateThread(fb, NULL);



    return (uint8_t *)ecran->pixels;


}

void framebuffer_close_display()
{
    
    SDL_KillThread( thread );
    SDL_FreeSurface( ecran );
    SDL_Quit();
}
