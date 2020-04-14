#ifndef COMBAT_H
#define COMBAT_H
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
extern int DEGAT_EBOULEMENT;







void pathfinding_combat(case_t terrain[N][M], int x, int y,int joueur_actu); //remplis la grille de jeu (terrain[x][y].attaque= ?) selon si la case est apte a l'attaque d'un allié

int calc_block(case_t terrain [N][M],int x_att, int y_att, int x_def,int y_def); //retourne la valeur du block de la cible en fonction de la position de notre unité

void combat(
  case_t terrain [N][M],int x_att, int y_att, int x_def,int y_def,
  int joueur,joueurs_t tab[J],degatx_t aff_deg[AFF_DEG],
  bash_t tab_info_bash[TAILLE_TAB_BASH],char variable2[80]
); //systeme de combat

void soin(
  case_t terrain [N][M],int x_att, int y_att, int x_def,int y_def,
  int joueur,joueurs_t tab[J],degatx_t aff_deg[AFF_DEG],
  bash_t tab_info_bash[TAILLE_TAB_BASH],char variable2[80]
); //soigne un allié

void update_stats(case_t terrain[N][M],int x,int y,int joueur_actu,joueurs_t tab[J]); //amélioration des stats apres un kill en fonction de la classe.

int a_portee(case_t terrain[N][M],int x_bot,int y_bot,int joueur_actu); //nb ennemis a porte (via pathfinding combat)


#endif
