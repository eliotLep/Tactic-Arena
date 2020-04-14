#ifndef DEPLACEMENT_H
#define DEPLACEMENT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "grille.h"
#include "logs.h"


extern int J_HUMAIN;
extern int NB_UNITE;
extern int ESPACE_GEN;
extern int PTS_ACTION_MAX;

extern int VITESSE_JEU_BOT;
extern int VITESSE_ANIM;

extern int AFF_DEG;
extern int TAILLE_TAB_BASH;

extern int PRESET;
extern int PRESET_U;
extern int NB_CLASSE;
extern int N;
extern int M;
extern int J;
extern int BASH_SCROLL_SPEED;
extern int DURE_JOUR_NUIT;
extern int NB_MAX_PRIEST;
extern int ISO;
extern float COEF_AFFICHAGE;
extern int TOUR_EVOL_FORET;
extern int CHANCE_EVOL;







void pathfinding(case_t terrain[N][M], int x, int y); //remplis la grille de jeu (terrain[x][y].deplacement= ?) selon si la case est apte au deplacement d'un allié SELECTIONNE

void move(case_t terrain[N][M],int x,int y, int joueur,joueurs_t tab[J],bash_t tab_info_bash[TAILLE_TAB_BASH],char variable2[80]); //deplacement vers une coordonné x et y de la grille

#endif
