#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "grille.h"
#include "combat.h"
#include "deplacement.h"







void save(case_t terrain[N][M],int compteur_tour,int joueur_actu,joueurs_t tab[J]){
  char variable3[200];
  FILE * fp;
  sprintf(variable3,"fichiers/saves/save_%d_%d",rand()%3000,rand()%3000);
  fp=fopen(variable3, "w");
  //on commence par save les variable principales de la partie et les types de cases
  fprintf(fp,"N=%d M=%d J=%d J_HUMAIN=%d VITESSE_JEU_BOT=%d VITESSE_ANIM=%d DURE_JOUR_NUIT=%d compteur_tour=%d joueur_actu=%d\n",N,M,J,J_HUMAIN,VITESSE_JEU_BOT,VITESSE_ANIM,DURE_JOUR_NUIT,compteur_tour,joueur_actu);
  fprintf(fp,"\n");
  for(int i=0;i<N;i++){
    for(int j=0;j<M;j++){
      fprintf(fp,"{%d %d} type=%d xImg=%d yImg=%d x1=%d x2=%d x3=%d x4=%d y1=%d y2=%d y3=%d y4=%d\n",i,j,terrain[i][j].type,terrain[i][j].xImg,terrain[i][j].yImg,terrain[i][j].x1,terrain[i][j].x2,terrain[i][j].x3,terrain[i][j].x4,terrain[i][j].y1,terrain[i][j].y2,terrain[i][j].y3,terrain[i][j].y4);
    }
  }
  //on sauvegarde le tableau de joueurs
  fprintf(fp,"\n");
  for(int i=0;i<J;i++){
    fprintf(fp,"id_joueur=%d nb_unite=%d humain=%d pts_action_max=%d pts_action_actu=%d\n",tab[i].id_joueur,tab[i].nb_unite,tab[i].humain,tab[i].pts_action_max,tab[i].pts_action_actu);
  }
  //on sauvegarde chaque piece
  fprintf(fp,"\n");
  for(int i=0;i<N;i++){
    for(int j=0;j<M;j++){
      if(terrain[i][j].piece){
        fprintf(fp,"{%d %d} classe=%d pdv=%d puissance=%d armure=%d block=%d portee=%d deplacement=%d joueur=%d direction=%d kill=%d frame=%d frame_interface=%d start_anim=%d \n",i,j,terrain[i][j].piece->classe,terrain[i][j].piece->pdv,terrain[i][j].piece->puissance,terrain[i][j].piece->armure,terrain[i][j].piece->block,terrain[i][j].piece->portee,terrain[i][j].piece->deplacement,terrain[i][j].piece->joueur,terrain[i][j].piece->direction,terrain[i][j].piece->kill,terrain[i][j].piece->frame,terrain[i][j].piece->frame_interface,terrain[i][j].piece->start_anim);
      }else{
        fprintf(fp,"{%d %d} classe=-1\n",i,j);
      }
    }
  }

  fclose(fp);
}


int nombre_classe(case_t terrain[N][M],int joueur_actu,classe_t classe){
  int nb=0;
  for(int i=0;i<N;i++){
    for(int j=0;j<M;j++){
      if(terrain[i][j].piece && terrain[i][j].piece->classe==classe && terrain[i][j].piece->joueur==joueur_actu){
        nb++;
      }
    }
  }
  return nb;
}


void rem_piece_joueur(case_t terrain[N][M],int joueur_actu){
  for(int i=0;i<N;i++){
    for(int j=0;j<M;j++){
      if(terrain[i][j].piece && terrain[i][j].piece->joueur==joueur_actu){
        free(terrain[i][j].piece);
        terrain[i][j].piece=NULL;
      }
    }
  }
  return;
}






int reste_ennemi(case_t terrain[N][M],int joueur_actu){
  int nb=0;
  for(int i=0;i<N;i++){
    for(int j=0;j<M;j++){
      if(terrain[i][j].piece && terrain[i][j].piece->joueur != joueur_actu){
        nb++;
      }
    }
  }
  return nb;
}

int reste_allie(case_t terrain[N][M],int joueur_actu){
  int nb=0;
  for(int i=0;i<N;i++){
    for(int j=0;j<M;j++){
      if(terrain[i][j].piece && terrain[i][j].piece->joueur == joueur_actu){
        nb++;
      }
    }
  }
  return nb;
}





int allie_adjacent(case_t terrain[N][M],int x_bot,int y_bot,int joueur_actu){
  for(int i=0;i<N;i++){
    for(int j=0;j<M;j++){
      if(i+1<N && terrain[i+1][j].piece && terrain[i+1][j].piece->joueur==joueur_actu){
        return 1;
      }
      if(j+1<N && terrain[i][j+1].piece && terrain[i][j+1].piece->joueur==joueur_actu){
        return 1;
      }
      if(i-1>=0 && terrain[i-1][j].piece && terrain[i-1][j].piece->joueur==joueur_actu){
        return 1;
      }
      if(j-1>=0 && terrain[i][j-1].piece && terrain[i][j-1].piece->joueur==joueur_actu){
        return 1;
      }
    }
  }
  return 0;

}



void carte_valide(case_t terrain[N][M]){
   int cases[N][M];
   for(int i=0;i<N;i++){
     for(int j=0;j<M;j++){
       if(terrain[i][j].type==5){
         cases[i][j]=-1;
       }else{
         cases[i][j]=0;
       }
     }
   }
   //coin =1
   cases[0][0]=1;
   cases[N-1][M-1]=1;
   cases[0][M-1]=1;
   cases[N-1][0]=1;

   for(int a=0;a<N*M;a++){
    for(int i=0;i<N;i++){
      for(int j=0;j<M;j++){
        if (cases[i][j]==1){
          if(i+1<N && cases[i+1][j]==0)
              cases[i+1][j]=1;
          if(i-1>=0 && cases[i-1][j]==0)
             cases[i-1][j]=1;
          if(j+1<M && cases[i][j+1]==0)
             cases[i][j+1]=1;
          if(j-1>=0 && cases[i][j-1]==0)
             cases[i][j-1]=1;
         }
       }
     }
   }
   //verification de la map
   for(int i=0;i<N;i++){
     for(int j=0;j<M;j++){
       if(cases[i][j]==0){
         cases[i][j]=2;
       }
     }
   }
   //correction de la map
   for(int i=0;i<N;i++){
     for(int j=0;j<M;j++){
       if(cases[i][j]==2){
         terrain[i][j].type=5;
       }
     }
   }
   return;
 }


void reset_block(case_t terrain[N][M]){
  for(int i=0;i<N;i++){
    for(int j=0;j<M;j++){
      terrain[i][j].block=0;
    }
  }
  return;
}

int viser_block(case_t terrain[N][M]){
  for(int i=0;i<N;i++){
    for(int j=0;j<M;j++){
      if(terrain[i][j].block==1)
        return 1;
    }
  }
  return 0;
}


int pathfinding_block(case_t terrain[N][M],int joueur_actu,int type){
  int min=0;
  reset_block(terrain);
  for(int i=0;i<N;i++){
    for(int j=0;j<M;j++){
      if(terrain[i][j].piece && terrain[i][j].piece->select==1){
        terrain[i][j].piece->select=0;
      }
    }
  }
  if(type==1){
    for(int i=0;i<N;i++){
      for(int j=0;j<M;j++){
        if(terrain[i][j].piece && terrain[i][j].piece->joueur==joueur_actu){
          if(i-1>=0 && terrain[i-1][j].type>=1 && terrain[i-1][j].type<=4 && terrain[i-1][j].piece==NULL && terrain[i-1][j].bloc==NULL){
            terrain[i-1][j].block=1;
            min=1;
          }
          if(i+1<N && terrain[i+1][j].type>=1 && terrain[i+1][j].type<=4 && terrain[i+1][j].piece==NULL  && terrain[i+1][j].bloc==NULL){
            terrain[i+1][j].block=1;
            min=1;
          }
          if(j-1>=0 && terrain[i][j-1].type>=1 && terrain[i][j-1].type<=4 && terrain[i][j-1].piece==NULL  && terrain[i][j-1].bloc==NULL){
            terrain[i][j-1].block=1;
            min=1;
          }
          if(j+1<M && terrain[i][j+1].type>=1 && terrain[i][j+1].type<=4 && terrain[i][j+1].piece==NULL  && terrain[i][j+1].bloc==NULL){
            terrain[i][j+1].block=1;
            min=1;
          }
        }
      }
    }
  }else if(type==2){
    for(int i=0;i<N;i++){
      for(int j=0;j<M;j++){
        if(terrain[i][j].piece && terrain[i][j].piece->joueur==joueur_actu){
          for(int a=i-3;a<N && a<=i+3;a++){
            for(int b=j-3;b<M && b<=j+3;b++){
              if(a>=0 && b>=0 && terrain[a][b].piece==NULL && terrain[a][b].type>=1 && terrain[a][b].type<=4 && terrain[a][b].bloc==NULL){
                terrain[a][b].block=2;
              }
            }
          }
        }
      }
    }
    for(int compteur=0;compteur<N;compteur++){
      for(int compteur2=0;compteur2<M;compteur2++){
        if(compteur-2>=0 && compteur2-2>=0 && terrain[compteur][compteur2].block!=0 && terrain[compteur][compteur2-1].block!=0 && terrain[compteur-1][compteur2-1].block!=0 && terrain[compteur-1][compteur2].block!=0 && terrain[compteur-2][compteur2].block!=0 && terrain[compteur-2][compteur2-1].block!=0 && terrain[compteur-2][compteur2-2].block!=0 && terrain[compteur-1][compteur2-2].block!=0 && terrain[compteur][compteur2-2].block!=0){
          terrain[compteur][compteur2].block=1;
          min=1;
        }
      }
    }
    for(int compteur=0;compteur<N;compteur++){
      for(int compteur2=0;compteur2<M;compteur2++){
        if(terrain[compteur][compteur2].block==2){
          terrain[compteur][compteur2].block=0;
        }
      }
    }
  }
  if(min==1)
    return 1;

  return 0;
}


void poser_block(case_t terrain[N][M],int compteur,int compteur2,int joueur_actu,joueurs_t tab[J],bash_t tab_info_bash[TAILLE_TAB_BASH],char variable2[80],type_bloc_t type){
  if(type==1){
    if(tab[joueur_actu].pts_action_actu>0 && tab[joueur_actu].nb_block>0){
      char variable[80];
      if(terrain[compteur][compteur2].block==1){
        terrain[compteur][compteur2].bloc=malloc(sizeof(bloc_t));
        terrain[compteur][compteur2].bloc->pdv_block=100;
        terrain[compteur][compteur2].bloc->block_allie=joueur_actu;
        terrain[compteur][compteur2].bloc->armure=35;
        terrain[compteur][compteur2].bloc->type=type;
        terrain[compteur][compteur2].bloc->aire=1;
        tab[joueur_actu].nb_block-=terrain[compteur][compteur2].bloc->aire;
        tab[joueur_actu].pts_action_actu-=terrain[compteur][compteur2].bloc->aire;
        sprintf(variable, "| Joueur %d pose un bloc en %d/%d",joueur_actu,compteur,compteur2);
        ajouter_ligne_bash(variable,tab_info_bash,info,variable2);
      }
    }
  }else if(type==2){
    if(tab[joueur_actu].pts_action_actu>8 && tab[joueur_actu].nb_block>8){
      char variable[80];
      if(terrain[compteur][compteur2].block){
        terrain[compteur][compteur2].bloc=malloc(sizeof(bloc_t));
        terrain[compteur][compteur2].bloc->pdv_block=400;
        terrain[compteur][compteur2].bloc->block_allie=joueur_actu;
        terrain[compteur][compteur2].bloc->type=type;
        terrain[compteur][compteur2].bloc->armure=70;
        terrain[compteur][compteur2].bloc->aire=9;
        terrain[compteur-1][compteur2].bloc=terrain[compteur][compteur2].bloc;
        terrain[compteur][compteur2-1].bloc=terrain[compteur][compteur2].bloc;
        terrain[compteur-1][compteur2-1].bloc=terrain[compteur][compteur2].bloc;
        terrain[compteur-2][compteur2-1].bloc=terrain[compteur][compteur2].bloc;
        terrain[compteur-2][compteur2-2].bloc=terrain[compteur][compteur2].bloc;
        terrain[compteur-1][compteur2-2].bloc=terrain[compteur][compteur2].bloc;
        terrain[compteur-2][compteur2].bloc=terrain[compteur][compteur2].bloc;
        terrain[compteur][compteur2-2].bloc=terrain[compteur][compteur2].bloc;
        tab[joueur_actu].nb_block-=terrain[compteur][compteur2].bloc->aire;
        tab[joueur_actu].pts_action_actu-=terrain[compteur][compteur2].bloc->aire;
        sprintf(variable, "| Joueur %d pose un chateau en %d/%d",joueur_actu,compteur,compteur2);
        ajouter_ligne_bash(variable,tab_info_bash,info,variable2);
      }
    }
  }


  return;
}


/*
int muraille(case_t terrain[N][M],int joueur_actu){
  int nb=0;
  for(int i=0;i<N;i++){
    for(int j=0;j<M;j++){
      if(){
        nb++;
      }else{
        return 0;
      }
    }
  }
  return nb;
}*/



