#ifndef _FRAMEBUFFER_H
#define _FRAMEBUFFER_H

#include <stdint.h>


/**
 * @brief Module de gestion d'une fenêtre graphique pour l'affichage du
 * frambuffer.
 *
 * @note L'utilisation de ce module nécessite de lier la librairie SDL
 * (-lSDL).
 *
 * Après la création initiale de la fenêtre, l'affichage est mis à
 * jour toutes les 0.5 secondes.
 */

#define FRAMEBUFFER_W   320
#define FRAMEBUFFER_H   200

/**
 * @brief framebuffer_init_display Cree une fenetre graphique de taille fixe
 * (FRAMEBUFFER_W par FRAMEBUFFER_H).
 *
 * La valeur retournée est l'adresse du premier pixel de la zone
 * d'affichage, dans le coin en haut à gauche.
 * - framebuffer[0] est le pixel en haut à gauche
 * - framebuffer[FRAMEBUFFER_W - 1] est le pixel en haut à droite
 * - framebuffer[(FRAMEBUFFER_W * FRAMEBUFFER_H) - 1] is est le pixel en bas à droite
 * Chaque pixel est encodé en niveaux de gris sur 8 bits (0 pour le noir,
 * 255 pour le blanc).
 *
 * L'adresse retournée sera libérée lors de l'appel à
 * \ref framebuffer_close_display.
 *
 * @note Il reste des fuites mémoires même après l'appel à framebuffer_close_display
 * (merci SDL...). Solution?
 *
 * @return l'adresse du premier pixel du frambuffer
 */
extern uint8_t *framebuffer_init_display();


/**
 * @brief framebuffer_close_display ferme la fenêtre crée par le
 * précédent appel à \ref framebuffer_init_display.
 *
 * @note Il reste des fuites mémoires même après l'appel à cette fonction
 * (merci SDL...). Solution?
 */
extern void framebuffer_close_display();

#endif
