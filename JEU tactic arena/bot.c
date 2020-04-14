#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bot.h"
#include "combat.h"
#include "deplacement.h"






int action_possible(case_t terrain[N][M],int joueur_actu){
  for(int i=0;i<N;i++){
    for(int j=0;j<M;j++){
      if(terrain[i][j].piece && terrain[i][j].piece->joueur==joueur_actu && terrain[i][j].piece->pts_action_actu>0 &&( (allie_adjacent(terrain,i,j,joueur_actu)==0 && reste_allie(terrain,joueur_actu)>1) || a_portee(terrain,i,j,joueur_actu)==0  )){
        return 1;
      }
      if(terrain[i][j].piece && ((a_portee(terrain,i,j,joueur_actu)>=1 && terrain[i][j].piece->classe!=priest)|| (a_portee(terrain,i,j,joueur_actu)>=2 && terrain[i][j].piece->classe==priest) || (reste_allie(terrain,joueur_actu)==1 && terrain[i][j].piece->classe==priest))){
        return 1;
      }
    }
  }
  return 0;
}

void depla_atk_mov(case_t terrain[N][M],int x_bot,int y_bot,int joueur_actu,joueurs_t tab[J],bash_t tab_info_bash[TAILLE_TAB_BASH],char variable2[80]){
  //V2
  //addition de la difference des pos x et y de l'unité par rapport a ses allié,
  //prendre la piece la plus proche et se rapprocher au max des ses coordonné x et y tt en restant dans la range de déplacement
  //int var4=rand()%2; //choix vers ennemi ou allie

  if(terrain[x_bot][y_bot].piece && a_portee(terrain,x_bot,y_bot,joueur_actu)==0 ){
    fprintf(stderr,"VERS ennemi\n");
    depla_ennem_plus_proche(terrain,x_bot,y_bot,joueur_actu,tab,tab_info_bash,variable2);
  }else if(terrain[x_bot][y_bot].piece && reste_allie(terrain,joueur_actu)>1 && (allie_adjacent(terrain,x_bot,y_bot,joueur_actu))==0){
    fprintf(stderr,"VERS allie\n");
    depla_allie_plus_proche(terrain,x_bot,y_bot,joueur_actu,tab,tab_info_bash,variable2);
  }else{
    move_alea(terrain,x_bot,y_bot,joueur_actu,tab,tab_info_bash,variable2);
  }
}


