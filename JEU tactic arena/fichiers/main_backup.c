#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>


#include "piece.h"
#include "interface.h"






//=====================================MAIN=============================//

int main(int argc, char** argv)
{


	SDL_Window* pWindow = NULL;
	SDL_Renderer *renderer=NULL;
  SDL_Rect imgDestRect;
	SDL_Rect img_anim;
	char variable[50];
	int *largeur=0,*hauteur=0;
	largeur=malloc(sizeof(*largeur));
	hauteur=malloc(sizeof(*hauteur));
	float coefZoom=1;
  int alea;
	int sel=0;
	int fin_tour=0;
  int var1;
	int nb_tour=0;
	int compteur_anim=VITESSE_ANIM;
	int scroll_speed=20;
	int x_bot,y_bot;
	case_t terrain[N][M];
	srand(time(NULL));
	FILE * fp;
	joueurs_t tab[J];
	int nb_joueur_restant;
	int joueur_actu=0;
	int clicout;

	image_t image[Z];

	degatx_t aff_deg[N*M];

	SDL_Color c = {0,0,0,0};
	SDL_Color c_rouge = {255,0,0,0};
	SDL_Color c_verte = {125,255,125,0};
	SDL_Color c_verte2 = {15,109,15,0};
	SDL_Color b = {255,255,255,255};

    /* Initialisation SDL*/
    if (SDL_Init(SDL_INIT_VIDEO) != 0 ) {
        fprintf(stdout,"Échec de l'initialisation de la SDL (%s)\n",SDL_GetError());
        return -1;
    }
		SDL_DisplayMode dm;
		SDL_GetCurrentDisplayMode(0,&dm);

		int SCREEN_HEIGHT = dm.h;
		int SCREEN_WIDTH = dm.w;
	/* Initialisation TTF */
	if(TTF_Init() == -1) {
		fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
		exit(EXIT_FAILURE);
	}
	/* Création de la fenêtre */
	pWindow = SDL_CreateWindow("Tactic Arena Upgraded V1.0",SDL_WINDOWPOS_UNDEFINED,
												  SDL_WINDOWPOS_UNDEFINED,
												  SCREEN_WIDTH,
												  SCREEN_HEIGHT,
												  SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);



	SDL_GetWindowSize(pWindow,largeur,hauteur); //initialisation des pointeur largeur et hauteur sur la taille de la fenetre

	if(!pWindow){
		fprintf(stderr, "Erreur à la création de la fenetre : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	renderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
	if(renderer == NULL){
		fprintf(stderr, "Erreur à la création du renderer\n");
		exit(EXIT_FAILURE);
	}

/* a tester, ne fonctionne pas sur windows (l'icone reste le meme)
	SDL_Surface* iconSurface;
	iconSurface = SDL_LoadBMP("images/icon.bmp");
	SDL_SetWindowIcon(pWindow, iconSurface);
*/

//===================================TEXTURES==========================================//

loadImage(image,renderer);

//==================================INITIALISATION GRILLE============================//
    //choix du mode de génération
//	printf("1 POUR CARTE ALEATOIRE, AUTRE CHIFFRE POUR UNE DES CARTES PRESET : ");
//	scanf("%d",&preset);

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

		}
	}

//initialisation tableau d'affichage des degats morts et soins

	for(int i=0;i<(N*M);i++){
		aff_deg[i].time=-1;
		aff_deg[i].pos_x=0;
		aff_deg[i].pos_y=0;
		aff_deg[i].c=0;
		strcpy(aff_deg[i].txt,"");
	}

	//initialisation de la grille//

	(imgDestRect.w)=100;
	(imgDestRect.h)=100;
	int bordure=43;                                                                                                                    //les coef ci dessous permettent d'aligner les images car elle ne sont pas totalement droite dans le sprite
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
			tab[i].pts_action_max=PTS_ACTION_MAX;
			tab[i].pts_action_actu=tab[i].pts_action_max;
			tab[i].nb_unite=NB_UNITE;
		}


//=========================================================INITIALISATION DES UNITEES===========================================//
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


//========================================FENETRE=====================================//