void update_grille(case_t terrain[N][M],int compteur_tour,bash_t tab_info_bash[TAILLE_TAB_BASH],char variable2[80]){
  char variable[80];
  if(compteur_tour%TOUR_EVOL_FORET==0){
    for(int i=0;i<N;i++){
      for(int j=0;j<M;j++){

        if(terrain[i][j].type==6 && rand()%CHANCE_EVOL==0){
          terrain[i][j].type=1;
          sprintf(variable, "| Foret en %d/%d  meurt",i,j);
          ajouter_ligne_bash(variable,tab_info_bash,info,variable2);
        }
        if(terrain[i][j].type==1 && rand()%CHANCE_EVOL==1 && terrain[i][j].bloc==NULL){
          terrain[i][j].type=6;
          sprintf(variable, "| Foret en %d/%d  nait",i,j);
          ajouter_ligne_bash(variable,tab_info_bash,info,variable2);
        }
      }
    }
  }
}







void initialisation_principale_iso(int bordure,SDL_Window * pWindow,int * largeur, int * hauteur,joueurs_t tab[J],degatx_t aff_deg[AFF_DEG],bash_t tab_info_bash[TAILLE_TAB_BASH],case_t terrain[N][M],char variable2[80],joueur_unite_t j_u[J]){
    int alea;
    SDL_Rect imgDestRect;
    int var1;
    FILE * fp;

    time_t secondes;
    struct tm instant;

    time(&secondes);
    instant=*localtime(&secondes);

    sprintf(variable2,"fichiers/log_%d-%d_%dh%dm%ds.txt", instant.tm_mday, instant.tm_mon+1, instant.tm_hour, instant.tm_min, instant.tm_sec);

  SDL_GetWindowSize(pWindow,largeur,hauteur);
  for(int i=0;i<TAILLE_TAB_BASH;i++){ //ini du tab bash
    tab_info_bash[i].pos_y=(265-(25*i));
    tab_info_bash[i].couleur=info;
    strcpy(tab_info_bash[i].txt,"PARTIE INITIALISE");
  }


  if(PRESET>1){
    fp = fopen ("fichiers/preset1.txt", "r"); //preset en 7x7
  }
  for(int i=0;i<N;i++){
    for(int j=0;j<M;j++){

      if(PRESET==1){			//======preset=======//

        if(i==0 && j==0){
          terrain[i][j].type=2;
        }
        else if(i==0 && j>0){
          terrain[i][j].type=4;
        }
        else if(i>0 && j==0){
          terrain[i][j].type=3;
        }
        else if((j==N-1 && i!=0 )||( i==M-1 && j!=0)){
          terrain[i][j].type=1;
        }
        else{												//ALEA "INTELLIGENTE"//
          alea=rand()%(N*M);
          if((alea<(0.05*N*M) )||( (i-1<N && terrain[i-1][j].type==5 && alea<(0.4*N*M)) || (j-1<N && terrain[i][j-1].type==5 && alea<(0.4*N*M)) || (i+1<N && terrain[i+1][j].type==5 && alea<(0.4*N*M)) || (i<N && terrain[i][j+1].type==5 && alea<(0.4*N*M)) )){
            terrain[i][j].type=5;

          }
          else if(((alea<(0.15*N*M) && alea>(0.05*N*M) )|| (i-1<N && terrain[i-1][j].type==7 && alea<(0.45*N*M)) || (j-1<N && terrain[i][j-1].type==7 && alea<(0.45*N*M)) || (i+1<N && terrain[i+1][j].type==7 && alea<(0.45*N*M)) || (i<N && terrain[i][j+1].type==7 && alea<(0.45*N*M)) )){
            terrain[i][j].type=7;
          }
          else if(((alea<(0.25*N*M) && alea>(0.15*N*M) )|| (i-1<N && terrain[i-1][j].type==6 && alea<(0.55*N*M)) || (j-1<N && terrain[i][j-1].type==6 && alea<(0.55*N*M)) || (i+1<N && terrain[i+1][j].type==6 && alea<(0.55*N*M)) || (i<N && terrain[i][j+1].type==6 && alea<(0.55*N*M)) )){
            terrain[i][j].type=6;
          }else{
            terrain[i][j].type=1;
          }
        }

      }else{						//=====preset de carte chargé par fichiers=====//

        fscanf(fp, "%i", &var1);
        terrain[i][j].type=var1;
      }

    }

  }
  carte_valide(terrain);
  if(PRESET>1){
    fclose(fp);
  }

  //initialisation de la matrice terrain,variables d'affichages;

    for(int i=0;i<N;i++){
      for(int j=0;j<M;j++){
        terrain[i][j].climat=0;
        terrain[i][j].xImg=0;
        terrain[i][j].yImg=0;
        terrain[i][j].x1=0;
        terrain[i][j].y1=0;
        terrain[i][j].x2=0;
        terrain[i][j].y2=0;
        terrain[i][j].x3=0;
        terrain[i][j].y3=0;
        terrain[i][j].x4=0;
        terrain[i][j].y4=0;
        terrain[i][j].piece=NULL;
        terrain[i][j].deplacement=0;
        terrain[i][j].attaque=0;
        terrain[i][j].block=0;
        terrain[i][j].bloc=NULL;
      }
    }

  //initialisation tableau d'affichage des degats morts et soins

    for(int i=0;i<(AFF_DEG);i++){
      aff_deg[i].time=-1;
      aff_deg[i].pos_x=0;
      aff_deg[i].pos_y=0;
      aff_deg[i].c=0;
      strcpy(aff_deg[i].txt,"");
    }

    //initialisation de la grille//

    (imgDestRect.w)=150;
    (imgDestRect.h)=75;                                                                                                                   //les coef ci dessous permettent d'aligner les images car elle ne sont pas totalement droite dans le sprite
    for(int compteur=0, indice = (*hauteur)/2, indice2=(*largeur)/2; compteur<N; compteur++,indice-=(bordure+(imgDestRect.h)/2)/2.38,indice2-=(bordure+(imgDestRect.w)/2)/1.48){
      for(int compteur2=0, indice3= indice , indice4 = indice2; compteur2<M; compteur2++,indice3-=(bordure+(imgDestRect.h)/2)/1.82,indice4+=(bordure+(imgDestRect.w)/2)/2){

          imgDestRect.x = indice4;
          imgDestRect.y = indice3;
          imgDestRect.w=150;
          imgDestRect.h=75;

          if(!terrain[compteur][compteur2].x1 || !terrain[compteur][compteur2].x2 || !terrain[compteur][compteur2].y1 || !terrain[compteur][compteur2].y2){

            terrain[compteur][compteur2].xImg=indice4;
            terrain[compteur][compteur2].yImg=indice3;
              //point en haut du losange puis les trois points suivant sont les sommet dans le sens des aiguille d'une montre.



            terrain[compteur][compteur2].x1=imgDestRect.x+imgDestRect.w/2;
            terrain[compteur][compteur2].y1=imgDestRect.y;

            terrain[compteur][compteur2].x2=imgDestRect.x+imgDestRect.w;
            terrain[compteur][compteur2].y2=imgDestRect.y+imgDestRect.h/2;

            terrain[compteur][compteur2].x3=imgDestRect.x+imgDestRect.w/2;
            terrain[compteur][compteur2].y3=imgDestRect.y+imgDestRect.h;

            terrain[compteur][compteur2].x4=imgDestRect.x;
            terrain[compteur][compteur2].y4=imgDestRect.y+imgDestRect.h/2;

          }
        }
      }


  //=========================================================INITIALISATION DES JOUEURS=============================================//

      for(int i=0;i<J;i++){
        if(i<J_HUMAIN){
            tab[i].humain=1;
        }else{
          tab[i].humain=0;
        }
        tab[i].id_joueur=i;
        tab[i].pts_action_max= NB_UNITE*PTS_ACTION_UNI;
        tab[i].pts_action_actu=tab[i].pts_action_max;
        tab[i].nb_unite=NB_UNITE;
        tab[i].nb_block=NB_BLOCK;
        tab[i].nb_gold=NB_GOLD;
	      tab[i].nb_bois=NB_BOIS;
      }


  //=========================================================INITIALISATION DES UNITEES===========================================//

  //alea
  /*
      int unit_gen;
      for(int i=0;i<J;i++){
        unit_gen=0;
        while(unit_gen<tab[i].nb_unite){
          for(int compteur=0;compteur<N; compteur++){
            for(int compteur2=0;compteur2<M; compteur2++){
              if(terrain[compteur][compteur2].piece==NULL && terrain[compteur][compteur2].type != 5){
                if(rand()%(N*M) ==1 && unit_gen<tab[i].nb_unite){
                  terrain[compteur][compteur2].piece=init_piece(((rand()%NB_CLASSE)+1),i); //(((rand()%NB_CLASSE)+1),i)
                  unit_gen++;
                }
              }
            }
          }
        }
      }
*/
  //intelligente

    if(PRESET_U==1){
      int nb_place=0;
      int porte=0;
      int unit_gen;
      int ini=0;
      for(int i=0;i<J;i++){
        do{
          rem_piece_joueur(terrain,i);
          nb_place=tab[i].nb_unite;
          unit_gen=0;
          while(unit_gen==0){
            for(int compteur=0;compteur<N; compteur++){
              for(int compteur2=0;compteur2<M; compteur2++){
                if(zone_libre(terrain,compteur,compteur2,nb_place)){
                  if(rand()%(N*M) ==1){
                    if(nb_place==1)porte=0;
                    else if(nb_place>=2 && nb_place<=9)porte=1;
                    else if(nb_place>=10 && nb_place<=25)porte=2;
                    else if(nb_place>=26 && nb_place<=49)porte=3;
                    fprintf(stderr, "joueur %d : restant %d\n",i, j_u[i].restant_unite);
                    for(int m = 0; m < NB_CLASSE; m++){
                      if(j_u[i].unit[m]>0){
                        for(int n = 0; n < j_u[i].unit[m]; n++){
                          ini=0;
                          for(int a=compteur-porte;a<=compteur+porte;a++){
                            for(int b=compteur2-porte;b<=compteur2+porte;b++){
                              if(a<N && a>=0 && b<M && b>=0 && case_libre(terrain,a,b) && unit_gen<nb_place && ini==0){
                                  terrain[a][b].piece=init_piece(m+1,i);
                                  unit_gen++;
                                  ini=1;
                                  fprintf(stderr, "ajout %d : %d\n", i, n);
                              }
                            }
                          }
                        }
                      }
                    }
                    if(j_u[i].restant_unite > 0){
                      for(int j = 0 ; j < j_u[i].restant_unite;j++){
                        for(int a=compteur-porte;a<=compteur+porte;a++){
                          for(int b=compteur2-porte;b<=compteur2+porte;b++){
                            if(a<N && a>=0 && b<M && b>=0 && case_libre(terrain,a,b) && unit_gen<nb_place){
                                terrain[a][b].piece=init_piece(((rand()%NB_CLASSE)+1),i);
                                unit_gen++;
                                fprintf(stderr, "ajout %d : %d\n", i, j);
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }while(nombre_classe(terrain,i,priest)>NB_MAX_PRIEST);
      }
    }else{
      int var_classe,var_joueur;
      fp = fopen ("fichiers/preset1_U.txt", "r"); //preset d'unité en 7x7
      for(int a=0;a<N;a++){
        for(int b=0;b<M;b++){
          fscanf(fp, "%i", &var_classe);
          fscanf(fp, "%i", &var_joueur);
          if(var_classe!=0 && var_joueur!= -1){
            terrain[a][b].piece=init_piece(var_classe,var_joueur);
          }
        }
      }
      fclose(fp);


    }
    

}






void af_erreur(SDL_Renderer *renderer,SDL_Window* pWindow,int *hauteur,int *largeur,int *compteur_erreur,image_t image[Z]){
  SDL_Color c = {0,0,0,0};
  SDL_Rect rect;
  if((*compteur_erreur)>0){
    afficher_img((*largeur)/2-200,(*hauteur)/2-100,200,400,"images/menu_nord.png",image,renderer,1,0,rect);
			AfficherText("ACTION IMPOSSIBLE","arial.ttf",c,30,renderer,(*largeur)/2-200+55,(*hauteur)/2+65-85);
    (*compteur_erreur)--;
  }
}




void affichage_principale_iso(SDL_Renderer *renderer,SDL_Window* pWindow,int bordure,int * largeur,int * hauteur,joueurs_t tab[J],degatx_t aff_deg[AFF_DEG],bash_t tab_info_bash[TAILLE_TAB_BASH],case_t terrain[N][M],int joueur_actu,image_t image[Z],int compteur_anim,int nb_joueur_restant,int compteur_tour,int frame_anim_montre,int jour_nuit,char variable2[80],int compteur_bouton_cam,int bloc_actu,SDL_Event e,page_t page_actu){
  //=================================================================================================================================//
						//=================================================================================================================================//
						//====================================================AFFICHAGE====================================================================//
						//=================================================================================================================================//
						//=================================================================================================================================//
	SDL_Color c = {0,0,0,0};
	SDL_Color c_rouge = {255,0,0,0};
	SDL_Color c_verte = {125,255,125,0};
	SDL_Color c_verte2 = {15,109,15,0};
	SDL_Color b = {255,255,255,255};
	SDL_Color c_bash = {255,0,255,0};
	SDL_Color c_bash_degat = {150,15,15,0};
	SDL_Color c_bash_soin = {15,150,15,0};
	SDL_Color c_bash_ineficace = {175,175,175,0};
	SDL_Color c_bash_deplacement = {255,255,255,0};
	SDL_Color c_bash_kill = {255,0,0,0};

  	int test_bash=0,varBash;
    float coefZoom=1;
    SDL_Rect img_anim;
	  char variable[80];


									SDL_RenderClear(renderer);

									SDL_GetWindowSize(pWindow,largeur,hauteur);
									afficher_img(0,0,*hauteur,*largeur,"images/fond.png",image,renderer,1,0,img_anim);

									for (int compteur=N-1;compteur>=0;compteur--){
										for (int compteur2=M-1;compteur2>=0;compteur2--){

											if(terrain[compteur][compteur2].type==1){
												afficher_img(terrain[compteur][compteur2].xImg,terrain[compteur][compteur2].yImg,75,150,"images/case_normalv2.png",image,renderer,coefZoom,0,img_anim);
											}
											else if(terrain[compteur][compteur2].type==2){
												afficher_img(terrain[compteur][compteur2].xImg,terrain[compteur][compteur2].yImg,75,150,"images/case_normal_coinv2.png",image,renderer,coefZoom,0,img_anim);
											}
											else if(terrain[compteur][compteur2].type==3){
												afficher_img(terrain[compteur][compteur2].xImg,terrain[compteur][compteur2].yImg,75,150,"images/case_normal_bordure_gauchev2.png",image,renderer,coefZoom,0,img_anim);
											}
											else if(terrain[compteur][compteur2].type==4){
												afficher_img(terrain[compteur][compteur2].xImg,terrain[compteur][compteur2].yImg,75,150,"images/case_normal_bordure_droitev2.png",image,renderer,coefZoom,0,img_anim);
											}
											else if(terrain[compteur][compteur2].type==5){
												afficher_img(terrain[compteur][compteur2].xImg,terrain[compteur][compteur2].yImg,75,150,"images/case_eauv2.png",image,renderer,coefZoom,0,img_anim);
											}
											else if(terrain[compteur][compteur2].type==6){
												afficher_img(terrain[compteur][compteur2].xImg,terrain[compteur][compteur2].yImg-175,250,150,"images/case_foretv2ISO.png",image,renderer,coefZoom,0,img_anim);
											}
											else if(terrain[compteur][compteur2].type==7){
												afficher_img(terrain[compteur][compteur2].xImg,terrain[compteur][compteur2].yImg-175,250,150,"images/case_montagnev2ISO.png",image,renderer,coefZoom,0,img_anim);
											}
										}
									}

									for (int compteur=0;compteur<N;compteur++){
										for (int compteur2=0;compteur2<M;compteur2++){
											if(terrain[compteur][compteur2].piece){
												if(terrain[compteur][compteur2].piece->select){		//affichage case possible déplacement ou attaque si allié, sinon affiche le stats de la cible et sa direction d'arret;
													reset_block(terrain);
                          if(terrain[compteur][compteur2].piece->joueur==joueur_actu){
																			//deplacement
														pathfinding(terrain,compteur,compteur2);

														for (int ind=0;ind<N;ind++){
															for (int ind2=0;ind2<M;ind2++){
																if(terrain[ind][ind2].deplacement==1 && (compteur_anim%(VITESSE_ANIM*2))<(VITESSE_ANIM/2)*2){

																	afficher_img(terrain[ind][ind2].xImg-bordure/15+8,terrain[ind][ind2].yImg-bordure/8+3,75,150,"images/deplacement.png",image,renderer,coefZoom,0,img_anim);

																}else if(terrain[ind][ind2].deplacement==1 && (compteur_anim%(VITESSE_ANIM*2))>=(VITESSE_ANIM/2)*2){

																	afficher_img(terrain[ind][ind2].xImg-bordure/15+8,terrain[ind][ind2].yImg-bordure/8+3,75,150,"images/deplacement2.png",image,renderer,coefZoom,0,img_anim);

																}
															}
														}
															//attaque
														pathfinding_combat(terrain,compteur,compteur2,joueur_actu);

														for (int ind=0;ind<N;ind++){
															for (int ind2=0;ind2<M;ind2++){
																if(terrain[ind][ind2].attaque==1 && ((compteur_anim%VITESSE_ANIM)<(VITESSE_ANIM/2) || (compteur_anim%VITESSE_ANIM)>((VITESSE_ANIM/4)*3)) ){

																	afficher_img(terrain[ind][ind2].xImg-bordure/15+8,terrain[ind][ind2].yImg-bordure/8+3,75,150,"images/attaque.png",image,renderer,coefZoom,0,img_anim);

																}else if(terrain[ind][ind2].attaque==1 && ((compteur_anim%VITESSE_ANIM)>(VITESSE_ANIM/2) || (compteur_anim%VITESSE_ANIM)<=((VITESSE_ANIM/4)*3)) ){

																	afficher_img(terrain[ind][ind2].xImg-bordure/15+8,terrain[ind][ind2].yImg-bordure/8+3,75,150,"images/attaque2.png",image,renderer,coefZoom,0,img_anim);

																}
															}
														}
													}
												}
											}
										}
									}

                  for (int ind=0;ind<N;ind++){
                    for (int ind2=0;ind2<M;ind2++){
                      if(terrain[ind][ind2].block==1 && (compteur_anim%(VITESSE_ANIM*2))<(VITESSE_ANIM/2)*2){

                        afficher_img(terrain[ind][ind2].xImg-bordure/15+8,terrain[ind][ind2].yImg-bordure/8+3,75,150,"images/block.png",image,renderer,coefZoom,0,img_anim);

                      }else if(terrain[ind][ind2].block==1 && (compteur_anim%(VITESSE_ANIM*2))>=(VITESSE_ANIM/2)*2){

                        afficher_img(terrain[ind][ind2].xImg-bordure/15+8,terrain[ind][ind2].yImg-bordure/8+3,75,150,"images/block2.png",image,renderer,coefZoom,0,img_anim);

                      }
                    }
                  }

									for (int compteur=0;compteur<N;compteur++){
										for (int compteur2=0;compteur2<M;compteur2++){
											if(terrain[compteur][compteur2].piece){ 		//AFFICHAGE DES CERCLE UNITES ALLIEES ET UNITES SELECTIONNEES

												if(terrain[compteur][compteur2].piece->joueur==joueur_actu && terrain[compteur][compteur2].piece->select==0){ //cercle des unité du joueur_actu affichage
													afficher_img(terrain[compteur][compteur2].xImg+25,terrain[compteur][compteur2].yImg-15,100,100,"images/image_allie.png",image,renderer,coefZoom,0,img_anim);
												}else if(terrain[compteur][compteur2].piece->joueur==joueur_actu && terrain[compteur][compteur2].piece->select==1){ //cercle de l'unité selectionné du joueur_actu
													afficher_img(terrain[compteur][compteur2].xImg+25,terrain[compteur][compteur2].yImg-15,100,100,"images/image_select_allie.png",image,renderer,coefZoom,0,img_anim);
												}
											}
										}
									}


									for (int compteur=N-1;compteur>=0;compteur--){
										for (int compteur2=M-1;compteur2>=0;compteur2--){
                      if(terrain[compteur][compteur2].type==6 && (terrain[compteur][compteur2].deplacement==1 || terrain[compteur][compteur2].attaque==1 || (compteur2+1<M && (terrain[compteur][compteur2+1].piece || terrain[compteur][compteur2+1].deplacement || terrain[compteur][compteur2+1].attaque)) || (compteur+1<N && (terrain[compteur+1][compteur2].piece || terrain[compteur+1][compteur2].deplacement || terrain[compteur+1][compteur2].attaque)) || (compteur2+1<M && compteur+1<N && (terrain[compteur+1][compteur2+1].piece || terrain[compteur+1][compteur2+1].deplacement || terrain[compteur+1][compteur2+1].attaque)) || (compteur2+2<M && compteur+1<N && (terrain[compteur+1][compteur2+2].piece || terrain[compteur+1][compteur2+2].deplacement || terrain[compteur+1][compteur2+2].attaque)) || (terrain[compteur][compteur2].piece && (terrain[compteur][compteur2].piece->select==1)))){
                        afficher_img(terrain[compteur][compteur2].xImg,terrain[compteur][compteur2].yImg-175,250,150,"images/arbreISO_trans.png",image,renderer,coefZoom,0,img_anim);
                      }else if(terrain[compteur][compteur2].type==6){
												afficher_img(terrain[compteur][compteur2].xImg,terrain[compteur][compteur2].yImg-175,250,150,"images/arbreISO.png",image,renderer,coefZoom,0,img_anim);
                      }if(terrain[compteur][compteur2].type==7 && (terrain[compteur][compteur2].deplacement==1 || terrain[compteur][compteur2].attaque==1 || (compteur2+1<M && (terrain[compteur][compteur2+1].piece || terrain[compteur][compteur2+1].deplacement || terrain[compteur][compteur2+1].attaque)) || (compteur+1<N && (terrain[compteur+1][compteur2].piece || terrain[compteur+1][compteur2].deplacement || terrain[compteur+1][compteur2].attaque)) || (compteur2+1<M && compteur+1<N && (terrain[compteur+1][compteur2+1].piece || terrain[compteur+1][compteur2+1].deplacement || terrain[compteur+1][compteur2+1].attaque)) || (compteur2+2<M && compteur+1<N && (terrain[compteur+1][compteur2+2].piece || terrain[compteur+1][compteur2+2].deplacement || terrain[compteur+1][compteur2+2].attaque)) || (terrain[compteur][compteur2].piece && (terrain[compteur][compteur2].piece->select==1)))){
                        afficher_img(terrain[compteur][compteur2].xImg,terrain[compteur][compteur2].yImg-175,250,150,"images/montagneISO_trans.png",image,renderer,coefZoom,0,img_anim);
                      }else if(terrain[compteur][compteur2].type==7){
												afficher_img(terrain[compteur][compteur2].xImg,terrain[compteur][compteur2].yImg-175,250,150,"images/montagneISO.png",image,renderer,coefZoom,0,img_anim);
                      }
											if(terrain[compteur][compteur2].piece){ 		//AFFICHAGE DES UNITEES
												switch (terrain[compteur][compteur2].piece->classe){
													case 1:

													img_anim=afficher_anim(compteur_anim,terrain[compteur][compteur2].piece->classe,terrain,compteur,compteur2,0);
													afficher_img(terrain[compteur][compteur2].xImg+25,terrain[compteur][compteur2].yImg-30,100,100,"images/knight_anim.png",image,renderer,coefZoom,1,img_anim);

													break;
													case 2:

													img_anim=afficher_anim(compteur_anim,terrain[compteur][compteur2].piece->classe,terrain,compteur,compteur2,0);
													afficher_img(terrain[compteur][compteur2].xImg+25,terrain[compteur][compteur2].yImg-30,100,100,"images/scout_anim.png",image,renderer,coefZoom,1,img_anim);

													break;
													case 3:

													img_anim=afficher_anim(compteur_anim,terrain[compteur][compteur2].piece->classe,terrain,compteur,compteur2,0);
													afficher_img(terrain[compteur][compteur2].xImg+25,terrain[compteur][compteur2].yImg-30,100,100,"images/priest_anim.png",image,renderer,coefZoom,1,img_anim);

													break;
													case 4:

													img_anim=afficher_anim(compteur_anim,terrain[compteur][compteur2].piece->classe,terrain,compteur,compteur2,0);
													afficher_img(terrain[compteur][compteur2].xImg+25,terrain[compteur][compteur2].yImg-30,100,100,"images/magician_anim.png",image,renderer,coefZoom,1,img_anim);

													break;
													case 5:

													afficher_img(terrain[compteur][compteur2].xImg+25,terrain[compteur][compteur2].yImg-30,100,100,"images/faucheuse.png",image,renderer,coefZoom,0,img_anim);

													break;
													case 6:

													img_anim=afficher_anim(compteur_anim,terrain[compteur][compteur2].piece->classe,terrain,compteur,compteur2,0);
													afficher_img(terrain[compteur][compteur2].xImg+25,terrain[compteur][compteur2].yImg-30,100,100,"images/ange_anim.png",image,renderer,coefZoom,1,img_anim);

													break;
												}
											}
                      //activer les buisson transparent ci dessous
                      //if(terrain[compteur][compteur2].type==6 && (terrain[compteur][compteur2].deplacement==1 || terrain[compteur][compteur2].attaque==1 || (compteur2+1<M && terrain[compteur][compteur2+1].piece) || (compteur+1<N && terrain[compteur+1][compteur2].piece) || (compteur2+1<M && compteur+1<N && terrain[compteur+1][compteur2+1].piece) || (compteur2+2<M && compteur+1<N && terrain[compteur+1][compteur2+2].piece))){
                        //afficher_img(terrain[compteur][compteur2].xImg,terrain[compteur][compteur2].yImg-175,250,150,"images/buissonISO_trans.png",image,renderer,coefZoom,0,img_anim);
                      //}
                      if(terrain[compteur][compteur2].type==6){
												afficher_img(terrain[compteur][compteur2].xImg,terrain[compteur][compteur2].yImg-175,250,150,"images/buissonISO.png",image,renderer,coefZoom,0,img_anim);
                      }
                      else if(terrain[compteur][compteur2].type==7){
												afficher_img(terrain[compteur][compteur2].xImg,terrain[compteur][compteur2].yImg-175,250,150,"images/rocherISO.png",image,renderer,coefZoom,0,img_anim);
                      }
                      if(terrain[compteur][compteur2].bloc!=NULL && terrain[compteur][compteur2].bloc->type==1 &&(terrain[compteur][compteur2].deplacement==1 || terrain[compteur][compteur2].attaque==1 || (compteur2+1<M && (terrain[compteur][compteur2+1].piece || terrain[compteur][compteur2+1].deplacement || terrain[compteur][compteur2+1].attaque)) || (compteur+1<N && (terrain[compteur+1][compteur2].piece || terrain[compteur+1][compteur2].deplacement || terrain[compteur+1][compteur2].attaque)) || (compteur2+1<M && compteur+1<N && (terrain[compteur+1][compteur2+1].piece || terrain[compteur+1][compteur2+1].deplacement || terrain[compteur+1][compteur2+1].attaque)) || (compteur2+2<M && compteur+1<N && (terrain[compteur+1][compteur2+2].piece || terrain[compteur+1][compteur2+2].deplacement || terrain[compteur+1][compteur2+2].attaque)) || (compteur2+2<M && compteur+2<N && (terrain[compteur+2][compteur2+2].piece || terrain[compteur+2][compteur2+2].deplacement || terrain[compteur+2][compteur2+2].attaque)) || terrain[compteur][compteur2].piece)){
                        afficher_img(terrain[compteur][compteur2].xImg,terrain[compteur][compteur2].yImg-165,250,150,"images/bloc_trans.png",image,renderer,1,0,img_anim);
                      }else if(terrain[compteur][compteur2].bloc!=NULL && terrain[compteur][compteur2].bloc->type==1){
                        afficher_img(terrain[compteur][compteur2].xImg,terrain[compteur][compteur2].yImg-165,250,150,"images/bloc.png",image,renderer,1,0,img_anim);

                      }else if(terrain[compteur][compteur2].bloc!=NULL && terrain[compteur][compteur2].bloc->type==2 &&(terrain[compteur][compteur2].deplacement==1 || terrain[compteur][compteur2].attaque==1 || (compteur2+1<M && (terrain[compteur][compteur2+1].piece || terrain[compteur][compteur2+1].deplacement || terrain[compteur][compteur2+1].attaque)) || (compteur2+1<M && compteur+2<N && (terrain[compteur+2][compteur2+1].piece || terrain[compteur+2][compteur2+1].deplacement || terrain[compteur+2][compteur2+1].attaque)) || (compteur2+2<M && (terrain[compteur][compteur2+2].piece || terrain[compteur][compteur2+2].deplacement || terrain[compteur][compteur2+2].attaque)) || (compteur+2<N && (terrain[compteur+2][compteur2].piece || terrain[compteur+2][compteur2].deplacement || terrain[compteur+2][compteur2].attaque)) || (compteur+1<N && (terrain[compteur+1][compteur2].piece || terrain[compteur+1][compteur2].deplacement || terrain[compteur+1][compteur2].attaque)) || (compteur2+1<M && compteur+1<N && (terrain[compteur+1][compteur2+1].piece || terrain[compteur+1][compteur2+1].deplacement || terrain[compteur+1][compteur2+1].attaque)) || (compteur2+2<M && compteur+1<N && (terrain[compteur+1][compteur2+2].piece || terrain[compteur+1][compteur2+2].deplacement || terrain[compteur+1][compteur2+2].attaque)) || (compteur2+2<M && compteur+2<N && (terrain[compteur+2][compteur2+2].piece || terrain[compteur+2][compteur2+2].deplacement || terrain[compteur+2][compteur2+2].attaque)) || terrain[compteur][compteur2].piece)){
                        if(compteur+2>=0 && compteur2+2>=0 && terrain[compteur+1][compteur2].bloc && terrain[compteur+1][compteur2+1].bloc && terrain[compteur][compteur2+1].bloc && (terrain[compteur][compteur2+1].bloc)==(terrain[compteur][compteur2].bloc) && (terrain[compteur+1][compteur2+1].bloc)==(terrain[compteur][compteur2].bloc) && (terrain[compteur+1][compteur2].bloc)==(terrain[compteur][compteur2].bloc) && (terrain[compteur+2][compteur2].bloc)==(terrain[compteur][compteur2].bloc) && (terrain[compteur+2][compteur2+1].bloc)==(terrain[compteur][compteur2].bloc) && (terrain[compteur+2][compteur2+2].bloc)==(terrain[compteur][compteur2].bloc) && (terrain[compteur+1][compteur2+2].bloc)==(terrain[compteur][compteur2].bloc) && (terrain[compteur][compteur2+2].bloc)==(terrain[compteur][compteur2].bloc)){
                          afficher_img(terrain[compteur][compteur2].xImg-200,terrain[compteur][compteur2].yImg-262,330,550,"images/chateau_trans.png",image,renderer,1,0,img_anim);
                        }
                      }else if(terrain[compteur][compteur2].bloc!=NULL && terrain[compteur][compteur2].bloc->type==2){
                        if(compteur+2>=0 && compteur2+2>=0 && terrain[compteur+1][compteur2].bloc && terrain[compteur+1][compteur2+1].bloc && terrain[compteur][compteur2+1].bloc && (terrain[compteur][compteur2+1].bloc)==(terrain[compteur][compteur2].bloc) && (terrain[compteur+1][compteur2+1].bloc)==(terrain[compteur][compteur2].bloc) && (terrain[compteur+1][compteur2].bloc)==(terrain[compteur][compteur2].bloc) && (terrain[compteur+2][compteur2].bloc)==(terrain[compteur][compteur2].bloc) && (terrain[compteur+2][compteur2+1].bloc)==(terrain[compteur][compteur2].bloc) && (terrain[compteur+2][compteur2+2].bloc)==(terrain[compteur][compteur2].bloc) && (terrain[compteur+1][compteur2+2].bloc)==(terrain[compteur][compteur2].bloc) && (terrain[compteur][compteur2+2].bloc)==(terrain[compteur][compteur2].bloc)){
                          afficher_img(terrain[compteur][compteur2].xImg-200,terrain[compteur][compteur2].yImg-262,330,550,"images/chateau.png",image,renderer,1,0,img_anim);
                        }
                      }
										}
									}




									//affichage des fleche de deplacement transparentes sur les ennemis a portee de l'allie selectionné si il y en a un
									for (int compteur=0;compteur<N;compteur++){
										for (int compteur2=0;compteur2<M;compteur2++){
											if(terrain[compteur][compteur2].piece && terrain[compteur][compteur2].piece->select == 1 && terrain[compteur][compteur2].piece->joueur==joueur_actu && terrain[compteur][compteur2].piece->classe!=priest){
												for (int i=0;i<N;i++){
													for (int j=0;j<M;j++){
														if(terrain[i][j].attaque==1 && terrain[i][j].piece){
															switch(terrain[i][j].piece->direction){
																case 1:
																	afficher_img(terrain[i][j].xImg+35,terrain[i][j].yImg-10,60,120,"images/fleche_nord_trans.png",image,renderer,1,0,img_anim);
																break;
																case 2:
																	afficher_img(terrain[i][j].xImg+10,terrain[i][j].yImg-10,60,120,"images/fleche_sud_trans.png",image,renderer,1,0,img_anim);
																break;
																case 3:
																	afficher_img(terrain[i][j].xImg+10,terrain[i][j].yImg-10,60,120,"images/fleche_ouest_trans.png",image,renderer,1,0,img_anim);
																break;
																case 4:
																	afficher_img(terrain[i][j].xImg+10,terrain[i][j].yImg-10,60,120,"images/fleche_est_trans.png",image,renderer,1,0,img_anim);
																break;
															}
														}
													}
												}
											}
										}
									}


                  //nuit et jour//
                  if((jour_nuit>=((DURE_JOUR_NUIT/24)*22) && jour_nuit<=(DURE_JOUR_NUIT)+50) || (jour_nuit>=0 && jour_nuit<=((DURE_JOUR_NUIT/24)*2) )){
                    afficher_img(0,0,*hauteur,*largeur,"images/nuit-3.png",image,renderer,1,0,img_anim);
                  }else if((jour_nuit>=((DURE_JOUR_NUIT/24)*21) && jour_nuit<((DURE_JOUR_NUIT/24)*22) )|| (jour_nuit>((DURE_JOUR_NUIT/24)*2) && jour_nuit<=((DURE_JOUR_NUIT/24)*3) )){
                    afficher_img(0,0,*hauteur,*largeur,"images/nuit-2.png",image,renderer,1,0,img_anim);
                  }else if((jour_nuit>=((DURE_JOUR_NUIT/24)*20) && jour_nuit<((DURE_JOUR_NUIT/24)*21) )|| (jour_nuit>((DURE_JOUR_NUIT/24)*3) && jour_nuit<=((DURE_JOUR_NUIT/24)*4) )){
                    afficher_img(0,0,*hauteur,*largeur,"images/nuit-1.png",image,renderer,1,0,img_anim);
                  }

                  //===========//



                  if(viser_block(terrain)){
                    if(bloc_actu==1){
                      afficher_img(e.motion.x-75,e.motion.y-190,250,150,"images/bloc_trans.png",image,renderer,1,0,img_anim);
                    }else if(bloc_actu==2){
                      afficher_img(e.motion.x-225,e.motion.y-145,335,550,"images/chateau_trans.png",image,renderer,1,0,img_anim);

                      //afficher_img(e.motion.x-155,e.motion.y-125,250,375,"images/chateau_trans.png",image,renderer,1,0,img_anim);
                    }
                  }


									//drap rouge en haut
									afficher_img(0,0,*hauteur,*largeur,"images/drap_rouge.png",image,renderer,1,0,img_anim);

									//fin du tour bouton

									afficher_img((*largeur)/3+420,15,50,150,"images/fin_du_tour.png",image,renderer,1,0,img_anim);

									//menu nord

									afficher_img((*largeur)/3,0,75,400,"images/menu_nord.png",image,renderer,1,0,img_anim);

									AfficherText("ACTION RESTANTE:","arial.ttf",c,12,renderer,(*largeur)/3+200,30);

									AfficherText("TOUR DU JOUEUR ","arial.ttf",c,12,renderer,(*largeur)/3+50,30);

									sprintf(variable, "%d",  tab[joueur_actu].pts_action_actu);

									AfficherText(variable,"arial.ttf",c,30,renderer,(*largeur)/3+325,18);

									sprintf(variable, "%d",  joueur_actu);

									AfficherText(variable,"arial.ttf",c,30,renderer,(*largeur)/3+165,18);
									//stats


									afficher_img(0,0,400,150,"images/inter.png",image,renderer,1,0,img_anim);

									AfficherText("PDV        :","arial.ttf",c,12,renderer,30,155);

									AfficherText("ATK        :","arial.ttf",c,12,renderer,30,175);

									AfficherText("DEF        :","arial.ttf",c,12,renderer,30,195);

									AfficherText("BLOCK  :","arial.ttf",c,12,renderer,30,215);

									AfficherText("RANGE  :","arial.ttf",c,12,renderer,30,235);

									AfficherText("MS          :","arial.ttf",c,12,renderer,30,255);

									AfficherText("MENU", "arial.ttf",b,15,renderer,gpScreen->w - 60,gpScreen->h - 30);



									//infos générales de la partie
									afficher_img(150,0,200,300,"images/menu_nord.png",image,renderer,1,0,img_anim);

									AfficherText("Joueurs Restants :        ","arial.ttf",c,20,renderer,185,30);
									sprintf(variable, "%d",  nb_joueur_restant);
									AfficherText(variable,"arial.ttf",c,30,renderer,365,26);

									AfficherText("Unit Allies :        ","arial.ttf",c,20,renderer,185,80);
									sprintf(variable, "%d",  reste_allie(terrain,joueur_actu));
									AfficherText(variable,"arial.ttf",c,30,renderer,365,72);

									AfficherText("Unit Ennemis:        ","arial.ttf",c,20,renderer,185,130);
									sprintf(variable, "%d",  reste_ennemi(terrain,joueur_actu));
									AfficherText(variable,"arial.ttf",c,30,renderer,365,122);



            //===================================================================================================================//
            //===================================================================================================================//
            //==================================================MENU SUD=========================================================//
            //===================================================================================================================//       
            //===================================================================================================================//
            afficher_img(*largeur/4 *COEF_AFFICHAGE,*hauteur-110,100,750,"images/Menu_sud.png",image,renderer,1,0,img_anim);


                  sprintf(variable, "%d",tab[joueur_actu].nb_block);
									AfficherText(variable,"arial.ttf",c,20,renderer,(*largeur/4*COEF_AFFICHAGE)+55,*hauteur-95);

                  sprintf(variable, "%d",tab[joueur_actu].nb_gold);
									AfficherText(variable,"arial.ttf",c,20,renderer,(*largeur/4*COEF_AFFICHAGE)+170,*hauteur-95);

                  sprintf(variable, "%d",tab[joueur_actu].nb_bois);
									AfficherText(variable,"arial.ttf",c,20,renderer,(*largeur/4*COEF_AFFICHAGE)+285,*hauteur-95);







            //===================================================================================================================//
            //===================================================================================================================//
            //==================================================FIN MENU SUD=====================================================//
            //===================================================================================================================//       
            //===================================================================================================================//


						//==============BASH=======================//

									afficher_img(((*largeur)-450*COEF_AFFICHAGE),0,300,450*COEF_AFFICHAGE,"images/bash.png",image,renderer,1,0,img_anim);
									test_bash=0;
									for(int i=0;i<J;i++){
										if(tab[i].id_joueur==-2){
											varBash=i;
											test_bash=1;
										}
									}
									if(test_bash==1){
										sprintf(variable, "| Le Joueur %d a ete elimine.",varBash);
										ajouter_ligne_bash(variable,tab_info_bash,info,variable2);
									}

									for(int i=0;i<TAILLE_TAB_BASH;i++){   //AFFICHAGE DU BASH
										if(tab_info_bash[i].pos_y<=265 && tab_info_bash[i].pos_y>=5){
											switch(tab_info_bash[i].couleur){
												case 1: AfficherText(tab_info_bash[i].txt,"arial.ttf",c_bash,15*COEF_AFFICHAGE,renderer,((*largeur)-450*COEF_AFFICHAGE+25),tab_info_bash[i].pos_y);break;
												case 2: AfficherText(tab_info_bash[i].txt,"arial.ttf",c_bash_degat,15*COEF_AFFICHAGE,renderer,((*largeur)-450*COEF_AFFICHAGE+25),tab_info_bash[i].pos_y);break;
												case 3: AfficherText(tab_info_bash[i].txt,"arial.ttf",c_bash_soin,15*COEF_AFFICHAGE,renderer,((*largeur)-450*COEF_AFFICHAGE+25),tab_info_bash[i].pos_y);break;
												case 4: AfficherText(tab_info_bash[i].txt,"arial.ttf",c_bash_ineficace,15*COEF_AFFICHAGE,renderer,((*largeur)-450*COEF_AFFICHAGE+25),tab_info_bash[i].pos_y);break;
												case 5: AfficherText(tab_info_bash[i].txt,"arial.ttf",c_bash_deplacement,15*COEF_AFFICHAGE,renderer,((*largeur)-450*COEF_AFFICHAGE+25),tab_info_bash[i].pos_y);break;
												case 6: AfficherText(tab_info_bash[i].txt,"arial.ttf",c_bash_kill,15*COEF_AFFICHAGE,renderer,((*largeur)-450*COEF_AFFICHAGE+25),tab_info_bash[i].pos_y);break;
											}
										}
									}


				//============================================//

				//=======COMPTEUR JOUR======//



									img_anim.x=150*frame_anim_montre;
									img_anim.y=0;
									img_anim.w=150;
									img_anim.h=150;
									afficher_img(((*largeur)-160),410,150,150,"images/montre_anim.png",image,renderer,coefZoom,1,img_anim);
									AfficherText("JOUR","arial.ttf",c,20,renderer,((*largeur)-110),445);
									sprintf(variable, "%d",compteur_tour);
									AfficherText(variable,"arial.ttf",c,30,renderer,((*largeur)-102),490);



				//============================//

                  if(compteur_bouton_cam<=0){
                    afficher_img(10,420,100,100,"images/bouton_cam.png",image,renderer,1,0,img_anim);
                  }else{
                    afficher_img(10,420,100,100,"images/bouton_cam2.png",image,renderer,1,0,img_anim);
                  }

//===================================================BATIMENTS==========================================//

                  /*afficher_img(10,550,200,100,"images/block_menu.png",image,renderer,1,0,img_anim);
                  if(bloc_actu==1){
                    afficher_img(40,655,70,40,"images/bloc.png",image,renderer,1,0,img_anim);
                    sprintf(variable, "%d",1);
                    AfficherText(variable,"arial.ttf",c,20,renderer,48,723);
                  }else if(bloc_actu==2){
                    afficher_img(20,655,60,90,"images/chateau.png",image,renderer,1,0,img_anim);
                    sprintf(variable, "%d",9);
                    AfficherText(variable,"arial.ttf",c,20,renderer,48,723);
                  }

                  AfficherText("BLOCS :","arial.ttf",c,15,renderer,28,588);
                  sprintf(variable, "%d",tab[joueur_actu].nb_block);
									AfficherText(variable,"arial.ttf",c,30,renderer,38,625);

                  afficher_img(0,670,50,50,"images/gauche_bloc.png",image,renderer,1,0,img_anim);
                  afficher_img(70,670,50,50,"images/droite_bloc.png",image,renderer,1,0,img_anim);*/





//===========================================================================================================//


                  int ini_bloc=0;
									for(int i=0;i<N;i++){
										for(int j=0;j<M;j++){
											if(terrain[i][j].piece && terrain[i][j].piece->select==1){
												//affichage des infos de l'unité en haut a gauche.

												switch (terrain[i][j].piece->classe){
													case 1:

													img_anim=afficher_anim(compteur_anim,terrain[i][j].piece->classe,terrain,i,j,1);
													afficher_img(20,20,100,100,"images/knight_anim.png",image,renderer,coefZoom,1,img_anim);

												break;
												case 2:

													img_anim=afficher_anim(compteur_anim,terrain[i][j].piece->classe,terrain,i,j,1);
													afficher_img(20,20,100,100,"images/scout_anim.png",image,renderer,coefZoom,1,img_anim);

												break;
												case 3:

													img_anim=afficher_anim(compteur_anim,terrain[i][j].piece->classe,terrain,i,j,1);
													afficher_img(20,20,100,100,"images/priest_anim.png",image,renderer,coefZoom,1,img_anim);

												break;
												case 4:

												img_anim=afficher_anim(compteur_anim,terrain[i][j].piece->classe,terrain,i,j,1);
												afficher_img(20,20,100,100,"images/magician_anim.png",image,renderer,coefZoom,1,img_anim);

												break;
												case 5:

												afficher_img(20,20,100,100,"images/faucheuse.png",image,renderer,1,0,img_anim);
												break;
												case 6:

													img_anim=afficher_anim(compteur_anim,terrain[i][j].piece->classe,terrain,i,j,1);
													afficher_img(20,20,100,100,"images/ange_anim.png",image,renderer,coefZoom,1,img_anim);

												break;
												}
												//text


															//affichage des stats de l'unité selectionné en fonction du terrain
												sprintf(variable, "%d",  terrain[i][j].piece->joueur);
												AfficherText(variable,"arial.ttf",c,25,renderer,65,110);

												sprintf(variable, "%d", terrain[i][j].piece->pdv);
												AfficherText(variable,"arial.ttf",c,12,renderer,90,155);

												sprintf(variable, "%d", terrain[i][j].piece->puissance);
												AfficherText(variable,"arial.ttf",c,12,renderer,90,175);

												if(terrain[i][j].type==6){
													sprintf(variable, "%d", (terrain[i][j].piece->armure+9));
													AfficherText(variable,"arial.ttf",c_verte2,12,renderer,90,195);
												}else{
													sprintf(variable, "%d", terrain[i][j].piece->armure);
													AfficherText(variable,"arial.ttf",c,12,renderer,90,195);
												}


												if(terrain[i][j].type==6){
													sprintf(variable, "%d", (terrain[i][j].piece->block+10));
													AfficherText(variable,"arial.ttf",c_verte2,12,renderer,90,215);
												}else{
													sprintf(variable, "%d", terrain[i][j].piece->block);
													AfficherText(variable,"arial.ttf",c,12,renderer,90,215);
												}

												if(terrain[i][j].type==7){
													sprintf(variable, "%d", (terrain[i][j].piece->portee+1));
													AfficherText(variable,"arial.ttf",c_verte2,12,renderer,90,235);
												}else{
													sprintf(variable, "%d", terrain[i][j].piece->portee);
													AfficherText(variable,"arial.ttf",c,12,renderer,90,235);
												}

												if(terrain[i][j].type==6 || terrain[i][j].type==7){
													sprintf(variable, "%d", (terrain[i][j].piece->deplacement-1));
													AfficherText(variable,"arial.ttf",c_rouge,12,renderer,90,255);
												}else{
													sprintf(variable, "%d", terrain[i][j].piece->deplacement);
													AfficherText(variable,"arial.ttf",c,12,renderer,90,255);
												}



												//affichage de nombre de kills actuel de l'unité si ce n'est pas un pretre
												if(terrain[i][j].piece->classe!=priest){
													afficher_img(106,48,30,30,"images/kills.png",image,renderer,1,0,img_anim);
													sprintf(variable, "%d", terrain[i][j].piece->kill);
													AfficherText(variable,"arial.ttf",c_rouge,24,renderer,114,75);
												}

                        sprintf(variable, "%d", terrain[i][j].piece->pts_action_actu);
												AfficherText(variable,"arial.ttf",c,20,renderer,20,75);

												//DIRECTION affichage


												switch(terrain[i][j].piece->direction){
													case 1:
														afficher_img(25,275,100,100,"images/fleche_nord.png",image,renderer,1,0,img_anim);
													break;
													case 2:
														afficher_img(25,275,100,100,"images/fleche_sud.png",image,renderer,1,0,img_anim);
													break;
													case 3:
														afficher_img(25,275,100,100,"images/fleche_ouest.png",image,renderer,1,0,img_anim);
													break;
													case 4:
														afficher_img(25,275,100,100,"images/fleche_est.png",image,renderer,1,0,img_anim);
													break;
												}


											}else if(terrain[i][j].bloc && terrain[i][j].bloc->block_sel==1 && terrain[i][j].bloc->type==1){
                        afficher_img(35,20,100,66,"images/bloc.png",image,renderer,1,0,img_anim);

                        sprintf(variable, "%d",  terrain[i][j].bloc->block_allie);
												AfficherText(variable,"arial.ttf",c,25,renderer,65,110);

												sprintf(variable, "%d", terrain[i][j].bloc->pdv_block);
												AfficherText(variable,"arial.ttf",c,12,renderer,90,155);

												sprintf(variable, "%d", terrain[i][j].bloc->armure);
												AfficherText(variable,"arial.ttf",c,12,renderer,90,195);
                      }else if(terrain[i][j].bloc && terrain[i][j].bloc->block_sel==1 && terrain[i][j].bloc->type==2 && ini_bloc==0){
                        afficher_img(15,35,80,130,"images/chateau.png",image,renderer,1,0,img_anim);

                        sprintf(variable, "%d",  terrain[i][j].bloc->block_allie);
												AfficherText(variable,"arial.ttf",c,25,renderer,65,110);

												sprintf(variable, "%d", terrain[i][j].bloc->pdv_block);
												AfficherText(variable,"arial.ttf",c,12,renderer,90,155);

												sprintf(variable, "%d", terrain[i][j].bloc->armure);
												AfficherText(variable,"arial.ttf",c,12,renderer,90,195);
                        ini_bloc=1;
                      }
                      
										}
									}

									for(int i=0;i<AFF_DEG;i++){ //affichage des dégats, morts soins etc
										if(aff_deg[i].time>0){
											if(aff_deg[i].c==1){//couleur degat
												AfficherText(aff_deg[i].txt,"arial.ttf",c_rouge,30,renderer,aff_deg[i].pos_x,aff_deg[i].pos_y);
											}else if(aff_deg[i].c==2){//couleur soin
												AfficherText(aff_deg[i].txt,"arial.ttf",c_verte,30,renderer,aff_deg[i].pos_x,aff_deg[i].pos_y);
                      }else if(aff_deg[i].c==3){//couleur kill
												AfficherText(aff_deg[i].txt,"arial.ttf",c_bash,30,renderer,aff_deg[i].pos_x,aff_deg[i].pos_y);
											}else{//couleur grise
												AfficherText(aff_deg[i].txt,"arial.ttf",c_bash_deplacement,30,renderer,aff_deg[i].pos_x,aff_deg[i].pos_y);
											}
										}
									}


                  //affichage de la page actu
                  if(page_actu==craft){
                    afficher_img(*largeur/6*COEF_AFFICHAGE,300,500,1000,"images/page_craft.png",image,renderer,1,0,img_anim);

									  AfficherText("1","arial.ttf",c,20,renderer,(*largeur/6*COEF_AFFICHAGE)+160,300+228);
                    AfficherText("9","arial.ttf",c,20,renderer,(*largeur/6*COEF_AFFICHAGE)+275,300+228);
                  
                  }else if(page_actu==recherche){
                    afficher_img(*largeur/6*COEF_AFFICHAGE,300,500,1000,"images/page_recherche.png",image,renderer,1,0,img_anim);

                  }



						//=================================================================================================================================//
						//=======================================================FIN=======================================================================//
						//====================================================AFFICHAGE====================================================================//
						//=======================================================...=======================================================================//
						//=================================================================================================================================//


}


void initialisation_principale(int bordure,SDL_Window * pWindow,int * largeur, int * hauteur,joueurs_t tab[J],degatx_t aff_deg[AFF_DEG],bash_t tab_info_bash[TAILLE_TAB_BASH],case_t terrain[N][M],char variable2[80]){
    int alea;
    SDL_Rect imgDestRect;
    int var1;
    FILE * fp;

    time_t secondes;
    struct tm instant;

    time(&secondes);
    instant=*localtime(&secondes);

    sprintf(variable2,"fichiers/log_%d-%d_%dh%dm%ds.txt", instant.tm_mday, instant.tm_mon+1, instant.tm_hour, instant.tm_min, instant.tm_sec);

  SDL_GetWindowSize(pWindow,largeur,hauteur);
  for(int i=0;i<TAILLE_TAB_BASH;i++){ //ini du tab bash
    tab_info_bash[i].pos_y=(265-(25*i));
    tab_info_bash[i].couleur=info;
    strcpy(tab_info_bash[i].txt,"PARTIE INITIALISE");
  }


  if(PRESET>1){
    fp = fopen ("fichiers/preset1.txt", "r"); //preset en 7x7
  }
  for(int i=0;i<N;i++){
    for(int j=0;j<M;j++){

      if(PRESET==1){			//======preset=======//

        if(i==0 && j==0){
          terrain[i][j].type=2;
        }
        else if(i==0 && j>0){
          terrain[i][j].type=4;
        }
        else if(i>0 && j==0){
          terrain[i][j].type=3;
        }
        else if((j==N-1 && i!=0 )||( i==M-1 && j!=0)){
          terrain[i][j].type=1;
        }
        else{												//ALEA "INTELLIGENTE"//
          alea=rand()%(N*M);
          if((alea<(0.05*N*M) )||( (i-1<N && terrain[i-1][j].type==5 && alea<(0.4*N*M)) || (j-1<N && terrain[i][j-1].type==5 && alea<(0.4*N*M)) || (i+1<N && terrain[i+1][j].type==5 && alea<(0.4*N*M)) || (i<N && terrain[i][j+1].type==5 && alea<(0.4*N*M)) )){
            terrain[i][j].type=5;

          }
          else if(((alea<(0.15*N*M) && alea>(0.05*N*M) )|| (i-1<N && terrain[i-1][j].type==7 && alea<(0.45*N*M)) || (j-1<N && terrain[i][j-1].type==7 && alea<(0.45*N*M)) || (i+1<N && terrain[i+1][j].type==7 && alea<(0.45*N*M)) || (i<N && terrain[i][j+1].type==7 && alea<(0.45*N*M)) )){
            terrain[i][j].type=7;
          }
          else if(((alea<(0.25*N*M) && alea>(0.15*N*M) )|| (i-1<N && terrain[i-1][j].type==6 && alea<(0.55*N*M)) || (j-1<N && terrain[i][j-1].type==6 && alea<(0.55*N*M)) || (i+1<N && terrain[i+1][j].type==6 && alea<(0.55*N*M)) || (i<N && terrain[i][j+1].type==6 && alea<(0.55*N*M)) )){
            terrain[i][j].type=6;
          }
          else{
            terrain[i][j].type=1;
          }
        }

      }else{						//=====preset de carte chargé par fichiers=====//

        fscanf(fp, "%i", &var1);
        terrain[i][j].type=var1;
      }

    }

  }
  carte_valide(terrain);
  if(PRESET>1){
    fclose(fp);
  }

  //initialisation de la matrice terrain,variables d'affichages;

    for(int i=0;i<N;i++){
      for(int j=0;j<M;j++){
        terrain[i][j].climat=0;
        terrain[i][j].xImg=0;
        terrain[i][j].yImg=0;
        terrain[i][j].x1=0;
        terrain[i][j].y1=0;
        terrain[i][j].x2=0;
        terrain[i][j].y2=0;
        terrain[i][j].x3=0;
        terrain[i][j].y3=0;
        terrain[i][j].x4=0;
        terrain[i][j].y4=0;
        terrain[i][j].piece=NULL;
        terrain[i][j].deplacement=0;
        terrain[i][j].attaque=0;
        terrain[i][j].block=0;
        terrain[i][j].bloc=NULL;

      }
    }

  //initialisation tableau d'affichage des degats morts et soins

    for(int i=0;i<(AFF_DEG);i++){
      aff_deg[i].time=-1;
      aff_deg[i].pos_x=0;
      aff_deg[i].pos_y=0;
      aff_deg[i].c=0;
      strcpy(aff_deg[i].txt,"");
    }

    //initialisation de la grille//

    (imgDestRect.w)=100;
    (imgDestRect.h)=100;                                                                                                                   //les coef ci dessous permettent d'aligner les images car elle ne sont pas totalement droite dans le sprite
    for(int compteur=0, indice = (*hauteur)/2, indice2=(*largeur)/2; compteur<N; compteur++,indice-=(bordure+(imgDestRect.h)/2)/2.1,indice2-=(bordure+(imgDestRect.w)/2)/1.5){
      for(int compteur2=0, indice3= indice , indice4 = indice2; compteur2<M; compteur2++,indice3-=(bordure+(imgDestRect.h)/2)/1.5,indice4+=(bordure+(imgDestRect.w)/2)/1.98){

          imgDestRect.x = indice4;
          imgDestRect.y = indice3;
          imgDestRect.w=100;
          imgDestRect.h=100;

          if(!terrain[compteur][compteur2].x1 || !terrain[compteur][compteur2].x2 || !terrain[compteur][compteur2].y1 || !terrain[compteur][compteur2].y2){

            terrain[compteur][compteur2].xImg=indice4;
            terrain[compteur][compteur2].yImg=indice3;
              //point en haut du losange puis les trois points suivant sont les sommet dans le sens des aiguille d'une montre.



            terrain[compteur][compteur2].x1=imgDestRect.x+imgDestRect.w/2;// -15;
            terrain[compteur][compteur2].y1=imgDestRect.y;// +35;

            terrain[compteur][compteur2].x2=imgDestRect.x+imgDestRect.w;// -15;
            terrain[compteur][compteur2].y2=imgDestRect.y+imgDestRect.h/2;// +35;

            terrain[compteur][compteur2].x3=imgDestRect.x+imgDestRect.w/2;// -15;
            terrain[compteur][compteur2].y3=imgDestRect.y+imgDestRect.h;// +35;

            terrain[compteur][compteur2].x4=imgDestRect.x;// -15;
            terrain[compteur][compteur2].y4=imgDestRect.y+imgDestRect.h/2;// +35;

          }
        }
      }


  //=========================================================INITIALISATION DES JOUEURS=============================================//

      for(int i=0;i<J;i++){
        if(i<J_HUMAIN){
            tab[i].humain=1;
        }else{
          tab[i].humain=0;
        }
        tab[i].id_joueur=i;
        tab[i].pts_action_max= NB_UNITE*PTS_ACTION_UNI;
        tab[i].pts_action_actu=tab[i].pts_action_max;
        tab[i].nb_unite=NB_UNITE;
        tab[i].nb_block=NB_BLOCK;
        tab[i].nb_gold=NB_GOLD;
	      tab[i].nb_bois=NB_BOIS;
      }


  //=========================================================INITIALISATION DES UNITEES===========================================//

  //alea
  /*
      int unit_gen;
      for(int i=0;i<J;i++){
        unit_gen=0;
        while(unit_gen<tab[i].nb_unite){
          for(int compteur=0;compteur<N; compteur++){
            for(int compteur2=0;compteur2<M; compteur2++){
              if(terrain[compteur][compteur2].piece==NULL && terrain[compteur][compteur2].type != 5){
                if(rand()%(N*M) ==1 && unit_gen<tab[i].nb_unite){
                  terrain[compteur][compteur2].piece=init_piece(((rand()%NB_CLASSE)+1),i); //(((rand()%NB_CLASSE)+1),i)
                  unit_gen++;
                }
              }
            }
          }
        }
      }
*/
  //intelligente

    if(PRESET_U==1){
      int nb_place=0;
      int porte=0;
      int unit_gen;
      for(int i=0;i<J;i++){
        do{
          rem_piece_joueur(terrain,i);
          nb_place=tab[i].nb_unite;
          unit_gen=0;
          while(unit_gen==0){
            for(int compteur=0;compteur<N; compteur++){
              for(int compteur2=0;compteur2<M; compteur2++){
                if(zone_libre(terrain,compteur,compteur2,nb_place)){
                  if(rand()%(N*M) ==1){
                    if(nb_place==1)porte=0;
                    else if(nb_place>=2 && nb_place<=9)porte=1;
                    else if(nb_place>=10 && nb_place<=25)porte=2;
                    else if(nb_place>=26 && nb_place<=49)porte=3;
                    for(int a=compteur-porte;a<=compteur+porte;a++){
                      for(int b=compteur2-porte;b<=compteur2+porte;b++){
                        if(a<N && a>=0 && b<M && b>=0 && case_libre(terrain,a,b) && unit_gen<nb_place){
                          terrain[a][b].piece=init_piece(((rand()%NB_CLASSE)+1),i);
                          unit_gen++;
                        }
                      }
                    }

                  }
                }
              }
            }
          }
        }while(nombre_classe(terrain,i,priest)>NB_MAX_PRIEST);
      }
    }else{
      int var_classe,var_joueur;
      fp = fopen ("fichiers/preset1_U.txt", "r"); //preset d'unité en 7x7
      for(int a=0;a<N;a++){
        for(int b=0;b<M;b++){
          fscanf(fp, "%i", &var_classe);
          fscanf(fp, "%i", &var_joueur);
          if(var_classe!=0 && var_joueur!= -1){
            terrain[a][b].piece=init_piece(var_classe,var_joueur);
          }
        }
      }
      fclose(fp);


    }



}






void affichage_principale(SDL_Renderer *renderer,SDL_Window* pWindow,int bordure,int * largeur,int * hauteur,joueurs_t tab[J],degatx_t aff_deg[AFF_DEG],bash_t tab_info_bash[TAILLE_TAB_BASH],case_t terrain[N][M],int joueur_actu,image_t image[Z],int compteur_anim,int nb_joueur_restant,int compteur_tour,int frame_anim_montre,int jour_nuit,char variable2[80],int compteur_bouton_cam){
  //=================================================================================================================================//
						//=================================================================================================================================//
						//====================================================AFFICHAGE====================================================================//
						//=================================================================================================================================//
						//=================================================================================================================================//
	SDL_Color c = {0,0,0,0};
	SDL_Color c_rouge = {255,0,0,0};
	SDL_Color c_verte = {125,255,125,0};
	SDL_Color c_verte2 = {15,109,15,0};
	SDL_Color b = {255,255,255,255};
	SDL_Color c_bash = {255,0,255,0};
	SDL_Color c_bash_degat = {150,15,15,0};
	SDL_Color c_bash_soin = {15,150,15,0};
	SDL_Color c_bash_ineficace = {175,175,175,0};
	SDL_Color c_bash_deplacement = {255,255,255,0};
	SDL_Color c_bash_kill = {255,0,0,0};

  	int test_bash=0,varBash;
    float coefZoom=1;
    SDL_Rect img_anim;
	  char variable[80];


									SDL_RenderClear(renderer);

									SDL_GetWindowSize(pWindow,largeur,hauteur);
									afficher_img(0,0,*hauteur,*largeur,"images/fond.png",image,renderer,1,0,img_anim);

									for (int compteur=0;compteur<N;compteur++){
										for (int compteur2=0;compteur2<M;compteur2++){

											if(terrain[compteur][compteur2].type==1){
												afficher_img(terrain[compteur][compteur2].xImg,terrain[compteur][compteur2].yImg,100,100,"images/case_normalv2.png",image,renderer,coefZoom,0,img_anim);
											}
											else if(terrain[compteur][compteur2].type==2){
												afficher_img(terrain[compteur][compteur2].xImg,terrain[compteur][compteur2].yImg,100,100,"images/case_normal_coinv2.png",image,renderer,coefZoom,0,img_anim);
											}
											else if(terrain[compteur][compteur2].type==3){
												afficher_img(terrain[compteur][compteur2].xImg,terrain[compteur][compteur2].yImg,100,100,"images/case_normal_bordure_gauchev2.png",image,renderer,coefZoom,0,img_anim);
											}
											else if(terrain[compteur][compteur2].type==4){
												afficher_img(terrain[compteur][compteur2].xImg,terrain[compteur][compteur2].yImg,100,100,"images/case_normal_bordure_droitev2.png",image,renderer,coefZoom,0,img_anim);
											}
											else if(terrain[compteur][compteur2].type==5){
												afficher_img(terrain[compteur][compteur2].xImg,terrain[compteur][compteur2].yImg,100,100,"images/case_eauv2.png",image,renderer,coefZoom,0,img_anim);
											}
											else if(terrain[compteur][compteur2].type==6){
												afficher_img(terrain[compteur][compteur2].xImg,terrain[compteur][compteur2].yImg,100,100,"images/case_foretv2.png",image,renderer,coefZoom,0,img_anim);
											}
											else if(terrain[compteur][compteur2].type==7){
												afficher_img(terrain[compteur][compteur2].xImg,terrain[compteur][compteur2].yImg,100,100,"images/case_montagnev2.png",image,renderer,coefZoom,0,img_anim);
											}
										}
									}

									for (int compteur=0;compteur<N;compteur++){
										for (int compteur2=0;compteur2<M;compteur2++){
											if(terrain[compteur][compteur2].piece){
												if(terrain[compteur][compteur2].piece->select){		//affichage case possible déplacement ou attaque si allié, sinon affiche le stats de la cible et sa direction d'arret;
													reset_block(terrain);
                          if(terrain[compteur][compteur2].piece->joueur==joueur_actu){
																			//deplacement
														pathfinding(terrain,compteur,compteur2);

														for (int ind=0;ind<N;ind++){
															for (int ind2=0;ind2<M;ind2++){
																if(terrain[ind][ind2].deplacement==1 && (compteur_anim%(VITESSE_ANIM*2))<(VITESSE_ANIM/2)*2){

																	afficher_img(terrain[ind][ind2].xImg-bordure/15+8,terrain[ind][ind2].yImg-bordure/8+3,100,100,"images/deplacement.png",image,renderer,coefZoom,0,img_anim);

																}else if(terrain[ind][ind2].deplacement==1 && (compteur_anim%(VITESSE_ANIM*2))>=(VITESSE_ANIM/2)*2){

																	afficher_img(terrain[ind][ind2].xImg-bordure/15+8,terrain[ind][ind2].yImg-bordure/8+3,100,100,"images/deplacement2.png",image,renderer,coefZoom,0,img_anim);

																}
															}
														}
															//attaque
														pathfinding_combat(terrain,compteur,compteur2,joueur_actu);

														for (int ind=0;ind<N;ind++){
															for (int ind2=0;ind2<M;ind2++){
																if(terrain[ind][ind2].attaque==1 && ((compteur_anim%VITESSE_ANIM)<(VITESSE_ANIM/2) || (compteur_anim%VITESSE_ANIM)>((VITESSE_ANIM/4)*3)) ){

																	afficher_img(terrain[ind][ind2].xImg-bordure/15+8,terrain[ind][ind2].yImg-bordure/8+3,100,100,"images/attaque.png",image,renderer,coefZoom,0,img_anim);

																}else if(terrain[ind][ind2].attaque==1 && ((compteur_anim%VITESSE_ANIM)>(VITESSE_ANIM/2) || (compteur_anim%VITESSE_ANIM)<=((VITESSE_ANIM/4)*3)) ){

																	afficher_img(terrain[ind][ind2].xImg-bordure/15+8,terrain[ind][ind2].yImg-bordure/8+3,100,100,"images/attaque2.png",image,renderer,coefZoom,0,img_anim);

																}
															}
														}
													}
												}
											}
										}
									}


                  for (int ind=0;ind<N;ind++){
                    for (int ind2=0;ind2<M;ind2++){
                      if(terrain[ind][ind2].block==1 && (compteur_anim%(VITESSE_ANIM*2))<(VITESSE_ANIM/2)*2){

                        afficher_img(terrain[ind][ind2].xImg-bordure/15+8,terrain[ind][ind2].yImg-bordure/8+3,75,150,"images/block.png",image,renderer,coefZoom,0,img_anim);

                      }else if(terrain[ind][ind2].block==1 && (compteur_anim%(VITESSE_ANIM*2))>=(VITESSE_ANIM/2)*2){

                        afficher_img(terrain[ind][ind2].xImg-bordure/15+8,terrain[ind][ind2].yImg-bordure/8+3,75,150,"images/block2.png",image,renderer,coefZoom,0,img_anim);

                      }
                    }
                  }

									for (int compteur=0;compteur<N;compteur++){
										for (int compteur2=0;compteur2<M;compteur2++){
											if(terrain[compteur][compteur2].piece){ 		//AFFICHAGE DES CERCLE UNITES ALLIEES ET UNITES SELECTIONNEES

												if(terrain[compteur][compteur2].piece->joueur==joueur_actu && terrain[compteur][compteur2].piece->select==0){ //cercle des unité du joueur_actu affichage
													afficher_img(terrain[compteur][compteur2].xImg,terrain[compteur][compteur2].yImg+5,100,100,"images/image_allie.png",image,renderer,coefZoom,0,img_anim);
												}else if(terrain[compteur][compteur2].piece->joueur==joueur_actu && terrain[compteur][compteur2].piece->select==1){ //cercle de l'unité selectionné du joueur_actu
													afficher_img(terrain[compteur][compteur2].xImg,terrain[compteur][compteur2].yImg+5,100,100,"images/image_select_allie.png",image,renderer,coefZoom,0,img_anim);
												}
											}
										}
									}


									for (int compteur=0;compteur<N;compteur++){
										for (int compteur2=0;compteur2<M;compteur2++){
											if(terrain[compteur][compteur2].piece){ 		//AFFICHAGE DES UNITEES
												switch (terrain[compteur][compteur2].piece->classe){
													case 1:

													img_anim=afficher_anim(compteur_anim,terrain[compteur][compteur2].piece->classe,terrain,compteur,compteur2,0);
													afficher_img(terrain[compteur][compteur2].xImg,terrain[compteur][compteur2].yImg-10,100,100,"images/knight_anim.png",image,renderer,coefZoom,1,img_anim);

													break;
													case 2:

													img_anim=afficher_anim(compteur_anim,terrain[compteur][compteur2].piece->classe,terrain,compteur,compteur2,0);
													afficher_img(terrain[compteur][compteur2].xImg,terrain[compteur][compteur2].yImg-10,100,100,"images/scout_anim.png",image,renderer,coefZoom,1,img_anim);

													break;
													case 3:

													img_anim=afficher_anim(compteur_anim,terrain[compteur][compteur2].piece->classe,terrain,compteur,compteur2,0);
													afficher_img(terrain[compteur][compteur2].xImg,terrain[compteur][compteur2].yImg-10,100,100,"images/priest_anim.png",image,renderer,coefZoom,1,img_anim);

													break;
													case 4:

													img_anim=afficher_anim(compteur_anim,terrain[compteur][compteur2].piece->classe,terrain,compteur,compteur2,0);
													afficher_img(terrain[compteur][compteur2].xImg,terrain[compteur][compteur2].yImg-10,100,100,"images/magician_anim.png",image,renderer,coefZoom,1,img_anim);

													break;
													case 5:

													afficher_img(terrain[compteur][compteur2].xImg,terrain[compteur][compteur2].yImg-10,100,100,"images/faucheuse.png",image,renderer,coefZoom,0,img_anim);

													break;
													case 6:

													img_anim=afficher_anim(compteur_anim,terrain[compteur][compteur2].piece->classe,terrain,compteur,compteur2,0);
													afficher_img(terrain[compteur][compteur2].xImg,terrain[compteur][compteur2].yImg-10,100,100,"images/ange_anim.png",image,renderer,coefZoom,1,img_anim);

													break;
												}
											}
										}
									}

									//affichage des fleche de deplacement transparentes sur les ennemis a portee de l'allie selectionné si il y en a un
									for (int compteur=0;compteur<N;compteur++){
										for (int compteur2=0;compteur2<M;compteur2++){
											if(terrain[compteur][compteur2].piece && terrain[compteur][compteur2].piece->select == 1 && terrain[compteur][compteur2].piece->joueur==joueur_actu && terrain[compteur][compteur2].piece->classe!=priest){
												for (int i=0;i<N;i++){
													for (int j=0;j<M;j++){
														if(terrain[i][j].attaque==1){
															switch(terrain[i][j].piece->direction){
																case 1:
																	afficher_img(terrain[i][j].xImg+10,terrain[i][j].yImg+10,80,80,"images/fleche_nord_trans.png",image,renderer,1,0,img_anim);
																break;
																case 2:
																	afficher_img(terrain[i][j].xImg+10,terrain[i][j].yImg+10,80,80,"images/fleche_sud_trans.png",image,renderer,1,0,img_anim);
																break;
																case 3:
																	afficher_img(terrain[i][j].xImg+10,terrain[i][j].yImg+10,80,80,"images/fleche_ouest_trans.png",image,renderer,1,0,img_anim);
																break;
																case 4:
																	afficher_img(terrain[i][j].xImg+10,terrain[i][j].yImg+10,80,80,"images/fleche_est_trans.png",image,renderer,1,0,img_anim);
																break;
															}
														}
													}
												}
											}
										}
									}


                  //nuit et jour//
                  if((jour_nuit>=((DURE_JOUR_NUIT/24)*22) && jour_nuit<=(DURE_JOUR_NUIT)+50) || (jour_nuit>=0 && jour_nuit<=((DURE_JOUR_NUIT/24)*2) )){
                    afficher_img(0,0,*hauteur,*largeur,"images/nuit-3.png",image,renderer,1,0,img_anim);
                  }else if((jour_nuit>=((DURE_JOUR_NUIT/24)*21) && jour_nuit<((DURE_JOUR_NUIT/24)*22) )|| (jour_nuit>((DURE_JOUR_NUIT/24)*2) && jour_nuit<=((DURE_JOUR_NUIT/24)*3) )){
                    afficher_img(0,0,*hauteur,*largeur,"images/nuit-2.png",image,renderer,1,0,img_anim);
                  }else if((jour_nuit>=((DURE_JOUR_NUIT/24)*20) && jour_nuit<((DURE_JOUR_NUIT/24)*21) )|| (jour_nuit>((DURE_JOUR_NUIT/24)*3) && jour_nuit<=((DURE_JOUR_NUIT/24)*4) )){
                    afficher_img(0,0,*hauteur,*largeur,"images/nuit-1.png",image,renderer,1,0,img_anim);
                  }

                  //===========//


									//drap rouge en haut
									afficher_img(0,0,*hauteur,*largeur,"images/drap_rouge.png",image,renderer,1,0,img_anim);

									//fin du tour bouton

									afficher_img((*largeur)/3+420,15,50,150,"images/fin_du_tour.png",image,renderer,1,0,img_anim);

									//menu nord

									afficher_img((*largeur)/3,0,75,400,"images/menu_nord.png",image,renderer,1,0,img_anim);

									AfficherText("ACTION RESTANTE:","arial.ttf",c,12,renderer,(*largeur)/3+200,30);

									AfficherText("TOUR DU JOUEUR ","arial.ttf",c,12,renderer,(*largeur)/3+50,30);

									sprintf(variable, "%d",  tab[joueur_actu].pts_action_actu);

									AfficherText(variable,"arial.ttf",c,30,renderer,(*largeur)/3+325,18);

									sprintf(variable, "%d",  joueur_actu);

									AfficherText(variable,"arial.ttf",c,30,renderer,(*largeur)/3+165,18);
									//stats


									afficher_img(0,0,400,150,"images/inter.png",image,renderer,1,0,img_anim);

									AfficherText("PDV        :","arial.ttf",c,12,renderer,30,155);

									AfficherText("ATK        :","arial.ttf",c,12,renderer,30,175);

									AfficherText("DEF        :","arial.ttf",c,12,renderer,30,195);

									AfficherText("BLOCK  :","arial.ttf",c,12,renderer,30,215);

									AfficherText("RANGE  :","arial.ttf",c,12,renderer,30,235);

									AfficherText("MS          :","arial.ttf",c,12,renderer,30,255);

									AfficherText("MENU", "arial.ttf",b,15,renderer,gpScreen->w - 60,gpScreen->h - 30);



									//infos générales de la partie
									afficher_img(150,0,200,300,"images/menu_nord.png",image,renderer,1,0,img_anim);

									AfficherText("Joueurs Restants :        ","arial.ttf",c,20,renderer,185,30);
									sprintf(variable, "%d",  nb_joueur_restant);
									AfficherText(variable,"arial.ttf",c,30,renderer,365,26);

									AfficherText("Unit Allies :        ","arial.ttf",c,20,renderer,185,80);
									sprintf(variable, "%d",  reste_allie(terrain,joueur_actu));
									AfficherText(variable,"arial.ttf",c,30,renderer,365,72);

									AfficherText("Unit Ennemis:        ","arial.ttf",c,20,renderer,185,130);
									sprintf(variable, "%d",  reste_ennemi(terrain,joueur_actu));
									AfficherText(variable,"arial.ttf",c,30,renderer,365,122);


						//==============BASH=======================//

									afficher_img(((*largeur)-450),0,300,450,"images/bash.png",image,renderer,1,0,img_anim);
									test_bash=0;
									for(int i=0;i<J;i++){
										if(tab[i].id_joueur==-2){
											varBash=i;
											test_bash=1;
										}
									}
									if(test_bash==1){
										sprintf(variable, "| Le Joueur %d a ete elimine.",varBash);
										ajouter_ligne_bash(variable,tab_info_bash,info,variable2);
									}

									for(int i=0;i<TAILLE_TAB_BASH;i++){   //AFFICHAGE DU BASH
										if(tab_info_bash[i].pos_y<=265 && tab_info_bash[i].pos_y>=5){
											switch(tab_info_bash[i].couleur){
												case 1: AfficherText(tab_info_bash[i].txt,"arial.ttf",c_bash,15,renderer,((*largeur)-450+25),tab_info_bash[i].pos_y);break;
												case 2: AfficherText(tab_info_bash[i].txt,"arial.ttf",c_bash_degat,15,renderer,((*largeur)-450+25),tab_info_bash[i].pos_y);break;
												case 3: AfficherText(tab_info_bash[i].txt,"arial.ttf",c_bash_soin,15,renderer,((*largeur)-450+25),tab_info_bash[i].pos_y);break;
												case 4: AfficherText(tab_info_bash[i].txt,"arial.ttf",c_bash_ineficace,15,renderer,((*largeur)-450+25),tab_info_bash[i].pos_y);break;
												case 5: AfficherText(tab_info_bash[i].txt,"arial.ttf",c_bash_deplacement,15,renderer,((*largeur)-450+25),tab_info_bash[i].pos_y);break;
												case 6: AfficherText(tab_info_bash[i].txt,"arial.ttf",c_bash_kill,15,renderer,((*largeur)-450+25),tab_info_bash[i].pos_y);break;
											}
										}
									}


				//=========================================//

				//=======COMPTEUR JOUR======//



									img_anim.x=150*frame_anim_montre;
									img_anim.y=0;
									img_anim.w=150;
									img_anim.h=150;
									afficher_img(((*largeur)-160),410,150,150,"images/montre_anim.png",image,renderer,coefZoom,1,img_anim);
									AfficherText("JOUR","arial.ttf",c,20,renderer,((*largeur)-110),445);
									sprintf(variable, "%d",compteur_tour);
									AfficherText(variable,"arial.ttf",c,30,renderer,((*largeur)-102),490);





         //========================//

                  afficher_img(10,550,200,100,"images/block_menu.png",image,renderer,1,0,img_anim);
                  afficher_img(40,630,90,35,"images/blockISO.png",image,renderer,1,0,img_anim);
                  AfficherText("BLOCS :","arial.ttf",c,15,renderer,28,588);
                  sprintf(variable, "%d",tab[joueur_actu].nb_block);
									AfficherText(variable,"arial.ttf",c,30,renderer,38,625);

                  if(compteur_bouton_cam<=0){
                    afficher_img(22,420,100,100,"images/bouton_cam.png",image,renderer,1,0,img_anim);
                  }else{
                    afficher_img(22,420,100,100,"images/bouton_cam2.png",image,renderer,1,0,img_anim);
                  }


									for(int i=0;i<N;i++){
										for(int j=0;j<M;j++){
											if(terrain[i][j].piece && terrain[i][j].piece->select==1){
												//affichage des infos de l'unité en haut a gauche.

												switch (terrain[i][j].piece->classe){
													case 1:

													img_anim=afficher_anim(compteur_anim,terrain[i][j].piece->classe,terrain,i,j,1);
													afficher_img(20,20,100,100,"images/knight_anim.png",image,renderer,coefZoom,1,img_anim);

												break;
												case 2:

													img_anim=afficher_anim(compteur_anim,terrain[i][j].piece->classe,terrain,i,j,1);
													afficher_img(20,20,100,100,"images/scout_anim.png",image,renderer,coefZoom,1,img_anim);

												break;
												case 3:

													img_anim=afficher_anim(compteur_anim,terrain[i][j].piece->classe,terrain,i,j,1);
													afficher_img(20,20,100,100,"images/priest_anim.png",image,renderer,coefZoom,1,img_anim);

												break;
												case 4:

												img_anim=afficher_anim(compteur_anim,terrain[i][j].piece->classe,terrain,i,j,1);
												afficher_img(20,20,100,100,"images/magician_anim.png",image,renderer,coefZoom,1,img_anim);

												break;
												case 5:

												afficher_img(20,20,100,100,"images/faucheuse.png",image,renderer,1,0,img_anim);
												break;
												case 6:

													img_anim=afficher_anim(compteur_anim,terrain[i][j].piece->classe,terrain,i,j,1);
													afficher_img(20,20,100,100,"images/ange_anim.png",image,renderer,coefZoom,1,img_anim);

												break;
												}
												//text


															//affichage des stats de l'unité selectionné en fonction du terrain
												sprintf(variable, "%d",  terrain[i][j].piece->joueur);
												AfficherText(variable,"arial.ttf",c,25,renderer,65,110);

												sprintf(variable, "%d", terrain[i][j].piece->pdv);
												AfficherText(variable,"arial.ttf",c,12,renderer,90,155);

												sprintf(variable, "%d", terrain[i][j].piece->puissance);
												AfficherText(variable,"arial.ttf",c,12,renderer,90,175);

												if(terrain[i][j].type==6){
													sprintf(variable, "%d", (terrain[i][j].piece->armure+9));
													AfficherText(variable,"arial.ttf",c_verte2,12,renderer,90,195);
												}else{
													sprintf(variable, "%d", terrain[i][j].piece->armure);
													AfficherText(variable,"arial.ttf",c,12,renderer,90,195);
												}


												if(terrain[i][j].type==6){
													sprintf(variable, "%d", (terrain[i][j].piece->block+10));
													AfficherText(variable,"arial.ttf",c_verte2,12,renderer,90,215);
												}else{
													sprintf(variable, "%d", terrain[i][j].piece->block);
													AfficherText(variable,"arial.ttf",c,12,renderer,90,215);
												}

												if(terrain[i][j].type==7){
													sprintf(variable, "%d", (terrain[i][j].piece->portee+1));
													AfficherText(variable,"arial.ttf",c_verte2,12,renderer,90,235);
												}else{
													sprintf(variable, "%d", terrain[i][j].piece->portee);
													AfficherText(variable,"arial.ttf",c,12,renderer,90,235);
												}

												if(terrain[i][j].type==6 || terrain[i][j].type==7){
													sprintf(variable, "%d", (terrain[i][j].piece->deplacement-1));
													AfficherText(variable,"arial.ttf",c_rouge,12,renderer,90,255);
												}else{
													sprintf(variable, "%d", terrain[i][j].piece->deplacement);
													AfficherText(variable,"arial.ttf",c,12,renderer,90,255);
												}



												//affichage de nombre de kills actuel de l'unité si ce n'est pas un pretre
												if(terrain[i][j].piece->classe!=priest){
													afficher_img(106,48,30,30,"images/kills.png",image,renderer,1,0,img_anim);
													sprintf(variable, "%d", terrain[i][j].piece->kill);
													AfficherText(variable,"arial.ttf",c_rouge,24,renderer,114,75);
												}
												//DIRECTION affichage


												switch(terrain[i][j].piece->direction){
													case 1:
														afficher_img(25,275,100,100,"images/fleche_nord.png",image,renderer,1,0,img_anim);
													break;
													case 2:
														afficher_img(25,275,100,100,"images/fleche_sud.png",image,renderer,1,0,img_anim);
													break;
													case 3:
														afficher_img(25,275,100,100,"images/fleche_ouest.png",image,renderer,1,0,img_anim);
													break;
													case 4:
														afficher_img(25,275,100,100,"images/fleche_est.png",image,renderer,1,0,img_anim);
													break;
												}


											}else if(terrain[i][j].bloc->block_sel==1){
                        afficher_img(20,20,100,100,"images/blockISO.png",image,renderer,1,0,img_anim);

                        sprintf(variable, "%d",  terrain[i][j].bloc->block_allie);
												AfficherText(variable,"arial.ttf",c,25,renderer,65,110);

												sprintf(variable, "%d", terrain[i][j].bloc->pdv_block);
												AfficherText(variable,"arial.ttf",c,12,renderer,90,155);

												sprintf(variable, "%d", 50);
												AfficherText(variable,"arial.ttf",c,12,renderer,90,195);
                      }
										}
									}

									for(int i=0;i<AFF_DEG;i++){ //affichage des dégats, morts soins etc
										if(aff_deg[i].time>0){
											if(aff_deg[i].c==1){//couleur degat
												AfficherText(aff_deg[i].txt,"arial.ttf",c_rouge,30,renderer,aff_deg[i].pos_x,aff_deg[i].pos_y);
											}else if(aff_deg[i].c==2){//couleur soin
												AfficherText(aff_deg[i].txt,"arial.ttf",c_verte,30,renderer,aff_deg[i].pos_x,aff_deg[i].pos_y);
                      }else if(aff_deg[i].c==3){//couleur kill
												AfficherText(aff_deg[i].txt,"arial.ttf",c_bash,30,renderer,aff_deg[i].pos_x,aff_deg[i].pos_y);
											}else{//couleur grise
												AfficherText(aff_deg[i].txt,"arial.ttf",c_bash_deplacement,30,renderer,aff_deg[i].pos_x,aff_deg[i].pos_y);
											}
										}
									}




						//=================================================================================================================================//
						//=======================================================FIN=======================================================================//
						//====================================================AFFICHAGE====================================================================//
						//=======================================================...=======================================================================//
						//=================================================================================================================================//


}


int zone_libre(case_t terrain[N][M],int x, int y,int nb_place){
  int porte=0,place_dispo=0;
  if(nb_place==1)porte=0;
  if(nb_place>=2 && nb_place<=9)porte=1;
  if(nb_place>=10 && nb_place<=25)porte=2;
  if(nb_place>=26 && nb_place<=49)porte=3;

  int porte_gen=porte+ESPACE_GEN;


  if(case_libre(terrain,x,y)){
    for(int a=x-porte_gen;a<=x+porte_gen;a++){
      for(int b=y-porte_gen;b<=y+porte_gen;b++){
        if(a<N && a>=0 && b<M && b>=0 && terrain[a][b].piece!=NULL){
          return 0;
        }
      }
    }
  }

  if(case_libre(terrain,x,y)){
    for(int a=x-porte;a<=x+porte;a++){
      for(int b=y-porte;b<=y+porte;b++){
        if(a<N && a>=0 && b<M && b>=0 && case_libre(terrain,a,b)){
          place_dispo++;
        }
      }
    }
  }
  if(place_dispo>=nb_place){
    return 1;
  }
  return 0;
}


int case_libre(case_t terrain[N][M],int x,int y){
  if(terrain[x][y].type!=5 && terrain[x][y].piece==NULL){
    return 1;
  }
  return 0;
}


SDL_Rect afficher_anim(int compteur_anim,classe_t classe,case_t terrain[N][M],int compteur,int compteur2,int inter){
  SDL_Rect img_anim;
  img_anim.h=100;
  img_anim.w=100;
    //anim a 2 images
  if(inter==0){
    if(classe==knight || classe==scout || classe==magician || classe==priest){
      if(compteur_anim%VITESSE_ANIM==terrain[compteur][compteur2].piece->start_anim){
        if(terrain[compteur][compteur2].piece->frame==2){
          terrain[compteur][compteur2].piece->frame=1;
        }else{
          terrain[compteur][compteur2].piece->frame++;
        }
      }
      if(terrain[compteur][compteur2].piece->frame==1){
          img_anim.x=0;
          img_anim.y=0;
      }else if(terrain[compteur][compteur2].piece->frame==2){
          img_anim.x=100;
          img_anim.y=0;
      }
    }
    //ANIM DE L'ANGE
    else if(classe==ange){
      if(compteur_anim%VITESSE_ANIM==terrain[compteur][compteur2].piece->start_anim){
        if(terrain[compteur][compteur2].piece->frame==10){
          terrain[compteur][compteur2].piece->frame=1;
        }else{
          terrain[compteur][compteur2].piece->frame++;
        }
      }
      if(terrain[compteur][compteur2].piece->frame==1 || terrain[compteur][compteur2].piece->frame>9){
          img_anim.x=0;
          img_anim.y=0;
      }else if(terrain[compteur][compteur2].piece->frame%2==0){
          img_anim.x=100;
          img_anim.y=0;
      }else{
          img_anim.x=0;
          img_anim.y=100;
      }
    }
  }else{ //anim de l'interface en haut a gauche
    if(classe==knight || classe==scout || classe==magician || classe==priest){
      if(compteur_anim%VITESSE_ANIM==terrain[compteur][compteur2].piece->start_anim){
        if(terrain[compteur][compteur2].piece->frame_interface==2){
          terrain[compteur][compteur2].piece->frame_interface=1;
        }else{
          terrain[compteur][compteur2].piece->frame_interface++;
        }
      }
      if(terrain[compteur][compteur2].piece->frame_interface==1){
          img_anim.x=0;
          img_anim.y=0;
      }else if(terrain[compteur][compteur2].piece->frame_interface==2){
          img_anim.x=100;
          img_anim.y=0;
      }
    }
    else if(classe==ange){
      if(compteur_anim%VITESSE_ANIM==terrain[compteur][compteur2].piece->start_anim){
        if(terrain[compteur][compteur2].piece->frame_interface==10){
          terrain[compteur][compteur2].piece->frame_interface=1;
        }else{
          terrain[compteur][compteur2].piece->frame_interface++;
        }
      }
      if(terrain[compteur][compteur2].piece->frame_interface==1 || terrain[compteur][compteur2].piece->frame_interface>9){
          img_anim.x=0;
          img_anim.y=0;
      }else if(terrain[compteur][compteur2].piece->frame_interface%2==0){
          img_anim.x=100;
          img_anim.y=0;
      }else{
          img_anim.x=0;
          img_anim.y=100;
      }
    }

  }



  return img_anim;
}
