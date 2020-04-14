#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "camera.h"






void centrer_camera(case_t terrain[N][M],int x,int y,int largeur,int hauteur){
	int x_centre,y_centre,diff_x,diff_y;
	x_centre=largeur/2;
	y_centre=hauteur/2;
	diff_x=abs(x_centre-x);
	diff_y=abs(y_centre-y);

	if(x<=x_centre){
		for(int i=0;i<N;i++){
			for(int j=0;j<M;j++){
				terrain[i][j].xImg+=diff_x;
				terrain[i][j].x1+=diff_x;
				terrain[i][j].x2+=diff_x;
				terrain[i][j].x3+=diff_x;
				terrain[i][j].x4+=diff_x;
			}
		}
	}else{
		for(int i=0;i<N;i++){
			for(int j=0;j<M;j++){
				terrain[i][j].xImg-=diff_x;
				terrain[i][j].x1-=diff_x;
				terrain[i][j].x2-=diff_x;
				terrain[i][j].x3-=diff_x;
				terrain[i][j].x4-=diff_x;
			}
		}
	}

	if(y<=y_centre){
		for(int i=0;i<N;i++){
			for(int j=0;j<M;j++){
				terrain[i][j].yImg+=diff_y;
				terrain[i][j].y1+=diff_y;
				terrain[i][j].y2+=diff_y;
				terrain[i][j].y3+=diff_y;
				terrain[i][j].y4+=diff_y;
			}
		}
	}else{
		for(int i=0;i<N;i++){
			for(int j=0;j<M;j++){
				terrain[i][j].yImg-=diff_y;
				terrain[i][j].y1-=diff_y;
				terrain[i][j].y2-=diff_y;
				terrain[i][j].y3-=diff_y;
				terrain[i][j].y4-=diff_y;
			}
		}
	}
}




void camera_sur_allie(case_t terrain[N][M],int joueur_actu,joueurs_t tab[J],int * largeur,int * hauteur){
  int compteur=0;
  int var=rand()%tab[joueur_actu].nb_unite;
	int x=-1,y=-1;
	for(int i=0;i<N;i++){
    for(int j=0;j<M;j++){
			if(terrain[i][j].piece && terrain[i][j].piece->select==1){
				terrain[i][j].piece->select=0;
				x=i;
				y=j;
			}
		}
	}
  for(int i=0;i<N;i++){
    for(int j=0;j<M;j++){
      if(terrain[i][j].piece && terrain[i][j].piece->joueur==joueur_actu)
        compteur++;
      if(terrain[i][j].piece && terrain[i][j].piece->joueur==joueur_actu && terrain[i][j].piece->select==0 && compteur>=var && terrain[i][j].piece->pts_action_actu>0 && (x!=i || y!=j)){
				terrain[i][j].piece->select=1;
        centrer_camera(terrain,terrain[i][j].xImg,terrain[i][j].yImg,*largeur,*hauteur);
        return;
      }
    }
    if(i==N-1)
      i=0;
  }
}