if( pWindow )
{
	running = Menu;
	SDL_Event e;
	while(running != Quit) {
		if(running == Menu){
			int longeur = draw_menu(renderer,pWindow);
			while(SDL_PollEvent(&e)) {
				switch(e.type) {
					case SDL_QUIT:	//cas ou l'on souhaite quitter
						running = Quit;
					break;
					case SDL_MOUSEBUTTONDOWN:
						if(e.button.button == SDL_BUTTON_LEFT){
							if (e.button.y > longeur && e.button.y < longeur*2)running = Menu2;
							if (e.button.y > longeur*2 && e.button.y < longeur*3)running = Load;
							if (e.button.y > longeur*3 && e.button.y < longeur*4)running = About;
							if (e.button.y > longeur*4 && e.button.y < longeur*5)running = Quit;
						}
					break;
				}
			}
		}else if(running == Menu2){
			int nb_j=J,nb_o=0,nb_unit=NB_CLASSE,taille=N;
			int x1 = gpScreen->w - gpScreen->w/5;
			int x2 = gpScreen->w - gpScreen->w/10;
			int fontsize = gpScreen->h/10;
			int y = fontsize/2;
			while(SDL_PollEvent(&e)||running == Menu2) {
				mod_menu(renderer,pWindow,nb_j,nb_o,taille,nb_unit);
				switch(e.type) {
					case SDL_QUIT:	//cas ou l'on souhaite quitter
						running = Quit;
					break;
					case SDL_MOUSEBUTTONDOWN:
						if(e.button.button == SDL_BUTTON_LEFT){
							if((nb_j+nb_o)<5){
								if(e.button.x>x1&&e.button.x<x1+fontsize){
									if(e.button.y>y && e.button.y<y+fontsize)nb_j++;
									if(e.button.y>y*5 && e.button.y<y*5+fontsize)nb_o++;
								}
							}
							if((nb_j+nb_o)>0){
								if(e.button.x>x2&&e.button.x<x2+fontsize){
									if(e.button.y>y && e.button.y<y+fontsize&&nb_j>0)nb_j--;
									if(e.button.y>y*5 && e.button.y<y*5+fontsize&&nb_o>0)nb_o--;
								}
							}
							if(taille>=10&&taille<=25){
								if(e.button.x>x1&&e.button.x<x1+fontsize&&e.button.y>y*9&&e.button.y<y*9+fontsize&&taille<25)taille++;
								if(e.button.x>x2&&e.button.x<x2+fontsize&&e.button.y>y*9&&e.button.y<y*9+fontsize&&taille>10)taille--;
							}
							if(nb_unit>=1&&nb_unit<=5){
								if(e.button.x>x1&&e.button.x<x1+fontsize&&e.button.y>y*13&&e.button.y<y*13+fontsize&&nb_unit<5)nb_unit++;
								if(e.button.x>x2&&e.button.x<x2+fontsize&&e.button.y>y*13&&e.button.y<y*13+fontsize&&nb_unit>1)nb_unit--;
							}
							if(e.button.y > gpScreen->h/5*4 && e.button.y < gpScreen->h){



								running = Play;
							}
						}
					break;
				}
			}
		}else if(running == Load){
			running = Play;
		}else if(running == Help){
			help(renderer,pWindow);
			while(SDL_PollEvent(&e)) {
				switch(e.type) {
					case SDL_QUIT:	//cas ou l'on souhaite quitter
						running = Quit;
					break;
					case SDL_MOUSEBUTTONDOWN:
						if(e.button.button == SDL_BUTTON_LEFT){
							if(e.button.x > gpScreen->w - 60 && e.button.y >  gpScreen->h - 30){
								running = Play;
							}
						}
					break;
				}
			}
		}else if(running == About){
			about(renderer,pWindow);
			while(SDL_PollEvent(&e)) {
				switch(e.type) {
					case SDL_QUIT:	//cas ou l'on souhaite quitter
						running = Quit;
					break;
					case SDL_MOUSEBUTTONDOWN:
						if(e.button.button == SDL_BUTTON_LEFT){
							if(e.button.x > gpScreen->w - 60 && e.button.y >  gpScreen->h - 30){
								running = Menu;
							}
						}
					break;
				}
			}
		}else if(running == Play){ //boucle de jeu
			while(SDL_PollEvent(&e)) {
				switch(e.type) {
					case SDL_QUIT:	//cas ou l'on souhaite quitter
						running = Quit;
					break;

					case SDL_KEYDOWN: //cas ou une touche est pressé
						switch (e.key.keysym.sym)  //switch qui gere les touches
						{
							case SDLK_LEFT:
								for (int compteur=0;compteur<N;compteur++){
									for (int compteur2=0;compteur2<M;compteur2++){
										terrain[compteur][compteur2].xImg+=scroll_speed;
										terrain[compteur][compteur2].x1+=scroll_speed;
										terrain[compteur][compteur2].x2+=scroll_speed;
										terrain[compteur][compteur2].x3+=scroll_speed;
										terrain[compteur][compteur2].x4+=scroll_speed;
									}
								}
								break;
							case SDLK_RIGHT:
								for (int compteur=0;compteur<N;compteur++){
									for (int compteur2=0;compteur2<M;compteur2++){
										terrain[compteur][compteur2].xImg-=scroll_speed;
										terrain[compteur][compteur2].x1-=scroll_speed;
										terrain[compteur][compteur2].x2-=scroll_speed;
										terrain[compteur][compteur2].x3-=scroll_speed;
										terrain[compteur][compteur2].x4-=scroll_speed;
									}
								}
								break;
							case SDLK_UP:
								for (int compteur=0;compteur<N;compteur++){
									for (int compteur2=0;compteur2<M;compteur2++){
										terrain[compteur][compteur2].yImg+=scroll_speed;
										terrain[compteur][compteur2].y1+=scroll_speed;
										terrain[compteur][compteur2].y2+=scroll_speed;
										terrain[compteur][compteur2].y3+=scroll_speed;
										terrain[compteur][compteur2].y4+=scroll_speed;
									}
								}
								break;
							case SDLK_DOWN:
								for (int compteur=0;compteur<N;compteur++){
									for (int compteur2=0;compteur2<M;compteur2++){
										terrain[compteur][compteur2].yImg-=scroll_speed;
										terrain[compteur][compteur2].y1-=scroll_speed;
										terrain[compteur][compteur2].y2-=scroll_speed;
										terrain[compteur][compteur2].y3-=scroll_speed;
										terrain[compteur][compteur2].y4-=scroll_speed;
									}
								}
								break;
							case SDLK_SPACE:
								ingame_menu(renderer,pWindow);
								running = In_menu;
								while(running == In_menu){
									while(SDL_PollEvent(&e)){
											switch(e.type) {
											case SDL_QUIT:	//cas ou l'on souhaite quitter
												running = Quit;
											break;
											case SDL_MOUSEBUTTONDOWN:
												if(e.button.button == SDL_BUTTON_LEFT){
													if(e.button.x > gpScreen->w - 260 && e.button.x < gpScreen->w - 60){
														if(e.button.y>gpScreen->h - 330 && e.button.y<gpScreen->h - 330+75)running = Play;
														if(e.button.y>gpScreen->h - 330+75 && e.button.y<gpScreen->h - 330+75*2)running = Play;
														if(e.button.y>gpScreen->h - 330+75*2 && e.button.y<gpScreen->h - 330+75*3)running = Help;
														if(e.button.y>gpScreen->h - 330+75*3 && e.button.y<gpScreen->h - 330+75*4)running = Menu;
													}else{
														running = Play;
													}
												}
											break;
											case SDL_KEYDOWN:
												if(e.key.keysym.sym == SDLK_SPACE){
													running = Play;
												}
											break;
										}
									}
								}
								break;
						}
						break;

						case SDL_MOUSEBUTTONDOWN:
							switch (e.button.button)
							{
								case SDL_BUTTON_LEFT:
									if(e.button.x > gpScreen->w - 60 && e.button.y >  gpScreen->h - 30){
										ingame_menu(renderer,pWindow);
										running = In_menu;
										while(running == In_menu){
											while(SDL_PollEvent(&e)){
													switch(e.type) {
													case SDL_QUIT:	//cas ou l'on souhaite quitter
														running = Quit;
													break;
													case SDL_MOUSEBUTTONDOWN:
														if(e.button.button == SDL_BUTTON_LEFT ){
															if(e.button.x > gpScreen->w - 260 && e.button.x < gpScreen->w - 60){
																if(e.button.y>gpScreen->h - 330 && e.button.y<gpScreen->h - 330+75)running = Play;
																if(e.button.y>gpScreen->h - 330+75 && e.button.y<gpScreen->h - 330+75*2)running = Play;
																if(e.button.y>gpScreen->h - 330+75*2 && e.button.y<gpScreen->h - 330+75*3)running = Help;
																if(e.button.y>gpScreen->h - 330+75*3 && e.button.y<gpScreen->h - 330+75*4)running = Menu;
															}else
																running = Play;
														}
													break;
												}
											}
										}
									}
									if(tab[joueur_actu].humain==1){
									clicout=0;
									//int a,b,a2,b2,a3,b3,a4,b4,res1,res2,res3,res4;
									for(int compteur=0;compteur<N;compteur++){
										for(int compteur2=0;compteur2<M; compteur2++){
												/* VERSION AVANCER NE FONCTIONNANT PAS ENCORE
												fprintf(stderr,"POUR  %d | %d : \n",compteur,compteur2);
												//Y
												a=((terrain[compteur][compteur2].y1-terrain[compteur][compteur2].y4)/(terrain[compteur][compteur2].x1-terrain[compteur][compteur2].x4));
												b= (terrain[compteur][compteur2].y4-a*terrain[compteur][compteur2].x4);
												res1=a*e.motion.x+b;j
												fprintf(stderr,"a = %d et b = %d pour x= %d et y = %d , doit dépasser %d en y \n",a,b,e.motion.x,e.motion.y,res1);
												//X
												a2=((terrain[compteur][compteur2].y2-terrain[compteur][compteur2].y1)/(terrain[compteur][compteur2].x2-terrain[compteur][compteur2].x1));
												b2= (terrain[compteur][compteur2].y1-a*terrain[compteur][compteur2].x1);
												res2=-1*(e.motion.y-b2)/a2;
												fprintf(stderr,"a2 = %d et b2= %d pour x= %d et y = %d ,doit dépasser %d en x \n",a2,b2,e.motion.x,e.motion.y,res2);
												//Y
												a3=((terrain[compteur][compteur2].y2-terrain[compteur][compteur2].y3)/(terrain[compteur][compteur2].x2-terrain[compteur][compteur2].x3));
												b3= (terrain[compteur][compteur2].y3-a*terrain[compteur][compteur2].x3);
												res3 = a3*e.motion.x+b3;
												fprintf(stderr,"a3 = %d et b3= %d pour x= %d et y = %d , ne doit pas dépasser %d en y \n",a3,b3,e.motion.x,e.motion.y,res3);
												//X
												a4=((terrain[compteur][compteur2].y3-terrain[compteur][compteur2].y4)/(terrain[compteur][compteur2].x3-terrain[compteur][compteur2].x4));
												b4= (terrain[compteur][compteur2].y4-a*terrain[compteur][compteur2].x4);
												res4 = -1*(e.motion.y-b4)/a4;
												fprintf(stderr,"a4 = %d et b4= %d pour x= %d et y = %d , ne doit pas dépasser %d en x \n",a4,b4,e.motion.x,e.motion.y,res4);

												if(e.motion.y >  res1 && e.motion.x > res2  && e.motion.y <  res3 && e.motion.x <  res4 ){
													fprintf(stderr,"la case %d | %d a été selectionné. \n",compteur,compteur2);
												}
												*/

												//VERSION BASIQUE QUI FONCTIONNE MAL
												if(e.motion.y > terrain[compteur][compteur2].y1+10 && e.motion.y < terrain[compteur][compteur2].y3-10 && e.motion.x > terrain[compteur][compteur2].x4+10 && e.motion.x < terrain[compteur][compteur2].x2-10 &&!(e.motion.x>(*largeur)/2+175 && e.motion.x<(*largeur)/2+175+150 && e.motion.y>15 && e.motion.y<15+50)&&!(e.motion.x>10 && e.motion.x<240 && e.motion.y>300 && e.motion.y<500)){
													//=================SELECTION D'UNE PIECE====================//
													if(terrain[compteur][compteur2].piece){
														clicout=1;

														for(int x=0;x<N;x++){
															for(int y=0;y<M;y++){
																if(terrain[x][y].piece && terrain[x][y].piece->joueur==joueur_actu && terrain[x][y].piece->select==1){
																	combat(terrain,x,y,compteur,compteur2,joueur_actu,tab,aff_deg);
																}
															}
														}
														for(int indice=0;indice<N;indice++){
															for(int indice2=0;indice2<M; indice2++){
																if(terrain[indice][indice2].piece && terrain[indice][indice2].piece->select==1){
																	terrain[indice][indice2].piece->select=0;
																}
															}
														}

														if(terrain[compteur][compteur2].piece)
															terrain[compteur][compteur2].piece->select=1;


													}else{
													//=================DEPLACEMENT et ATTAQUE D'UNE PIECE====================//

													move(terrain,compteur,compteur2,joueur_actu,tab);
													}

												}
												else if(e.motion.x>(*largeur)/3+420 && e.motion.x<(*largeur)/2+175+150 && e.motion.y>15 && e.motion.y<15+50){
													fin_tour=1;
												}
											}
										}
										if(e.motion.x>10 && e.motion.x<240 && e.motion.y>300 && e.motion.y<500){
											clicout=1;
											for(int indice=0;indice<N;indice++){
												for(int indice2=0;indice2<M; indice2++){
													if(terrain[indice][indice2].piece && terrain[indice][indice2].piece->select==1 && terrain[indice][indice2].piece->joueur==joueur_actu){
														if(terrain[indice][indice2].piece->direction==1)
															terrain[indice][indice2].piece->direction=4;
														else if(terrain[indice][indice2].piece->direction==4)
															terrain[indice][indice2].piece->direction=2;
														else if(terrain[indice][indice2].piece->direction==2)
															terrain[indice][indice2].piece->direction=3;
														else if(terrain[indice][indice2].piece->direction==3)
															terrain[indice][indice2].piece->direction=1;
													}
												}
											}
										}
										if(clicout==0){
											for(int indice=0;indice<N;indice++){
												for(int indice2=0;indice2<M; indice2++){
													if(terrain[indice][indice2].piece && terrain[indice][indice2].piece->select==1)
														terrain[indice][indice2].piece->select=0;
												}

										}
									}
									}
								break;
							}
				break;
				//ZOOM
				/*case SDL_MOUSEWHEEL:
					if(e.wheel.y==1){
						coefZoom+=0.01;
						for(int a=0;a<N;a++){
							for(int b=0;b<M;b++){
								terrain[a][b].xImg-=15;
								terrain[a][b].yImg-=15;
								terrain[a][b].x1-=15;
								terrain[a][b].x2-=15;
								terrain[a][b].x3-=15;
								terrain[a][b].x4-=15;
								terrain[a][b].y1-=15;
								terrain[a][b].y2-=15;
								terrain[a][b].y3-=15;
								terrain[a][b].y4-=15;
							}
						}

					}
					else{
						coefZoom-=0.01;
						for(int a=0;a<N;a++){
							for(int b=0;b<M;b++){
								terrain[a][b].xImg+=15;
								terrain[a][b].yImg+=15;
								terrain[a][b].x1+=15;
								terrain[a][b].x2+=15;
								terrain[a][b].x3+=15;
								terrain[a][b].x4+=15;
								terrain[a][b].y1+=15;
								terrain[a][b].y2+=15;
								terrain[a][b].y3+=15;
								terrain[a][b].y4+=15;
							}
						}
					}

				break;
				*/
					}
				}
				gpScreen = SDL_GetWindowSurface(pWindow);
				if( e.motion.x >0 && e.motion.x <30 && e.type!=SDL_MOUSEWHEEL){
					for (int compteur=0;compteur<N;compteur++){
						for (int compteur2=0;compteur2<M;compteur2++){
							terrain[compteur][compteur2].xImg+=scroll_speed;
							terrain[compteur][compteur2].x1+=scroll_speed;
							terrain[compteur][compteur2].x2+=scroll_speed;
							terrain[compteur][compteur2].x3+=scroll_speed;
							terrain[compteur][compteur2].x4+=scroll_speed;
						}
					}
				}
				if(e.motion.x > gpScreen->w - 30 && e.motion.x < gpScreen->w && e.type!=SDL_MOUSEWHEEL){
					for (int compteur=0;compteur<N;compteur++){
						for (int compteur2=0;compteur2<M;compteur2++){
							terrain[compteur][compteur2].xImg-=scroll_speed;
							terrain[compteur][compteur2].x1-=scroll_speed;
							terrain[compteur][compteur2].x2-=scroll_speed;
							terrain[compteur][compteur2].x3-=scroll_speed;
							terrain[compteur][compteur2].x4-=scroll_speed;
						}
					}
				}
				if( e.motion.y >0 && e.motion.y <30 && e.type!=SDL_MOUSEWHEEL){
					for (int compteur=0;compteur<N;compteur++){
						for (int compteur2=0;compteur2<M;compteur2++){
							terrain[compteur][compteur2].yImg+=scroll_speed;
							terrain[compteur][compteur2].y1+=scroll_speed;
							terrain[compteur][compteur2].y2+=scroll_speed;
							terrain[compteur][compteur2].y3+=scroll_speed;
							terrain[compteur][compteur2].y4+=scroll_speed;
						}
					}
				}
				if(e.motion.y > gpScreen->h - 30 && e.motion.y < gpScreen->h && e.type!=SDL_MOUSEWHEEL){
					for (int compteur=0;compteur<N;compteur++){
						for (int compteur2=0;compteur2<M;compteur2++){
							terrain[compteur][compteur2].yImg-=scroll_speed;
							terrain[compteur][compteur2].y1-=scroll_speed;
							terrain[compteur][compteur2].y2-=scroll_speed;
							terrain[compteur][compteur2].y3-=scroll_speed;
							terrain[compteur][compteur2].y4-=scroll_speed;
						}
					}
				}

			//fin action du joueur
			if(tab[joueur_actu].humain==0){ 	//===================================================TOUR DU BOT===================================================//
				if(nb_tour>=VITESSE_JEU_BOT){
					if(sel==0){ //SELECTION
						fprintf(stderr,"TOUR DU BOT %d : PTS D'ACTION = %d\n",joueur_actu,tab[joueur_actu].pts_action_actu);
						for (int i=0;i<N;i++){ //deselectionne toutes les pieces
							for (int j=0;j<M;j++){
								if(terrain[i][j].piece && terrain[i][j].piece->select==1){
										terrain[i][j].piece->select=0;
								}
							}
						}
						int var5=0;
						int var6=(rand()%tab[joueur_actu].nb_unite);
						for (int i=0;i<N;i++){ //selection d'une piece
							for (int j=0;j<M;j++){
								if(terrain[i][j].piece && terrain[i][j].piece->select==0 && terrain[i][j].piece->joueur==joueur_actu && var5==var6){
									terrain[i][j].piece->select=1;
									x_bot=i;
									y_bot=j;
								}
								if(terrain[i][j].piece && terrain[i][j].piece->joueur==joueur_actu)
									var5++;
							}
						}

						centrer_camera(terrain,terrain[x_bot][y_bot].xImg,terrain[x_bot][y_bot].yImg,*largeur,*hauteur);
						sel=1;
					}else{ //ACTION
						int nb_ennemies_portee=a_portee(terrain,x_bot,y_bot,joueur_actu);
						if((nb_ennemies_portee>=1 && terrain[x_bot][y_bot].piece->classe!=priest)|| (nb_ennemies_portee>=2 && terrain[x_bot][y_bot].piece->classe==priest) || (reste_allie(terrain,joueur_actu)==1 && terrain[x_bot][y_bot].piece->classe==priest)){
							fprintf(stderr,"ATTAQUE\n");
							attaquer_meilleur_cible(terrain,x_bot,y_bot,nb_ennemies_portee,joueur_actu,tab,aff_deg);
						}else{
							fprintf(stderr,"DEPLACEMENT\n");
							depla_atk_mov(terrain,x_bot,y_bot,joueur_actu,tab);
						}


						sel=0;
					}
					nb_tour=0;
					if(tab[joueur_actu].pts_action_actu<=0){ //direction blockage
						for (int i=0;i<N;i++){
							for (int j=0;j<M;j++){
								if(terrain[i][j].piece && terrain[i][j].piece->joueur==joueur_actu){
									IA_blockage_direction(terrain,i,j,joueur_actu);//tourne le perso selon la direction la plus intéressante
								}
							}
						}
					}

				}


			}

			for(int i=0;i<J;i++){
				if(tab[i].nb_unite<=0){
					tab[i].id_joueur=-1;
				}
			}


			nb_joueur_restant=0;
			for(int i=0;i<J;i++){		//fermeture de la fenetre qd il reste un joueur en vie seulement (a modifié pour retourner au menu qd le joueur gagne)
				if(tab[i].id_joueur!=-1){
					nb_joueur_restant++;
				}
			}

//=================================================================================================================================//
//=================================================================================================================================//
//====================================================AFFICHAGE====================================================================//
//=================================================================================================================================//
//=================================================================================================================================//


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



			AfficherText("Joueurs Restants :        ","arial.ttf",c,20,renderer,185,25);
			sprintf(variable, "%d",  nb_joueur_restant);
			AfficherText(variable,"arial.ttf",c,30,renderer,350,22);

			AfficherText("Allie Restant :        ","arial.ttf",c,20,renderer,185,75);
			sprintf(variable, "%d",  reste_allie(terrain,joueur_actu));
			AfficherText(variable,"arial.ttf",c,30,renderer,350,72);

			AfficherText("Ennemis Restants :        ","arial.ttf",c,20,renderer,185,125);
			sprintf(variable, "%d",  reste_ennemi(terrain,joueur_actu));
			AfficherText(variable,"arial.ttf",c,30,renderer,350,122);




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
							sprintf(variable, "%d", (terrain[i][j].piece->armure+3));
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


					}
				}
			}

			for(int i=0;i<AFF_DEG;i++){ //affichage des dégats, morts soins etc
				if(aff_deg[i].time>0){
					if(aff_deg[i].c==1){//couleur degat
						AfficherText(aff_deg[i].txt,"arial.ttf",c_rouge,30,renderer,aff_deg[i].pos_x,aff_deg[i].pos_y);
					}else if(aff_deg[i].c==2){//couleur soin
						AfficherText(aff_deg[i].txt,"arial.ttf",c_verte,30,renderer,aff_deg[i].pos_x,aff_deg[i].pos_y);
					}else{//couleur noire
						AfficherText(aff_deg[i].txt,"arial.ttf",c,30,renderer,aff_deg[i].pos_x,aff_deg[i].pos_y);
					}
				}
			}

			clean_degat_txt(aff_deg);



			SDL_RenderPresent(renderer);
			SDL_Delay(16);



