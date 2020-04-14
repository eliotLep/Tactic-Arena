#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "deplacement.h"




void pathfinding(case_t terrain[N][M], int x, int y){
  int deplacement[N][M];
  int i,j;
  int cpt;
    //initialisation des deplacements possibles
    for (i=0;i<N;i++){//parcours du terrain
      for(j=0;j<M;j++){
        terrain[i][j].deplacement=0;
      }
    }
    if(terrain[x][y].piece==NULL){
      return;
    }


    for (i=0;i<N;i++){//parcours du terrain
        for(j=0;j<M;j++){
            if (terrain[i][j].type==5 || terrain[i][j].piece!=NULL || (terrain[i][j].bloc!=NULL && terrain[x][y].piece->joueur!=terrain[i][j].bloc->block_allie)){
                deplacement[i][j]=-1;
            }//case d'eau deplacement impossible
            else{
              deplacement[i][j]=0;
            }
        }


    }
    deplacement[x][y]=1;//case ou se trouve la piece

    int depla;
    //désavantage de deplacement qd le deplacement commence sur une case foret ou montagne.
    if(terrain[x][y].type==6 || terrain[x][y].type==7){
      depla=terrain[x][y].piece->deplacement -1;
    }else{
      depla=terrain[x][y].piece->deplacement;
    }


    for(cpt=1;cpt<=depla;cpt++){//tant qu'il y a des point de deplacement
        for (i=0;i<N;i++){//parcours du terrain
            for(j=0;j<M;j++){
                if (deplacement[i][j]==cpt){
                    if(i+1<N && deplacement[i+1][j]==0)
                        deplacement[i+1][j]=cpt+1;
                    if(i-1>=0 && deplacement[i-1][j]==0)
                        deplacement[i-1][j]=cpt+1;
                    if(j+1<M && deplacement[i][j+1]==0)
                        deplacement[i][j+1]=cpt+1;
                    if(j-1>=0 && deplacement[i][j-1]==0)
                        deplacement[i][j-1]=cpt+1;

                }

            }
        }
    }

    for (i=0;i<N;i++){//parcours du terrain
        for(j=0;j<M;j++){
            if(deplacement[i][j]>1){
              terrain[i][j].deplacement=1;
            }
        }
    }

}






void move(case_t terrain[N][M],int x,int y,int joueur,joueurs_t tab[J],bash_t tab_info_bash[TAILLE_TAB_BASH],char variable2[80]){
  char variable[80];
  if(tab[joueur].pts_action_actu>0){
    if(terrain[x][y].deplacement==1){
      for(int compteur=0;compteur<N;compteur++){
        for(int compteur2=0;compteur2<M; compteur2++){
          if(terrain[compteur][compteur2].piece && terrain[compteur][compteur2].piece->select==1 && terrain[compteur][compteur2].piece->joueur==joueur && terrain[compteur][compteur2].piece->pts_action_actu>0){

            sprintf(variable, "| Unite de Joueur %d en %d/%d se deplace vers %d/%d",joueur,compteur,compteur2,x,y);
				    ajouter_ligne_bash(variable,tab_info_bash,deplacement,variable2);


            terrain[x][y].piece=malloc(sizeof(piece_t));
            terrain[x][y].piece->classe = terrain[compteur][compteur2].piece->classe;
            terrain[x][y].piece->pdv = terrain[compteur][compteur2].piece->pdv;
            terrain[x][y].piece->puissance = terrain[compteur][compteur2].piece->puissance;
            terrain[x][y].piece->armure = terrain[compteur][compteur2].piece->armure;
            terrain[x][y].piece->block = terrain[compteur][compteur2].piece->block;
            terrain[x][y].piece->portee = terrain[compteur][compteur2].piece->portee;
            terrain[x][y].piece->deplacement = terrain[compteur][compteur2].piece->deplacement;
            terrain[x][y].piece->select = terrain[compteur][compteur2].piece->select;
            terrain[x][y].piece->joueur = terrain[compteur][compteur2].piece->joueur;
            terrain[x][y].piece->direction = terrain[compteur][compteur2].piece->direction;
            terrain[x][y].piece->kill = terrain[compteur][compteur2].piece->kill;
            terrain[x][y].piece->frame = terrain[compteur][compteur2].piece->frame;
            terrain[x][y].piece->frame_interface = terrain[compteur][compteur2].piece->frame_interface;
            terrain[x][y].piece->start_anim = terrain[compteur][compteur2].piece->start_anim;
            terrain[x][y].piece->pts_action_max = terrain[compteur][compteur2].piece->pts_action_max;
            terrain[x][y].piece->pts_action_actu = terrain[compteur][compteur2].piece->pts_action_actu;


            free(terrain[compteur][compteur2].piece);
            terrain[compteur][compteur2].piece=NULL;

            terrain[x][y].piece->select=1;

            tab[joueur].pts_action_actu--;
            terrain[x][y].piece->pts_action_actu--;

            return;
          }
        }
      }

    }
  }
}