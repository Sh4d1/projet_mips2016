#include "../include/framebuffer_s.h"
#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>

SDL_Thread *thread = NULL;
SDL_Surface *ecran = NULL;

int fb()
{
    while (1) {
        SDL_Delay(500);
        SDL_Flip(ecran);
    }
    return 0;
}


uint8_t *framebuffer_init_display()
{
    if (SDL_Init(SDL_INIT_VIDEO) == -1) // Démarrage de la SDL. Si erreur :
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError()); // Écriture de l'erreur
        exit(EXIT_FAILURE); // On quitte le programme
    }

    ecran = SDL_SetVideoMode(FRAMEBUFFER_W, FRAMEBUFFER_H, 0, SDL_HWSURFACE);

    thread = SDL_CreateThread(fb, (void *)NULL );

    return (uint8_t *)ecran->pixels;
}

void framebuffer_close_display()
{
    SDL_KillThread( thread );
    SDL_FreeSurface( ecran );
    SDL_Quit();
}
