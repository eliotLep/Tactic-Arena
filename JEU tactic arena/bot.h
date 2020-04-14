#ifndef BOT_H
#define BOT_H
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






void depla_atk_mov(
    case_t terrain[N][M],int x_bot,int y_bot,
    int joueur_actu,joueurs_t tab[J],bash_t tab_info_bash[TAILLE_TAB_BASH],
    char variable2[80]
); //IA de deplacement des bots

void depla_allie_plus_proche(case_t terrain[N][M],int x_bot,int y_bot,
int joueur_actu,joueurs_t tab[J],bash_t tab_info_bash[TAILLE_TAB_BASH],
char variable2[80]
); //deplacement au plus proche d'un allié potentiellement en dehors de la zone de deplacement possible de l'unitée se deplaçant(utilise move_longue_range)


void depla_ennem_plus_proche(case_t terrain[N][M],int x_bot,int y_bot,
int joueur_actu,joueurs_t tab[J],bash_t tab_info_bash[TAILLE_TAB_BASH],
char variable2[80]
); //deplacement au plus proche d'un ennemi potentiellement en dehors de la zone de deplacement possible de l'unitée se deplaçant(utilise move_longue_range)

void IA_blockage_direction(case_t terrain[N][M],int x_def,int y_def,
int joueur_actu
); //met le blockage de l'unité en x_def et y_def dans la position la plus intéressante

void move_longue_range(case_t terrain[N][M], int x, int y,int x_dest,
int y_dest,int joueur_actu,joueurs_t tab[J],bash_t tab_info_bash[TAILLE_TAB_BASH],
char variable2[80]
); //pathfinding et deplacement utilisé pour faire le déplacement et trouver la coordonné pour se rapprocher au max de la cible


void move_alea(case_t terrain[N][M],int x,int y, int joueur,
joueurs_t tab[J],bash_t tab_info_bash[TAILLE_TAB_BASH],char variable2[80]
); //deplacement aléatoire parmis les choix de déplacement possible


void attaquer_meilleur_cible(case_t terrain[N][M],int x_bot,int y_bot,
int nb_ennemies_portee,int joueur_actu,joueurs_t tab[J],
degatx_t aff_deg[AFF_DEG],bash_t tab_info_bash[TAILLE_TAB_BASH],
char variable2[80]
); //IA d'attaque des bots

int action_possible(case_t terrain[N][M],int joueur_actu);


#endif
