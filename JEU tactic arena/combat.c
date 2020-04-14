#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "combat.h"







void pathfinding_combat(case_t terrain[N][M], int x, int y,int joueur_actu){
  int attaque[N][M];
  int i,j;
  int cpt;

  //initialisation des attaques possibles
  for (i=0;i<N;i++){//parcours du terrain
      for(j=0;j<M;j++){
        terrain[i][j].attaque=0;
      }
  }
  if(terrain[x][y].piece==NULL){
    return;
  }

  for (i=0;i<N;i++){//parcours du terrain
      for(j=0;j<M;j++){
        attaque[i][j]=0;
      }
  }

    attaque[x][y]=1;//case ou se trouve la piece
    //gestion du terrain
    int portee_attaquant;
    if(terrain[x][y].type!=7){
      portee_attaquant=terrain[x][y].piece->portee;
    }else{
      portee_attaquant=terrain[x][y].piece->portee+1;
    }

    for(cpt=1;cpt<=portee_attaquant;cpt++){//tant qu'il y a des point de la portée
        for (i=0;i<N;i++){//parcours du terrain
            for(j=0;j<M;j++){
                if (attaque[i][j]==cpt){
                  if(i+1<N && attaque[i+1][j]==0)
                    attaque[i+1][j]=cpt+1;//ICI
                  if(i-1>=0 && attaque[i-1][j]==0)
                    attaque[i-1][j]=cpt+1;
                  if(j+1<M && attaque[i][j+1]==0)
                    attaque[i][j+1]=cpt+1;
                  if(j-1>=0 && attaque[i][j-1]==0)
                    attaque[i][j-1]=cpt+1;
                }

            }
        }
    }
    for (i=0;i<N;i++){
      for(j=0;j<M;j++){
        if(terrain[i][j].piece && terrain[i][j].piece->joueur==joueur_actu && attaque[i][j]!=0)
          attaque[i][j]=-2;
      }
    }

    for (i=0;i<N;i++){//parcours du terrain
        for(j=0;j<M;j++){
            if(attaque[i][j]>1 && terrain[i][j].piece && terrain[i][j].piece->joueur!=joueur_actu && terrain[x][y].piece->classe!=priest)
              terrain[i][j].attaque=1;
            if(attaque[i][j]==-2 && terrain[i][j].piece && terrain[i][j].piece->joueur==joueur_actu && terrain[x][y].piece->classe==priest)
              terrain[i][j].attaque=1;
            if(attaque[i][j]>1 && terrain[i][j].bloc!=NULL && terrain[x][y].piece->joueur!=terrain[i][j].bloc->block_allie && terrain[x][y].piece->classe!=priest)
              terrain[i][j].attaque=1;
        }
    }


}

int calc_block(case_t terrain [N][M],int x_att, int y_att, int x_def,int y_def){
    int pos_block[N][M];
    int cpt=1;
    int block=0;
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


    if(terrain[x_def][y_def].piece->direction==haut){
        if (pos_block[x_att][y_att]==2){
            block= 0;
        }
        else if (pos_block[x_att][y_att]==3 || pos_block[x_att][y_att]==4 || pos_block[x_att][y_att]==(-1)){
            block=((terrain[x_def][y_def].piece->block)/2);
        }
        else{
            block= (terrain[x_def][y_def].piece->block);
        }
    }
    else if(terrain[x_def][y_def].piece->direction==bas){
        if (pos_block[x_att][y_att]==1){
            block= 0;
        }
        else if (pos_block[x_att][y_att]==3 || pos_block[x_att][y_att]==4 || pos_block[x_att][y_att]==-1){
            block= ((terrain[x_def][y_def].piece->block)/2);
        }
        else{
            block= (terrain[x_def][y_def].piece->block);
        }
    }
    else if(terrain[x_def][y_def].piece->direction==gauche){
        if (pos_block[x_att][y_att]==4){
            block= 0;
        }
        else if (pos_block[x_att][y_att]==1 || pos_block[x_att][y_att]==2 || pos_block[x_att][y_att]==-1){
            block= ((terrain[x_def][y_def].piece->block)/2);
        }
        else{
            block= (terrain[x_def][y_def].piece->block);
        }
    }
    else if(terrain[x_def][y_def].piece->direction==droite){
        if (pos_block[x_att][y_att]==3){
            block= 0;
        }
        else if (pos_block[x_att][y_att]==1 || pos_block[x_att][y_att]==2 || pos_block[x_att][y_att]==-1){
            block= ((terrain[x_def][y_def].piece->block)/2);
        }
        else{
            block= (terrain[x_def][y_def].piece->block);
        }
    }
    return block;
}




