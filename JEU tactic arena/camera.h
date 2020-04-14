#ifndef CAMERA_H
#define CAMERA_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "grille.h"

extern float COEF_AFFICHAGE;
extern int TOUR_EVOL_FORET;
extern int CHANCE_EVOL;






void centrer_camera(case_t terrain[N][M],int x,int y,int largeur,int hauteur); //centre la caméra sur une case de la grille
void camera_sur_allie(case_t terrain[N][M],int joueur_actu,joueurs_t tab[J],int * largeur,int * hauteur);

#endif