void attaquer_meilleur_cible(case_t terrain[N][M],int x_bot,int y_bot,int nb_ennemies_portee,int joueur_actu,joueurs_t tab[J],degatx_t aff_deg[AFF_DEG],bash_t tab_info_bash[TAILLE_TAB_BASH],char variable2[80]){
  pathfinding_combat(terrain,x_bot,y_bot,joueur_actu);
  if(terrain[x_bot][y_bot].piece->classe==priest){//on heal l'allié avec le moin de pv si on est soigneur ou celui qui a le plus d'ennemi adjacent si tt les points de vie sont supérieur 25
    int x_low,y_low,pv_low=-1;
    int tab_ennemi_ont_portee[N][M];
    for(int i=0;i<N;i++){
      for(int j=0;j<M;j++){
        tab_ennemi_ont_portee[i][j]=0;
      }
    }
    for(int i=0;i<N;i++){ //minimum de pv des alliés
      for(int j=0;j<M;j++){
        if(terrain[i][j].attaque==1 && pv_low==-1){
          x_low=i;
          y_low=j;
          pv_low=terrain[i][j].piece->pdv;
        }else if(terrain[i][j].attaque==1 && pv_low>terrain[i][j].piece->pdv){
          x_low=i;
          y_low=j;
          pv_low=terrain[i][j].piece->pdv;
        }
      }
    }
    if(pv_low<=25){//soin si pv <= a 25

      combat(terrain,x_bot,y_bot,x_low,y_low,joueur_actu,tab,aff_deg,tab_info_bash,variable2);

    }else{ //sinon on heal l'allié avec le plus d'ennemi adjacent


//version OPTIMISE
      int max_portee_ennemi=1;
      for(int i=0;i<N;i++){
        for(int j=0;j<M;j++){
          if(terrain[i][j].piece && terrain[i][j].piece->joueur!=joueur_actu && terrain[i][j].piece->portee>max_portee_ennemi){
            max_portee_ennemi=terrain[i][j].piece->portee+1; //+ 1 au cas ou la piece serais sur un montagne
          }
        }
      }
      for(int a=x_bot-max_portee_ennemi-terrain[x_bot][y_bot].piece->portee-1;a<x_bot-max_portee_ennemi+terrain[x_bot][y_bot].piece->portee+1 && a<N;a++){
        for(int b=y_bot-max_portee_ennemi-terrain[x_bot][y_bot].piece->portee-1;b<y_bot-max_portee_ennemi+terrain[x_bot][y_bot].piece->portee+1 && b<M;b++){
          if(a>=0 && b>=0 && terrain[a][b].piece && terrain[a][b].piece->joueur!=joueur_actu){
            pathfinding_combat(terrain,a,b,terrain[a][b].piece->joueur);
            for(int compteur=a-terrain[a][b].piece->portee-1; compteur<a+terrain[a][b].piece->portee+1 && compteur<N;compteur++){
              for(int compteur2=b-terrain[a][b].piece->portee-1; compteur2<b+terrain[a][b].piece->portee+1 && compteur2<M;compteur2++){
                if(compteur>=0 && compteur2>=0 && terrain[compteur][compteur2].attaque==1){
                  tab_ennemi_ont_portee[compteur][compteur2]+=1;
                }
              }
            }
          }
        }
      }
      int max_atk=0,x=x_bot,y=y_bot;
      pathfinding_combat(terrain,x_bot,y_bot,joueur_actu);
      for(int i=0;i<N;i++){
        for(int j=0;j<M;j++){
          if(terrain[i][j].attaque==1 && tab_ennemi_ont_portee[i][j]>max_atk){
            max_atk=tab_ennemi_ont_portee[i][j];
            x=i;
            y=j;
          }
        }
      }
      combat(terrain,x_bot,y_bot,x,y,joueur_actu,tab,aff_deg,tab_info_bash,variable2);
    }

//FIN VERSION OPTIMISE

      //version NON OPTIMISE
      /*for(int i=0;i<N;i++){
        for(int j=0;j<M;j++){
          if(terrain[i][j].piece && terrain[i][j].piece->joueur!=joueur_actu){
            pathfinding_combat(terrain,i,j,terrain[i][j].piece->joueur);  //xbot ou i j ?
            for(int compteur=0;compteur<N;compteur++){  //a optimiser pour grille plus grande
              for(int compteur2=0;compteur2<M;compteur2++){
                if(terrain[compteur][compteur2].attaque==1){
                  tab_ennemi_ont_portee[compteur][compteur2]+=1;
                }
              }
            }
          }
        }
      }
      int max_atk=0,x=x_bot,y=y_bot;
      pathfinding_combat(terrain,x_bot,y_bot,joueur_actu);
      for(int i=0;i<N;i++){
        for(int j=0;j<M;j++){
          if(terrain[i][j].attaque==1 && tab_ennemi_ont_portee[i][j]>max_atk){
            max_atk=tab_ennemi_ont_portee[i][j];
            x=i;
            y=j;
          }
        }
      }
      combat(terrain,x_bot,y_bot,x,y,joueur_actu,tab,aff_deg,tab_info_bash,variable2);
    }*/

  }else{ //cas ou ce n'est pas un soigneur
    int x_low,y_low,pv_low=-1;
    for(int i=0;i<N;i++){
      for(int j=0;j<M;j++){
        if(terrain[i][j].attaque==1 && pv_low==-1){
          x_low=i;
          y_low=j;
          if(terrain[i][j].piece){
            pv_low=terrain[i][j].piece->pdv;
          }else{
            pv_low=terrain[i][j].bloc->pdv_block;
          }
        }else if(terrain[i][j].attaque==1 && ((terrain[i][j].piece && pv_low>terrain[i][j].piece->pdv) || (terrain[i][j].piece==NULL && pv_low>terrain[i][j].bloc->pdv_block) )){
          x_low=i;
          y_low=j;
          if(terrain[i][j].piece){
            pv_low=terrain[i][j].piece->pdv;
          }else{
            pv_low=terrain[i][j].bloc->pdv_block;
          }
        }
      }
    }
    combat(terrain,x_bot,y_bot,x_low,y_low,joueur_actu,tab,aff_deg,tab_info_bash,variable2); //attaque l'ennemi avec le moin de pv
  }

}





void depla_allie_plus_proche(case_t terrain[N][M],int x_bot,int y_bot,int joueur_actu,joueurs_t tab[J],bash_t tab_info_bash[TAILLE_TAB_BASH],char variable2[80]){
  //calcule de l'allie le plus proche et sauvegarde de ses position x et y
  int x_allie,y_allie;
  int distance_allie=-1;
  for(int i=0;i<N;i++){
    for(int j=0;j<M;j++){
      if(terrain[i][j].piece && terrain[i][j].piece->joueur==joueur_actu && distance_allie==-1 && (x_bot!=i || y_bot!=j)){
        x_allie=i;
        y_allie=j;
        distance_allie=(abs(x_bot-x_allie)+abs(y_bot-y_allie));
      }else if(terrain[i][j].piece && terrain[i][j].piece->joueur==joueur_actu && distance_allie>(abs(x_bot-i)+abs(y_bot-j)) && (x_bot!=i || y_bot!=j)){
        x_allie=i;
        y_allie=j;
        distance_allie=(abs(x_bot-x_allie)+abs(y_bot-y_allie));
      }
    }
  }

  //deplacement au plus pret
  move_longue_range(terrain,x_bot,y_bot,x_allie,y_allie,joueur_actu,tab,tab_info_bash,variable2);
}