void combat(case_t terrain [N][M],int x_att, int y_att, int x_def,int y_def,int joueur,joueurs_t tab[J],degatx_t aff_deg[AFF_DEG],bash_t tab_info_bash[TAILLE_TAB_BASH],char variable2[80]){
  if(tab[joueur].pts_action_actu>0 && terrain[x_att][y_att].piece!=NULL && terrain[x_att][y_att].piece->pts_action_actu>0){
      if(terrain[x_att][y_att].piece->classe==priest){//soin si la piece selectionné est un pretre
          soin(terrain,x_att,y_att,x_def,y_def,joueur,tab,aff_deg,tab_info_bash,variable2);
          return;
      }
      if(terrain[x_def][y_def].attaque==1 && terrain[x_def][y_def].piece!=NULL && terrain[x_def][y_def].bloc==NULL){//si la piece attaquée est sur une case a portée
        int blockd=((rand()%(100)) + 1);//att bloquée ou non
        int blockage;
        char variable[80];
        int armure;
        //avantages du terrain de foret
        if(terrain[x_def][y_def].type==6){
          blockage = calc_block(terrain,x_att,y_att,x_def,y_def) + 10;
          armure = terrain[x_def][y_def].piece->armure + 9;
        }else{
          blockage = calc_block(terrain,x_att,y_att,x_def,y_def);
          armure = terrain[x_def][y_def].piece->armure;
        }

        if(blockd>=blockage){

          int deg=((terrain[x_att][y_att].piece->puissance)*(100-armure)/100);

          if(deg>=0){
            terrain[x_def][y_def].piece->pdv-=deg;//armure=absorption de dégats //application des dégats ici

            sprintf(variable, "| Unite de Joueur %d en %d/%d attaque pour %d en %d/%d",joueur,x_att,y_att,deg,x_def,y_def);
			    	ajouter_ligne_bash(variable,tab_info_bash,degat,variable2);

            sprintf(variable, "%d", deg);//sauvegarde des dégat pour affichage
            ajouter_degat_txt(variable,aff_deg,(terrain[x_def][y_def].xImg+50),(terrain[x_def][y_def].yImg),1);


          }else{
            sprintf(variable, "| Unite de Joueur %d en %d/%d attaque pour 0 en %d/%d",joueur,x_att,y_att,x_def,y_def);
			    	ajouter_ligne_bash(variable,tab_info_bash,ineficace,variable2);
            ajouter_degat_txt("0",aff_deg,(terrain[x_def][y_def].xImg+50),(terrain[x_def][y_def].yImg),0);
          }
          if(terrain[x_def][y_def].piece->pdv<=0){   //mort ou non de la piece ennemi

            sprintf(variable, "| Unite de Joueur %d en %d/%d tue Unite de %d en %d/%d",joueur,x_att,y_att,terrain[x_def][y_def].piece->joueur,x_def,y_def);
			    	ajouter_ligne_bash(variable,tab_info_bash,kill,variable2);

            tab[terrain[x_def][y_def].piece->joueur].nb_unite--;
            terrain[x_def][y_def].attaque=0;
            terrain[x_att][y_att].piece->kill+=1;
            ajouter_degat_txt("KILL",aff_deg,(terrain[x_def][y_def].xImg),(terrain[x_def][y_def].yImg+20),3);
            free(terrain[x_def][y_def].piece);
            terrain[x_def][y_def].piece=NULL;
            update_stats(terrain,x_att,y_att,joueur,tab);
          }
        }else{

          sprintf(variable, "| Unite de Joueur %d en %d/%d block Unite de %d en %d/%d ",terrain[x_def][y_def].piece->joueur,x_def,y_def,joueur,x_att,y_att);
			    ajouter_ligne_bash(variable,tab_info_bash,4,variable2);


          ajouter_degat_txt("BLOCK",aff_deg,(terrain[x_def][y_def].xImg+50),(terrain[x_def][y_def].yImg),0);
        }
        tab[joueur].pts_action_actu--;
        terrain[x_att][y_att].piece->pts_action_actu--;

      }else if(terrain[x_def][y_def].attaque==1 && terrain[x_def][y_def].bloc){ // cas ou c'est un batiment
        char variable[80];
        int armure = terrain[x_def][y_def].bloc->armure;
        int deg=((terrain[x_att][y_att].piece->puissance)*(100-armure)/100);

        terrain[x_def][y_def].bloc->pdv_block-=deg;

        sprintf(variable, "| Unite de Joueur %d en %d/%d attaque batiment pour %d en %d/%d",joueur,x_att,y_att,deg,x_def,y_def);
			  ajouter_ligne_bash(variable,tab_info_bash,degat,variable2);

        sprintf(variable, "%d", deg);//sauvegarde des dégat pour affichage
        ajouter_degat_txt(variable,aff_deg,(terrain[x_def][y_def].xImg+50),(terrain[x_def][y_def].yImg),1);
        if(terrain[x_def][y_def].bloc->pdv_block<=0){   //destruction du bloc

            sprintf(variable, "| Unite de Joueur %d en %d/%d detruit batiment de %d en %d/%d",joueur,x_att,y_att,terrain[x_def][y_def].bloc->block_allie,x_def,y_def);
			    	ajouter_ligne_bash(variable,tab_info_bash,kill,variable2);
            int x_destru,y_destru,compteur=1;
            for(int i=0;i<N;i++){
              for(int j=0;j<N;j++){
                if(terrain[i][j].bloc && terrain[i][j].bloc->pdv_block<=0){
                  x_destru=i;
                  y_destru=j;
                  if(compteur<terrain[i][j].bloc->aire){
                    terrain[i][j].bloc=NULL;
                    compteur++;
                  }
                  if(terrain[i][j].piece){ //dégat d'éboulement sur la piece lors de la destruction de la tour
                    sprintf(variable, "| Unite de Joueur %d en %d/%d prend %d degat d'eboulement",terrain[i][j].piece->joueur,i,j,DEGAT_EBOULEMENT);
                    ajouter_ligne_bash(variable,tab_info_bash,degat,variable2);
                    sprintf(variable, "%d", DEGAT_EBOULEMENT);
                    ajouter_degat_txt(variable,aff_deg,(terrain[i][j].xImg+50),(terrain[i][j].yImg),1);
                    terrain[i][j].piece->pdv-=DEGAT_EBOULEMENT;
                    if(terrain[i][j].piece->pdv<=0){
                      sprintf(variable, "| Unite de Joueur %d en %d/%d meurt de l'eboulement",joueur,i,j);
                      ajouter_ligne_bash(variable,tab_info_bash,kill,variable2);

                      tab[terrain[i][j].piece->joueur].nb_unite--;
                      terrain[i][j].attaque=0;
                      ajouter_degat_txt("MORT",aff_deg,(terrain[i][j].xImg),(terrain[i][j].yImg+20),3);
                      free(terrain[i][j].piece);
                      terrain[i][j].piece=NULL;

                    }

                  }
                }
              }
            }

            free(terrain[x_def][y_def].bloc);
            terrain[x_destru][y_destru].bloc=NULL;

            ajouter_degat_txt("DETRUIT",aff_deg,(terrain[x_def][y_def].xImg),(terrain[x_def][y_def].yImg+20),3);

        }

        tab[joueur].pts_action_actu--;
        terrain[x_att][y_att].piece->pts_action_actu--;
      }

    }
}