//=================================================================================================================================//
//=======================================================FIN=======================================================================//
//====================================================AFFICHAGE====================================================================//
//=======================================================...=======================================================================//
//=================================================================================================================================//




		//===================================ACTUALISATION DES TOURS==========================================//

			if(nb_joueur_restant==1){
				for(int i=0;i<J;i++){
					if(tab[i].id_joueur!=-1)
							fprintf(stderr,"LE JOUEUR %d GAGNE\n",i);
				}
				//ecran de victoire/defaite

				running=Quit;
			}

			if((tab[joueur_actu].pts_action_actu<=0 && tab[joueur_actu].humain==0 )|| (tab[joueur_actu].pts_action_actu<=0 && fin_tour==1) || (fin_tour==1)){				//gestion des tours de jeu
				tab[joueur_actu].pts_action_actu=tab[joueur_actu].pts_action_max;
				do{
					joueur_actu=(joueur_actu+1)%J;
				}while(tab[joueur_actu].id_joueur==-1);
				fin_tour=0;
			}






			//incrémentation a chaque tours
			nb_tour=(nb_tour+1)%(VITESSE_JEU_BOT*2);
			compteur_anim=compteur_anim%(VITESSE_ANIM*2);
			compteur_anim++;


		}//fin boucle de jeu
		}

	}
	else {
		fprintf(stderr,"Erreur de création de la fenêtre: %s\n",SDL_GetError());
	}

	//Destruction de la fenetre
	SDL_DestroyWindow(pWindow);

//	TTF_CloseFont(police); /* Doit être avant TTF_Quit() */
	TTF_Quit();
  SDL_Quit();
	return 0;
}