void depla_ennem_plus_proche(case_t terrain[N][M],int x_bot,int y_bot,int joueur_actu,joueurs_t tab[J],bash_t tab_info_bash[TAILLE_TAB_BASH],char variable2[80]){
  //calcule de l'ennemi le plus proche et sauvegarde de ses position x et y
  int x_allie,y_allie;
  int distance_allie=-1;
  for(int i=0;i<N;i++){
    for(int j=0;j<M;j++){
      if((terrain[i][j].piece && terrain[i][j].piece->joueur!=joueur_actu && distance_allie==-1 && (x_bot!=i || y_bot!=j)) || (terrain[i][j].bloc!=NULL && terrain[i][j].bloc->block_allie!=joueur_actu)){
        x_allie=i;
        y_allie=j;
        distance_allie=(abs(x_bot-x_allie)+abs(y_bot-y_allie));
      }else if((terrain[i][j].piece && terrain[i][j].piece->joueur!=joueur_actu && distance_allie>(abs(x_bot-i)+abs(y_bot-j))&& (x_bot!=i || y_bot!=j)) || (terrain[i][j].bloc!=NULL && terrain[i][j].bloc->block_allie!=joueur_actu)){
        x_allie=i;
        y_allie=j;
        distance_allie=(abs(x_bot-x_allie)+abs(y_bot-y_allie));
      }
    }
  }

  //deplacement au plus pret
  move_longue_range(terrain,x_bot,y_bot,x_allie,y_allie,joueur_actu,tab,tab_info_bash,variable2);

}




void move_longue_range(case_t terrain[N][M], int x, int y,int x_dest,int y_dest,int joueur_actu,joueurs_t tab[J],bash_t tab_info_bash[TAILLE_TAB_BASH],char variable2[80]){
  int deplacement[N][M];
  int i,j;
  int cpt;
    //initialisation des deplacements possibles

  for (i=0;i<N;i++){//parcours du terrain
      for(j=0;j<M;j++){
          if ((terrain[i][j].type==5) || terrain[i][j].piece!=NULL || (terrain[i][j].bloc!=NULL && terrain[x][y].piece->joueur!=terrain[i][j].bloc->block_allie)){
              deplacement[i][j]=-1;
          }//case d'eau deplacement impossible
          else{
            deplacement[i][j]=0;
          }
      }


  }
  deplacement[x][y]=1;//case ou se trouve la piece
  deplacement[x_dest][y_dest]=0;//case ou se trouve la piece ou le bloc a rejoindre

  int possible=1;
  for(cpt=1;deplacement[x_dest][y_dest]==0 && possible!=0;cpt++){
      possible=0;
      for (i=0;i<N;i++){//parcours du terrain
          for(j=0;j<M;j++){
              if (deplacement[i][j]==cpt){
                  if(i+1<N && deplacement[i+1][j]==0){
                      deplacement[i+1][j]=cpt+1;
                      possible++;
                  }
                  if(i-1>=0 && deplacement[i-1][j]==0){
                      deplacement[i-1][j]=cpt+1;
                      possible++;
                  }
                  if(j+1<M && deplacement[i][j+1]==0){
                      deplacement[i][j+1]=cpt+1;
                      possible++;
                  }
                  if(j-1>=0 && deplacement[i][j-1]==0){
                      deplacement[i][j-1]=cpt+1;
                      possible++;
                  }

              }

          }
      }
  }

  fprintf(stderr,"avant alea valeur dest = %d\n",deplacement[x_dest][y_dest]);
  int a=x_dest;
  int b=y_dest;
  int var=deplacement[x_dest][y_dest];
  pathfinding(terrain,x,y);
  while(terrain[a][b].deplacement!=1){
    if(a+1<N && deplacement[a+1][b]==var-1){
      a++;
      var--;
    }
    else if(a-1>=0 && deplacement[a-1][b]==var-1){
      a--;
      var--;
    }
    else if(b+1<M && deplacement[a][b+1]==var-1){
      b++;
      var--;
    }
    else if(b-1>=0 && deplacement[a][b-1]==var-1){
      b--;
      var--;
    }else{
      move_alea(terrain,x,y,joueur_actu,tab,tab_info_bash,variable2);
      return;
    }
  }

  move(terrain,a,b,joueur_actu,tab,tab_info_bash,variable2);

}