void soin(case_t terrain [N][M],int x_att, int y_att, int x_def,int y_def,int joueur,joueurs_t tab[J],degatx_t aff_deg[AFF_DEG],bash_t tab_info_bash[TAILLE_TAB_BASH],char variable2[80]){
    char variable[80];
    if(terrain[x_def][y_def].attaque==1){
        terrain[x_def][y_def].piece->pdv+=terrain[x_att][y_att].piece->puissance;

        sprintf(variable, "| Unite de Joueur %d en %d/%d soigne de %d en %d/%d",joueur,x_att,y_att,terrain[x_att][y_att].piece->puissance,x_def,y_def);
				ajouter_ligne_bash(variable,tab_info_bash,soins,variable2);

        tab[joueur].pts_action_actu--;
        terrain[x_att][y_att].piece->pts_action_actu--;
        sprintf(variable, "%d", terrain[x_att][y_att].piece->puissance);
        ajouter_degat_txt(variable,aff_deg,(terrain[x_def][y_def].xImg+50),(terrain[x_def][y_def].yImg),2);
    }
}



int a_portee(case_t terrain[N][M],int x_bot,int y_bot,int joueur_actu){
  pathfinding_combat(terrain,x_bot,y_bot,joueur_actu);
  int nb=0;
  for(int i=0;i<N;i++){
    for(int j=0;j<M;j++){
      if(terrain[i][j].attaque==1){
        nb++;
      }
    }
  }
  return nb;
}




