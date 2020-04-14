#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "logs.h"






void ajouter_ligne_bash(char * variable,bash_t tab_info_bash[TAILLE_TAB_BASH],typeTxt_t c,char variable2[80]){
  if(!strcmp(tab_info_bash[0].txt,"PARTIE INITIALISE")){
    FILE * fp=fopen(variable2,"w");
    fprintf(fp,"MAP : %d x %d || Joueurs : %d || UNITEE : %d par joueurs\n",N,M,J,NB_UNITE);
    fclose(fp);
  }
  FILE * fp=fopen(variable2,"a");
  fprintf(fp,variable);
  fprintf(fp,"\n");
  fclose(fp);


  for(int j=TAILLE_TAB_BASH-1;j>0;j--){
    if(tab_info_bash[j-1].pos_y!=0){
      tab_info_bash[j].pos_y=tab_info_bash[j-1].pos_y-25; ///ICI POUR PLACE ENTRE LES LIGNE DE BASH (-TAILLE ENTRE LES LIGNES).
      tab_info_bash[j].couleur=tab_info_bash[j-1].couleur;
      strcpy(tab_info_bash[j].txt,tab_info_bash[j-1].txt);
    }
  }
  tab_info_bash[0].pos_y=tab_info_bash[1].pos_y+25;
  tab_info_bash[0].couleur=c;
  strcpy(tab_info_bash[0].txt,variable);
  return;
}



void ajouter_degat_txt(char txt[20],degatx_t aff_deg[AFF_DEG],int x,int y,int c){
	for(int i=0;i<AFF_DEG;i++){
		if(aff_deg[i].time<=0){
      strcpy(aff_deg[i].txt,txt);
      aff_deg[i].pos_x=x;
      aff_deg[i].pos_y=y;
      aff_deg[i].c=c;
      aff_deg[i].time=100;
      return;
    }
	}
}


void clean_degat_txt(degatx_t aff_deg[AFF_DEG]){
  for(int i=0;i<AFF_DEG;i++){
		if(aff_deg[i].time<=0 && !vide(aff_deg)){
      for(int j=i;j<(AFF_DEG)-1;j++){
        aff_deg[j].time=aff_deg[j+1].time;
        aff_deg[j].pos_x=aff_deg[j+1].pos_x;
        aff_deg[j].pos_y=aff_deg[j+1].pos_y;
        aff_deg[j].c=aff_deg[j+1].c;
        strcpy(aff_deg[j].txt,aff_deg[j+1].txt);
      }
    }else{
      if(strcmp(aff_deg[i].txt,"MORT")){
        aff_deg[i].time-=10;
        aff_deg[i].pos_x+=(aff_deg[i].time/10);
        aff_deg[i].pos_y-=(aff_deg[i].time/10);
      }else{
        aff_deg[i].time-=10;
        aff_deg[i].pos_y-=(aff_deg[i].time/15);
      }
    }
  }
}

int vide(degatx_t aff_deg[AFF_DEG]){
  for(int i=0;i<AFF_DEG;i++){
    if(aff_deg[i].time>0)
      return 0;
  }
  return 1;
}