void move_alea(case_t terrain[N][M],int x,int y, int joueur,joueurs_t tab[J],bash_t tab_info_bash[TAILLE_TAB_BASH],char variable2[80]){
  pathfinding(terrain,x,y);
  int ini=0,x_dest,y_dest;
  for (int i=0;i<N;i++){
    for(int j=0;j<M;j++){
      if(terrain[i][j].deplacement==1 && ini==0){
        x_dest=i;
        y_dest=j;
        ini=1;
      }else if(terrain[i][j].deplacement==1 && rand()%((N+M)/2)){
        x_dest=i;
        y_dest=j;
      }
    }
  }
  if(ini!=0){
    move(terrain,x_dest,y_dest,joueur,tab,tab_info_bash,variable2);
  }else{
    fprintf(stderr,"AUCUNE ACTION POSSIBLE");
    terrain[x][y].piece->pts_action_actu--;
    tab[joueur].pts_action_actu--;
  }
}




void IA_blockage_direction(case_t terrain[N][M],int x_def,int y_def,int joueur_actu){
  int pos_block[N][M];
  int cpt=1;
  //init matrice a 0
  for(int i=0;i<N;i++){
      for(int j=0;j<M;j++){
        pos_block[i][j]=-2;
      }
  }
  pos_block[x_def][y_def]=0;
  //diagonales a -1
  while (x_def-cpt>=0 && y_def+cpt<M){
      pos_block[x_def-cpt][y_def+cpt]=-1;
      cpt++;
  }
  cpt=1;
  while (x_def+cpt<N && y_def+cpt<N){
      pos_block[x_def+cpt][y_def+cpt]=-1;
      cpt++;
  }
  cpt=1;
  while (x_def+cpt<N && y_def-cpt>=0){
      pos_block[x_def+cpt][y_def-cpt]=-1;
      cpt++;
  }
  cpt=1;
  while (x_def-cpt>=0 && y_def-cpt>=0){
      pos_block[x_def-cpt][y_def-cpt]=-1;
      cpt++;
  }
  for(int i=0;i<N;i++){
      for(int j=0;j<M;j++){
          if ((pos_block[i][j-1]==-1 || pos_block[i][j-1]==0 || pos_block[i][j-1]==1) && j>y_def)
              pos_block[i][j]=1;
      }
  }
  for(int i=0;i<N;i++){
      for(int j=M-1;j>=0;j--){
          if ((pos_block[i][j+1]==-1 || pos_block[i][j+1]==0 || pos_block[i][j+1]==2) && j<y_def)
            pos_block[i][j]=2;
      }
  }
  for(int j=0;j<M;j++){
      for(int i=0;i<N;i++){
          if ((pos_block[i-1][j]==-1 || pos_block[i-1][j]==0 || pos_block[i-1][j]==3) && i>x_def)
              pos_block[i][j]=3;
      }
  }
  for(int j=0;j<M;j++){
      for(int i=0;i<N;i++){
          if (pos_block[i][j]==-2)
              pos_block[i][j]=4;
      }
  }
  //on compte le nb d'unité dans chaque direction, on ignore les diagonnales car une unité sur n'ipmporte quel diagonnale fera toujours 50% du block
  int nb_nord=0,nb_sud=0,nb_ouest=0,nb_est=0;

  for (int i=0;i<N;i++){
    for(int j=0;j<M;j++){
      if(terrain[i][j].piece && terrain[i][j].piece->joueur != joueur_actu){
        if(pos_block[i][j]==1){
          nb_nord++;
        }else if(pos_block[i][j]==2){
          nb_sud++;
        }else if(pos_block[i][j]==3){
          nb_ouest++;
        }else if(pos_block[i][j]==4){
          nb_est++;
        }
      }
    }
  }

  //on se tourne vers la meilleur direction
  if(nb_nord>=nb_sud && nb_nord>=nb_ouest && nb_nord>=nb_est){
    terrain[x_def][y_def].piece->direction=haut;

  }else if(nb_sud>=nb_nord && nb_sud>=nb_ouest && nb_sud>=nb_est){
    terrain[x_def][y_def].piece->direction=bas;

  }else if(nb_ouest>=nb_sud && nb_ouest>=nb_nord && nb_ouest>=nb_est){
    terrain[x_def][y_def].piece->direction=gauche;

  }else{
    terrain[x_def][y_def].piece->direction=droite;
  }

}