void update_stats(case_t terrain[N][M],int x,int y,int joueur_actu,joueurs_t tab[J]){



        //++++++KNIGHT++++++++//
  if(terrain[x][y].piece->classe==knight){
    //===bonus a chaque kill=====//
    terrain[x][y].piece->puissance+=terrain[x][y].piece->kill;
    if(terrain[x][y].piece->armure<90){
      terrain[x][y].piece->armure+=1;
    }
    //===bonus tous les 3 kills=====//
    if(terrain[x][y].piece->kill % 3==0){
      terrain[x][y].piece->pdv+=30;
    }
    //===bonus tous les 7 kills=====//
    if(terrain[x][y].piece->kill % 7==0){
      terrain[x][y].piece->deplacement+=1;
    }
    //===bonus de vétérant de la guerre(10 kills)===//
    if(terrain[x][y].piece->kill==10){
      terrain[x][y].piece->deplacement+=1;
      terrain[x][y].piece->portee+=1;
      terrain[x][y].piece->puissance+=10;
      terrain[x][y].piece->armure+=5;
      terrain[x][y].piece->block+=10;
    }



        //++++++SCOUT++++++++//
  }else if(terrain[x][y].piece->classe==scout){
    //===bonus a chaque kill=====//
    terrain[x][y].piece->puissance+=terrain[x][y].piece->kill;
    //===bonus tous les 2 kills=====//
    if(terrain[x][y].piece->kill % 2==0){
      terrain[x][y].piece->pdv+=10;
    }
    //===bonus tous les 3 kills=====//
    if(terrain[x][y].piece->kill % 3==0){
      terrain[x][y].piece->deplacement+=1;
    }
    //===bonus de vétérant de la guerre(10 kills)===//
    if(terrain[x][y].piece->kill==10){
      terrain[x][y].piece->puissance+=13;
      terrain[x][y].piece->armure+=10;
      terrain[x][y].piece->block+=20;
      terrain[x][y].piece->portee+=1;
    }





        //++++++MAGICIAN++++++++//
  }else if(terrain[x][y].piece->classe==magician){
    //===bonus a chaque kill=====//
    terrain[x][y].piece->puissance+=terrain[x][y].piece->kill;
    terrain[x][y].piece->pdv+=terrain[x][y].piece->kill;
    //===bonus tous les 5 kills=====//
    if(terrain[x][y].piece->kill % 5==0){
      terrain[x][y].piece->pdv+=15;
    }
    //===bonus tous les 10 kills=====//
    if(terrain[x][y].piece->kill % 10==0){
      terrain[x][y].piece->portee+=1;
    }

    //===bonus de vétérant de la guerre(10 kills)===//
    if(terrain[x][y].piece->kill==10){
      terrain[x][y].piece->puissance+=14;
      terrain[x][y].piece->armure+=5;
      terrain[x][y].piece->block+=5;
      terrain[x][y].piece->deplacement+=1;
    }





       //++++++FAUCHEUSE++++++++//
  }else if(terrain[x][y].piece->classe==faucheuse){
      //===bonus a chaque kill=====//
    terrain[x][y].piece->puissance+=terrain[x][y].piece->kill;
    //===bonus tous les 5 kills=====//
    if(terrain[x][y].piece->kill % 5==0){
      if(terrain[x][y].piece->block<90){
        terrain[x][y].piece->block+=10;
      }
    }
    //===bonus de vétérant de la guerre(10 kills)===//
    if(terrain[x][y].piece->kill==10){
      terrain[x][y].piece->deplacement+=1;
      terrain[x][y].piece->puissance+=15;
      terrain[x][y].piece->portee+=1;
      terrain[x][y].piece->block+=10;
    }




     //++++++ANGE++++++++//
  }else if(terrain[x][y].piece->classe==ange){
      //===bonus a chaque kill=====//
    terrain[x][y].piece->puissance+=terrain[x][y].piece->kill;
    tab[joueur_actu].pts_action_actu+=1;
    terrain[x][y].piece->pts_action_actu++;
    terrain[x][y].piece->pdv+=(terrain[x][y].piece->kill)*3;
    //===bonus tous les 5 kills=====//
    if(terrain[x][y].piece->kill % 5==0){
      terrain[x][y].piece->puissance+=terrain[x][y].piece->kill;
      terrain[x][y].piece->deplacement+=1;
    }
    //===bonus de vétérant de la guerre(10 kills)===//
    if(terrain[x][y].piece->kill==10){
      terrain[x][y].piece->puissance+=15;
      terrain[x][y].piece->armure+=15;
      terrain[x][y].piece->deplacement+=1;
    }



      //++++++AUTRE++++++++//
  }else if(terrain[x][y].piece->classe==priest){ //autre unité

  }

}
