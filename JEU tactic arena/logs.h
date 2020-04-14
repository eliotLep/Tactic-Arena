#ifndef LOG_H
#define LOG_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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



typedef struct degatx_s{
  int pos_x;
  int pos_y;
  int time;
  int c; //noire =0 rouge =1 verte =2
  char txt[20];
}degatx_t;


typedef enum {
    info=1,degat,soins,ineficace,deplacement,kill
}typeTxt_t;


typedef struct bash_s{
  char txt[80];
  int pos_y;
  typeTxt_t couleur;
}bash_t;


void ajouter_ligne_bash(char * variable,bash_t tab_info_bash[TAILLE_TAB_BASH],typeTxt_t c,char variable2[80]);

void ajouter_degat_txt(char txt[20],degatx_t aff_deg[AFF_DEG],int x,int y,int c); //ajoute un texte dans le tableau d'anim des dégats, blocks et kills

void clean_degat_txt(degatx_t aff_deg[AFF_DEG]); //nettoie le tableau d'anim de texte des degats,blocks, kills etc

int vide(degatx_t aff_deg[AFF_DEG]); //regarde si le tableau d'affichage des dégats sous forme d'animation est vide ou non

#endif
