#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>



#include "grille.h"
#include "piece.h"
#include "interface.h"
#include "logs.h"
#include "bot.h"
#include "camera.h"
#include "deplacement.h"
#include "combat.h"


//=NB JOUEURS,JOUEUR TOTAUX=====//
int J= 2 ; //nb de joueur total
int J_HUMAIN= 1 ;//nb de joueur humain parmis les joueurs totales
int J_BOT= 1 ;//nb de joueur humain parmis les joueurs totales
//==============================//

//==UNITEES=====================//
int NB_CLASSE= 6 ; //nb de classe actuelement dans le jeu !!!!!A ne pas modifier!!!!!!
int NB_UNITE= 7 ; //nb unité pour chaque joueurs au debut de la partie
int ESPACE_GEN= 2; //espace (nb de case de chaque coté au minimum) entre deux équipes d'unitée
int NB_MAX_PRIEST=2;
int NB_BLOCK=18;
int PDV_BLOCK=100;
int NB_GOLD=50;
int NB_BOIS=25;
//==============================//

//===========VITESSE DU JEU=====//
int PTS_ACTION_MAX= 0 ; //pts d'action max pour chaque tours de chaque joueur
int PTS_ACTION_UNI=2; //pts action max pour chaque unité
int VITESSE_JEU_BOT= 1 ; //nb de boucle d'affichage entre chaque action d'un bot (vitesse max=1)
int VITESSE_ANIM=15;
//==============================//

//========AFFICHAGE/GRILLE======//
int AFF_DEG= 10 ; //nombre d'affichage max a la fois par boucle d'affichage d'info texte de dégats, morts et soins
int TAILLE_TAB_BASH=1000; //taille max de ligne de bash sauvegardé
int BASH_SCROLL_SPEED=2;
int DURE_JOUR_NUIT=4000;
int PRESET=1 ; //1 pour generation alea, autre pour preset de carte via fichier
int PRESET_U=1; //1 pour gene aleatoire et autre pour preset d'unité

int N= 30 ; //taille de la grille (ne peux pas eccéder 200x200 actuelement (mettre en place des fichier ou enregistrer et reouvrir pour chargement dynamique de la map et grandeur infini))
int M= 30 ;
//==============================//
int ISO=1;
int VITESSE_INCREMENTATION=10;
float COEF_AFFICHAGE=0;
int TOUR_EVOL_FORET=1;
int CHANCE_EVOL=100;

int DEGAT_EBOULEMENT=30;

//=====================================MAIN=============================//

int WinMain(int argc, char** argv)
{


	SDL_Window* pWindow = NULL;
	SDL_Renderer *renderer=NULL;

	int *largeur=0,*hauteur=0;
	largeur=malloc(sizeof(*largeur));
	hauteur=malloc(sizeof(*hauteur));

	int bordure=43;

	int sel=0;
	int fin_tour=0;


	bash_t tab_info_bash[TAILLE_TAB_BASH];
	int nb_tour=0;
	int compteur_anim=VITESSE_ANIM;
	int scroll_speed=30;
	int x_bot,y_bot;
	srand(time(NULL));
	int souris=0;
	int nb_joueur_restant;
	int joueur_start=0;
	int joueur_actu=joueur_start;
	int frame_anim_montre=0;
	int compteur_tour=1;
	int clicout;//sert a ne pas deselectionner la piece actuel dans la gestion des clics
	long int jour_nuit;
	image_t image[Z];
	char variable[80];
	char variable2[80];
	int compteur_bouton_cam=0;
	int wait=0;
	int bloc_actu=1;
	int compteur_erreur=0;







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
	float test=(*largeur);
	COEF_AFFICHAGE=test / 1920.00;

SDL_SetWindowFullscreen(pWindow,SDL_WINDOW_FULLSCREEN_DESKTOP);
//SDL_WINDOW_FULLSCREEN


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


//========================================FENETRE=====================================//

if( pWindow )
{
	running = Menu;
	SDL_Event e;
	while(running != Quit) {
		//================================================================================================================================================//
		//================================================================================================================================================//
		//=============================================================MENU PRINCIPALE====================================================================//
		//================================================================================================================================================//
		//================================================================================================================================================//
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
		}else if(running==option){
			int delay_clic=0;
			while(SDL_PollEvent(&e) || running == option){
				mod_option(renderer,pWindow,image);
				switch(e.type) {
					case SDL_QUIT:	//cas ou l'on souhaite quitter
						running = Quit;
					break;
					case SDL_MOUSEBUTTONDOWN:
						if(e.button.button == SDL_BUTTON_LEFT){
							gpScreen = SDL_GetWindowSurface(pWindow);
							if(delay_clic>=VITESSE_INCREMENTATION){
								delay_clic=0;
								if(e.motion.x>((gpScreen->w/100 * 14)+400*COEF_AFFICHAGE) && e.motion.x<((gpScreen->w/100 * 14)+460*COEF_AFFICHAGE) && e.motion.y>(gpScreen->h/100 * 18) && e.motion.y<((gpScreen->h/100 * 18)+60*COEF_AFFICHAGE) && ESPACE_GEN<N/(2*J) && ESPACE_GEN<M/(2*J)){
									ESPACE_GEN++;
								}
								if(e.motion.x>((gpScreen->w/100 * 14)+470*COEF_AFFICHAGE) && e.motion.x<((gpScreen->w/100 * 14)+520*COEF_AFFICHAGE) && e.motion.y>(gpScreen->h/100 * 18) && e.motion.y<((gpScreen->h/100 * 18)+60*COEF_AFFICHAGE) && ESPACE_GEN>0){
									ESPACE_GEN--;
								}
								if(e.motion.x>((gpScreen->w/100 * 14)+400*COEF_AFFICHAGE) && e.motion.x<((gpScreen->w/100 * 14)+460*COEF_AFFICHAGE) && e.motion.y>(gpScreen->h/100 * 34) && e.motion.y<((gpScreen->h/100 * 34)+60*COEF_AFFICHAGE) && TAILLE_TAB_BASH<10000){
									TAILLE_TAB_BASH+=10;
								}
								if(e.motion.x>((gpScreen->w/100 * 14)+470*COEF_AFFICHAGE) && e.motion.x<((gpScreen->w/100 * 14)+520*COEF_AFFICHAGE) && e.motion.y>(gpScreen->h/100 * 34) && e.motion.y<((gpScreen->h/100 * 34)+60*COEF_AFFICHAGE) && TAILLE_TAB_BASH>30){
									TAILLE_TAB_BASH-=10;
								}
								if(e.motion.x>((gpScreen->w/100 * 14)+400*COEF_AFFICHAGE) && e.motion.x<((gpScreen->w/100 * 14)+460*COEF_AFFICHAGE) && e.motion.y>(gpScreen->h/100 * 50) && e.motion.y<((gpScreen->h/100 * 50)+60*COEF_AFFICHAGE)){
									ISO=1;
								}
								if(e.motion.x>((gpScreen->w/100 * 14)+470*COEF_AFFICHAGE) && e.motion.x<((gpScreen->w/100 * 14)+520*COEF_AFFICHAGE) && e.motion.y>(gpScreen->h/100 * 50) && e.motion.y<((gpScreen->h/100 * 50)+60*COEF_AFFICHAGE)){
									ISO=0;
								}
								if(e.motion.x>((gpScreen->w/100 * 58)+400*COEF_AFFICHAGE) && e.motion.x<((gpScreen->w/100 * 58)+460*COEF_AFFICHAGE) && e.motion.y>(gpScreen->h/100 * 18) && e.motion.y<((gpScreen->h/100 * 18)+60*COEF_AFFICHAGE) && VITESSE_JEU_BOT<100 ){
									VITESSE_JEU_BOT++;
								}
								if(e.motion.x>((gpScreen->w/100 * 58)+470*COEF_AFFICHAGE) && e.motion.x<((gpScreen->w/100 * 58)+520*COEF_AFFICHAGE) && e.motion.y>(gpScreen->h/100 * 18) && e.motion.y<((gpScreen->h/100 * 18)+60*COEF_AFFICHAGE) && VITESSE_JEU_BOT>1 ){
									VITESSE_JEU_BOT--;
								}
								if(e.motion.x>((gpScreen->w/100 * 58)+400*COEF_AFFICHAGE) && e.motion.x<((gpScreen->w/100 * 58)+460*COEF_AFFICHAGE) && e.motion.y>(gpScreen->h/100 * 34) && e.motion.y<((gpScreen->h/100 * 34)+60*COEF_AFFICHAGE) && BASH_SCROLL_SPEED<15 ){
									BASH_SCROLL_SPEED++;
								}
								if(e.motion.x>((gpScreen->w/100 * 58)+470*COEF_AFFICHAGE) && e.motion.x<((gpScreen->w/100 * 58)+520*COEF_AFFICHAGE) && e.motion.y>(gpScreen->h/100 * 34) && e.motion.y<((gpScreen->h/100 * 34)+60*COEF_AFFICHAGE) && BASH_SCROLL_SPEED>0 ){
									BASH_SCROLL_SPEED--;
								}
								if(e.motion.x>((gpScreen->w/100 * 58)+400*COEF_AFFICHAGE) && e.motion.x<((gpScreen->w/100 * 58)+460*COEF_AFFICHAGE) && e.motion.y>(gpScreen->h/100 * 50) && e.motion.y<((gpScreen->h/100 * 50)+60*COEF_AFFICHAGE) && VITESSE_INCREMENTATION<20 ){
									VITESSE_INCREMENTATION++;
								}
								if(e.motion.x>((gpScreen->w/100 * 58)+470*COEF_AFFICHAGE) && e.motion.x<((gpScreen->w/100 * 58)+520*COEF_AFFICHAGE) && e.motion.y>(gpScreen->h/100 * 50) && e.motion.y<((gpScreen->h/100 * 50)+60*COEF_AFFICHAGE) && VITESSE_INCREMENTATION>2 ){
									VITESSE_INCREMENTATION--;
								}
								if(e.motion.x>((gpScreen->w/100 * 64)) && e.motion.x<((gpScreen->w/100 * 64)+320*COEF_AFFICHAGE) && e.motion.y>(gpScreen->h/100 * 66) && e.motion.y<((gpScreen->h/100 * 66)+100*COEF_AFFICHAGE)){
									running=Menu2;
									wait=20;
								}

							}
							delay_clic=(delay_clic+1)%51;

						}
				}
			}
			//================================================================================================================================================//
			//================================================================================================================================================//
			//=============================================================CHOIX DES PROPRIÉTÉ PARTIE=========================================================//
			//===================================================================ET===========================================================================//
			//=================================================================DEBUT DE PARTIE================================================================//
		}else if(running == Menu2 || running==Play_load){
			int delay_clic=0;
			while(SDL_PollEvent(&e)||running == Menu2 || running==Play_load) {
				mod_menu(renderer,pWindow,image);
				switch(e.type) {
					case SDL_QUIT:	//cas ou l'on souhaite quitter
						running = Quit;
					break;
					case SDL_MOUSEBUTTONDOWN:
						if(e.button.button == SDL_BUTTON_LEFT){
							gpScreen = SDL_GetWindowSurface(pWindow);
							if(delay_clic>=VITESSE_INCREMENTATION){
								delay_clic=0;
								if(e.motion.x>((gpScreen->w/100 * 14)+400*COEF_AFFICHAGE) && e.motion.x<((gpScreen->w/100 * 14)+460*COEF_AFFICHAGE) && e.motion.y>(gpScreen->h/100 * 18) && e.motion.y<((gpScreen->h/100 * 18)+60*COEF_AFFICHAGE) && J<50 && J_HUMAIN<50){
									J_HUMAIN++;
								}
								if(e.motion.x>((gpScreen->w/100 * 14)+470*COEF_AFFICHAGE) && e.motion.x<((gpScreen->w/100 * 14)+520*COEF_AFFICHAGE) && e.motion.y>(gpScreen->h/100 * 18) && e.motion.y<((gpScreen->h/100 * 18)+60*COEF_AFFICHAGE) && J>2 && J_HUMAIN>0){
									J_HUMAIN--;
								}
								if(e.motion.x>((gpScreen->w/100 * 14)+400*COEF_AFFICHAGE) && e.motion.x<((gpScreen->w/100 * 14)+460*COEF_AFFICHAGE) && e.motion.y>(gpScreen->h/100 * 34) && e.motion.y<((gpScreen->h/100 * 34)+60*COEF_AFFICHAGE) && J<50 && J_BOT<50){
									J_BOT++;
								}
								if(e.motion.x>((gpScreen->w/100 * 14)+470*COEF_AFFICHAGE) && e.motion.x<((gpScreen->w/100 * 14)+520*COEF_AFFICHAGE) && e.motion.y>(gpScreen->h/100 * 34) && e.motion.y<((gpScreen->h/100 * 34)+60*COEF_AFFICHAGE) && J>2 && J_BOT>0){
									J_BOT--;
								}
								if(e.motion.x>((gpScreen->w/100 * 14)+400*COEF_AFFICHAGE) && e.motion.x<((gpScreen->w/100 * 14)+460*COEF_AFFICHAGE) && e.motion.y>(gpScreen->h/100 * 50) && e.motion.y<((gpScreen->h/100 * 50)+60*COEF_AFFICHAGE) && NB_UNITE<30 ){
									NB_UNITE++;
								}
								if(e.motion.x>((gpScreen->w/100 * 14)+470*COEF_AFFICHAGE) && e.motion.x<((gpScreen->w/100 * 14)+520*COEF_AFFICHAGE) && e.motion.y>(gpScreen->h/100 * 50) && e.motion.y<((gpScreen->h/100 * 50)+60*COEF_AFFICHAGE) && NB_UNITE>1 ){
									NB_UNITE--;
								}
								if(e.motion.x>((gpScreen->w/100 * 14)+400*COEF_AFFICHAGE) && e.motion.x<((gpScreen->w/100 * 14)+460*COEF_AFFICHAGE) && e.motion.y>(gpScreen->h/100 * 66) && e.motion.y<((gpScreen->h/100 * 66)+60*COEF_AFFICHAGE) && M<100 ){
									M++;
								}
								if(e.motion.x>((gpScreen->w/100 * 14)+470*COEF_AFFICHAGE) && e.motion.x<((gpScreen->w/100 * 14)+520*COEF_AFFICHAGE) && e.motion.y>(gpScreen->h/100 * 66) && e.motion.y<((gpScreen->h/100 * 66)+60*COEF_AFFICHAGE) && M>10 ){
									M--;
								}
								if(e.motion.x>((gpScreen->w/100 * 14)+400*COEF_AFFICHAGE) && e.motion.x<((gpScreen->w/100 * 14)+460*COEF_AFFICHAGE) && e.motion.y>(gpScreen->h/100 * 79) && e.motion.y<((gpScreen->h/100 * 79)+60*COEF_AFFICHAGE) && N<100 ){
									N++;
								}
								if(e.motion.x>((gpScreen->w/100 * 14)+470*COEF_AFFICHAGE) && e.motion.x<((gpScreen->w/100 * 14)+520*COEF_AFFICHAGE) && e.motion.y>(gpScreen->h/100 * 79) && e.motion.y<((gpScreen->h/100 * 79)+60*COEF_AFFICHAGE) && N>10 ){
									N--;
								}
								if(e.motion.x>((gpScreen->w/100 * 58)+400*COEF_AFFICHAGE) && e.motion.x<((gpScreen->w/100 * 58)+460*COEF_AFFICHAGE) && e.motion.y>(gpScreen->h/100 * 18) && e.motion.y<((gpScreen->h/100 * 18)+60*COEF_AFFICHAGE) && PTS_ACTION_UNI<15 ){
									PTS_ACTION_UNI++;
								}
								if(e.motion.x>((gpScreen->w/100 * 58)+470*COEF_AFFICHAGE) && e.motion.x<((gpScreen->w/100 * 58)+520*COEF_AFFICHAGE)&& e.motion.y>(gpScreen->h/100 * 18) && e.motion.y<((gpScreen->h/100 * 18)+60*COEF_AFFICHAGE) && PTS_ACTION_UNI>1 ){
									PTS_ACTION_UNI--;
								}
								if(e.motion.x>((gpScreen->w/100 * 58)+400*COEF_AFFICHAGE) && e.motion.x<((gpScreen->w/100 * 58)+460*COEF_AFFICHAGE) && e.motion.y>(gpScreen->h/100 * 34) && e.motion.y<((gpScreen->h/100 * 34)+60*COEF_AFFICHAGE) && NB_MAX_PRIEST<30 ){
									NB_MAX_PRIEST++;
								}
								if(e.motion.x>((gpScreen->w/100 * 58)+470*COEF_AFFICHAGE) && e.motion.x<((gpScreen->w/100 * 58)+520*COEF_AFFICHAGE) && e.motion.y>(gpScreen->h/100 * 34) && e.motion.y<((gpScreen->h/100 * 34)+60*COEF_AFFICHAGE) && NB_MAX_PRIEST>0 ){
									NB_MAX_PRIEST--;
								}
								if(e.motion.x>((gpScreen->w/100 * 58)+400*COEF_AFFICHAGE) && e.motion.x<((gpScreen->w/100 * 58)+460*COEF_AFFICHAGE) && e.motion.y>(gpScreen->h/100 * 50) && e.motion.y<((gpScreen->h/100 * 50)+60*COEF_AFFICHAGE) && DURE_JOUR_NUIT<15000 ){
									DURE_JOUR_NUIT+=100;
								}
								if(e.motion.x>((gpScreen->w/100 * 58)+470*COEF_AFFICHAGE) && e.motion.x<((gpScreen->w/100 * 58)+520*COEF_AFFICHAGE) && e.motion.y>(gpScreen->h/100 * 50) && e.motion.y<((gpScreen->h/100 * 50)+60*COEF_AFFICHAGE) && DURE_JOUR_NUIT>1000 ){
									DURE_JOUR_NUIT-=100;
								}


								if(e.motion.x>((gpScreen->w/100 * 50)) && e.motion.x<((gpScreen->w/100 * 50)+150*COEF_AFFICHAGE) && e.motion.y>(gpScreen->h/100 * 71) && e.motion.y<((gpScreen->h/100 * 71)+150*COEF_AFFICHAGE)){
									running=option;
								}






							}
							delay_clic=(delay_clic+1)%(VITESSE_INCREMENTATION*5);
							J=J_HUMAIN+J_BOT;
							wait=(wait-1)%(-200);

							if(e.motion.x>((gpScreen->w/100 * 64)) && e.motion.x<((gpScreen->w/100 * 64)+320) && e.motion.y>(gpScreen->h/100 * 66) && e.motion.y<((gpScreen->h/100 * 66)+100) && wait<=0){
								running=Menu;
							}

							if(e.motion.x>((gpScreen->w/100 * 66))&& e.motion.x<((gpScreen->w/100 * 66)+300) && e.motion.y>(gpScreen->h/100 * 79) && e.motion.y<((gpScreen->h/100 * 79)+100)){
								running=Menu3;
							}

					}
					break;
				}
			}
		}else if(running == Menu3){
			joueur_unite_t joueur_unite[J];
			init_joueur_unit(joueur_unite,J,NB_UNITE);
			fprintf(stderr, "%d %d\n", J,NB_UNITE);
			int id = 0;
			int delay_clic=0;
			while(SDL_PollEvent(&e)||running == Menu3) {
				gpScreen = SDL_GetWindowSurface(pWindow);
				int longeur = gpScreen->h/5;
				int fonsize = longeur/3.5;
				int y3 = fonsize/2;
				int x3 = gpScreen->w/10;
				if(running == Menu3)unit_menu(renderer,pWindow,joueur_unite[id]);
				switch(e.type) {
					case SDL_QUIT:	//cas ou l'on souhaite quitter
						running = Quit;
					break;
					case SDL_MOUSEBUTTONDOWN:
							if(e.button.button == SDL_BUTTON_LEFT){
								if(delay_clic%10==0){
								if(e.button.y>y3&&e.button.y<y3+fonsize){
									if(e.button.x>x3*3&&e.button.x<x3*3+fonsize&&id>0)id--;
									if(e.button.x>x3*6&&e.button.x<x3*6+fonsize&&id<J-1)id++;
								}
								if(e.button.y>y3*14&&e.button.y<y3*14+fonsize){
									if(e.button.x>x3&&e.button.x<x3+fonsize&&joueur_unite[id].unit[0]>0){
										joueur_unite[id].unit[0]--;
										joueur_unite[id].restant_unite++;
									}
									if(e.button.x>x3+fonsize*3&&e.button.x<x3+fonsize*4&&joueur_unite[id].restant_unite>0){
										joueur_unite[id].unit[0]++;
										joueur_unite[id].restant_unite--;
									}
									if(e.button.x>x3*4&&e.button.x<x3*4+fonsize&&joueur_unite[id].unit[1]>0){
										joueur_unite[id].unit[1]--;
										joueur_unite[id].restant_unite++;
									}
									if(e.button.x>x3*4+fonsize*3&&e.button.x<x3*4+fonsize*4&&joueur_unite[id].restant_unite>0){
										joueur_unite[id].unit[1]++;
										joueur_unite[id].restant_unite--;
									}
									if(e.button.x>x3*7&&e.button.x<x3*7+fonsize&&joueur_unite[id].unit[2]>0){
										joueur_unite[id].unit[2]--;
										joueur_unite[id].restant_unite++;
									}
									if(e.button.x>x3*7+fonsize*3&&e.button.x<x3*7+fonsize*4&&joueur_unite[id].restant_unite>0){
										joueur_unite[id].unit[2]++;
										joueur_unite[id].restant_unite--;
									}
								}
								if(e.button.y>y3*25&&e.button.y<y3*25+fonsize){
									if(e.button.x>x3&&e.button.x<x3+fonsize&&joueur_unite[id].unit[3]>0){
										joueur_unite[id].unit[3]--;
										joueur_unite[id].restant_unite++;
									}
									if(e.button.x>x3+fonsize*3&&e.button.x<x3+fonsize*4&&joueur_unite[id].restant_unite>0){
										joueur_unite[id].unit[3]++;
										joueur_unite[id].restant_unite--;
									}
									if(e.button.x>x3*4&&e.button.x<x3*4+fonsize&&joueur_unite[id].unit[4]>0){
										joueur_unite[id].unit[4]--;
										joueur_unite[id].restant_unite++;
									}
									if(e.button.x>x3*4+fonsize*3&&e.button.x<x3*4+fonsize*4&&joueur_unite[id].restant_unite>0){
										joueur_unite[id].unit[4]++;
										joueur_unite[id].restant_unite--;
									}
									if(e.button.x>x3*7&&e.button.x<x3*7+fonsize&&joueur_unite[id].unit[5]>0){
										joueur_unite[id].unit[5]--;
										joueur_unite[id].restant_unite++;
									}
									if(e.button.x>x3*7+fonsize*3&&e.button.x<x3*7+fonsize*4&&joueur_unite[id].restant_unite>0){
										joueur_unite[id].unit[5]++;
										joueur_unite[id].restant_unite--;
									}
								}
								delay_clic=0;
							}
						delay_clic=(delay_clic+1)%51;
						if(e.button.y>y3*30&&e.button.x < x3*4)running = Menu2;
						if(e.button.y>y3*30&&e.button.x > x3*7){

								//==================================INITIALISATION GRILLE============================//
								    //choix du mode de génération
								//	printf("1 POUR CARTE ALEATOIRE, AUTRE CHIFFRE POUR UNE DES CARTES PRESET : ");
								//	scanf("%d",&preset);


								degatx_t aff_deg[AFF_DEG];
								joueurs_t tab[J];
								case_t terrain[N][M];
								page_t page_actu=rien;

								frame_anim_montre=4;
								jour_nuit=((DURE_JOUR_NUIT/24)*7);
								joueur_start=rand()%J;
								joueur_actu=joueur_start;
								compteur_tour=1;
								bloc_actu=1;
								int fin_partie=0;


								if(ISO==1){
									initialisation_principale_iso(bordure,pWindow,largeur,hauteur,tab,aff_deg,tab_info_bash,terrain,variable2,joueur_unite);
								}else{
									initialisation_principale(bordure,pWindow,largeur,hauteur,tab,aff_deg,tab_info_bash,terrain,variable2);
								}

								running = Play;
								while(running == Play){ //boucle de jeu
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
													case SDLK_y:
															camera_sur_allie(terrain,joueur_actu,tab,largeur,hauteur);
															compteur_bouton_cam=VITESSE_ANIM;
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
																				if(e.button.y>gpScreen->h - 330+75 && e.button.y<gpScreen->h - 330+75*2){
																					//==========================//sauvegarder//=============================================//
																					save(terrain,compteur_tour,joueur_actu,tab);
																					running = Menu;
																				}
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
														case SDL_BUTTON_RIGHT:
															for(int indice=0;indice<N;indice++){
																for(int indice2=0;indice2<M; indice2++){
																	terrain[indice][indice2].deplacement=0;
																	terrain[indice][indice2].attaque=0;
																	terrain[indice][indice2].block=0;
																	if(terrain[indice][indice2].bloc && terrain[indice][indice2].bloc->block_sel==1)
																		terrain[indice][indice2].bloc->block_sel=0;
																	if(terrain[indice][indice2].piece && terrain[indice][indice2].piece->select==1)
																		terrain[indice][indice2].piece->select=0;
																}
															}
														break;

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
															if(page_actu==rien){
																for(int compteur=0;compteur<N;compteur++){
																	for(int compteur2=0;compteur2<M; compteur2++){
																			/* VERSION AVANCER NE FONCTIONNANT PAS ENCORE*/
																			float vert=abs((terrain[compteur][compteur2].y3 - terrain[compteur][compteur2].y1))/2;
																			float hori=abs((terrain[compteur][compteur2].x4 - terrain[compteur][compteur2].x2))/2;
																			float y_mid=terrain[compteur][compteur2].y2;
																			float x_mid=terrain[compteur][compteur2].x1;

																			int calc_vert1=(((y_mid-e.motion.y)/vert)*100);
																			int calc_hori1=(((e.motion.x-x_mid)/hori)*100);

																			int calc_vert2=(((e.motion.y-y_mid)/vert)*100);
																			int calc_hori2=(((e.motion.x-x_mid)/hori)*100);

																			int calc_vert3=(((e.motion.y-y_mid)/vert)*100);
																			int calc_hori3=(((x_mid-e.motion.x)/hori)*100);

																			int calc_vert4=(((y_mid-e.motion.y)/vert)*100);
																			int calc_hori4=(((x_mid-e.motion.x)/hori)*100);





																			//VERSION BASIQUE QUI FONCTIONNE MAL
																			//if(e.motion.y > terrain[compteur][compteur2].y1+20 && e.motion.y < terrain[compteur][compteur2].y3-20 && e.motion.x > terrain[compteur][compteur2].x4+20 && e.motion.x < terrain[compteur][compteur2].x2-20 &&!(e.motion.x>(*largeur)/2+175 && e.motion.x<(*largeur)/2+175+150 && e.motion.y>15 && e.motion.y<15+50)&&!(e.motion.x>10 && e.motion.x<240 && e.motion.y>300 && e.motion.y<500)){
																			if( (calc_vert1 + calc_hori1 )<=100 && (calc_vert2 + calc_hori2 )<=100 && (calc_vert3 + calc_hori3 )<=100 && (calc_vert4 + calc_hori4 )<=100 ){

																			/*fprintf(stderr,"\n=========TEST============\n");
																			fprintf(stderr,"y1 = %d\n",terrain[compteur][compteur2].y1);
																			fprintf(stderr,"y3 = %d\n",terrain[compteur][compteur2].y3);
																			fprintf(stderr,"x4 = %d\n",terrain[compteur][compteur2].x4);
																			fprintf(stderr,"x2 = %d\n",terrain[compteur][compteur2].x2);



																			fprintf(stderr,"\ncalc_vert1 = %d\n",calc_vert1);
																			fprintf(stderr,"calc_hori1 = %d\n",calc_hori1);

																			fprintf(stderr,"\nmotion x = %d\n",e.motion.x);
																			fprintf(stderr,"motion y = %d\n",e.motion.y);*/

																				//=================SELECTION D'UNE PIECE====================//
																				if(terrain[compteur][compteur2].piece || (terrain[compteur][compteur2].bloc!=NULL && terrain[compteur][compteur2].bloc->block_allie!=joueur_actu && terrain[compteur][compteur2].attaque==1)){
																					clicout=1;
																					for(int indice=0;indice<N;indice++){
																						for(int indice2=0;indice2<M; indice2++){
																							if(terrain[indice][indice2].bloc && terrain[indice][indice2].bloc->block_sel==1)
																								terrain[indice][indice2].bloc->block_sel=0;
																						}
																					}

																					for(int x=0;x<N;x++){
																						for(int y=0;y<M;y++){
																							if(terrain[x][y].piece && terrain[x][y].piece->joueur==joueur_actu && terrain[x][y].piece->select==1 && fin_partie==0){
																								combat(terrain,x,y,compteur,compteur2,joueur_actu,tab,aff_deg,tab_info_bash,variable2);
																							}
																						}
																					}
																					if(terrain[compteur][compteur2].piece && terrain[compteur][compteur2].piece->select!=1){
																						for(int indice=0;indice<N;indice++){
																							for(int indice2=0;indice2<M; indice2++){
																								if(terrain[indice][indice2].piece && terrain[indice][indice2].piece->select==1){
																									terrain[indice][indice2].piece->select=0;
																								}
																							}
																						}

																						if(terrain[compteur][compteur2].piece){
																							terrain[compteur][compteur2].piece->select=1;
																						}
																					}else if(terrain[compteur][compteur2].bloc){
																						for(int indice=0;indice<N;indice++){
																							for(int indice2=0;indice2<M; indice2++){
																								if(terrain[indice][indice2].piece && terrain[indice][indice2].piece->select==1){
																									terrain[indice][indice2].piece->select=0;
																								}
																							}
																						}
																						terrain[compteur][compteur2].bloc->block_sel=1;
																					}


																				}else if(terrain[compteur][compteur2].piece==NULL && (terrain[compteur][compteur2].bloc==NULL || (terrain[compteur][compteur2].bloc!=NULL && terrain[compteur][compteur2].bloc->block_allie==joueur_actu && terrain[compteur][compteur2].deplacement==1)) && fin_partie==0){
																				//=================DEPLACEMENT et ATTAQUE D'UNE PIECE====================//

																				move(terrain,compteur,compteur2,joueur_actu,tab,tab_info_bash,variable2);
																				poser_block(terrain,compteur,compteur2,joueur_actu,tab,tab_info_bash,variable2,bloc_actu);

																				}else if(terrain[compteur][compteur2].piece==NULL && terrain[compteur][compteur2].bloc!=NULL){
																					clicout=1;
																					for(int indice=0;indice<N;indice++){
																						for(int indice2=0;indice2<M; indice2++){
																							if(terrain[indice][indice2].bloc && terrain[indice][indice2].bloc->block_sel==1)
																								terrain[indice][indice2].bloc->block_sel=0;
																							if(terrain[indice][indice2].piece && terrain[indice][indice2].piece->select==1)
																								terrain[indice][indice2].piece->select=0;
																						}
																					}
																					terrain[compteur][compteur2].bloc->block_sel=1;
																				}

																			}
																			else if(e.motion.x>(*largeur)/3+420 && e.motion.x<(*largeur)/2+175+150 && e.motion.y>15 && e.motion.y<15+50){
																				fin_tour=1;
																			}
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
																if(e.motion.x>10 && e.motion.x<110 && e.motion.y>420 && e.motion.y<520){
																	camera_sur_allie(terrain,joueur_actu,tab,largeur,hauteur);
																	compteur_bouton_cam=VITESSE_ANIM;
																}


																//=================MENU SUD============================//
																if(page_actu==rien){ //acces aux sous menus

																	if(e.motion.x>(*largeur/4 *COEF_AFFICHAGE) && e.motion.x<(*largeur/4 *COEF_AFFICHAGE)+75 && e.motion.y>(*hauteur-60) && e.motion.y<(*hauteur-10)){
																		page_actu=craft;
																	}else if(e.motion.x>(*largeur/4 *COEF_AFFICHAGE)+75 && e.motion.x<(*largeur/4 *COEF_AFFICHAGE)+150 && e.motion.y>(*hauteur-60) && e.motion.y<(*hauteur-10)){
																		page_actu=recherche;
																	}

																}else if(page_actu==craft){ //action de la page de craft
																				//quitter
																				if(e.motion.x>(*largeur/6 *COEF_AFFICHAGE)+940 && e.motion.x<(*largeur/6 *COEF_AFFICHAGE)+1000 && e.motion.y>300 && e.motion.y<360){
																					page_actu=rien;
																					compteur_erreur=0;
																				}
																				//choix du batiment
																				if(e.motion.x>(*largeur/6 *COEF_AFFICHAGE)+120 && e.motion.x<(*largeur/6 *COEF_AFFICHAGE)+230 && e.motion.y>630 && e.motion.y<730 && tab[joueur_actu].nb_block>0){
																					clicout=1;
																					bloc_actu=1;
																					if(pathfinding_block(terrain,joueur_actu,bloc_actu)){
																						page_actu=rien;
																						compteur_erreur=0;
																					}
																					else{
																						compteur_erreur=40;
																					}
																				}else if(e.motion.x>(*largeur/6 *COEF_AFFICHAGE)+235 && e.motion.x<(*largeur/6 *COEF_AFFICHAGE)+345 && e.motion.y>630 && e.motion.y<730 && tab[joueur_actu].nb_block>8){
																					clicout=1;
																					bloc_actu=2;
																					if(pathfinding_block(terrain,joueur_actu,bloc_actu)){
																						page_actu=rien;
																						compteur_erreur=0;
																					}
																					else{
																						compteur_erreur=40;
																					}
																				}
																			

																}else if(page_actu==recherche){ //action de la page de recherche
																				//quitter
																				if(e.motion.x>(*largeur/6 *COEF_AFFICHAGE)+940 && e.motion.x<(*largeur/6 *COEF_AFFICHAGE)+1000 && e.motion.y>300 && e.motion.y<360){
																					page_actu=rien;
																				}
																}
																//===============FIN MENU SUD==========================//
																if(clicout==0){
																	for(int indice=0;indice<N;indice++){
																		for(int indice2=0;indice2<M; indice2++){
																			terrain[indice][indice2].deplacement=0;
																			terrain[indice][indice2].attaque=0;
																			terrain[indice][indice2].block=0;
																			if(terrain[indice][indice2].bloc && terrain[indice][indice2].bloc->block_sel==1)
																				terrain[indice][indice2].bloc->block_sel=0;
																			if(terrain[indice][indice2].piece && terrain[indice][indice2].piece->select==1)
																				terrain[indice][indice2].piece->select=0;
																		}

																	}
																}
															}
														break;
													}
										break;
										case SDL_MOUSEWHEEL:  //SCROLLING BASH
											if(e.wheel.y==1){
												souris=BASH_SCROLL_SPEED;
											}else if (e.wheel.y==-1){
												souris=-BASH_SCROLL_SPEED;
											}
										break;


											}
										}
										if(tab[joueur_actu].humain==1){
									//if(e.motion.x>((*largeur)-450+25) && e.motion.x<(*largeur) && e.motion.y>0 && e.motion.y<300){
										if(souris>0 && tab_info_bash[TAILLE_TAB_BASH-1].pos_y<=0){
											for(int a=0;a<TAILLE_TAB_BASH;a++){
												//VERS LE HAUT
												tab_info_bash[a].pos_y+=25;
											}
											souris-=1;
										}
										else if (souris<0 && tab_info_bash[0].pos_y>265){
											for(int a=0;a<TAILLE_TAB_BASH;a++){
												//VERS LE BAS
												tab_info_bash[a].pos_y-=25;
											}
											souris+=1;
										}
									//	}


										gpScreen = SDL_GetWindowSurface(pWindow);
										if( e.motion.x >=0 && e.motion.x <=3 && e.type==SDL_MOUSEMOTION){
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
										if(e.motion.x >= gpScreen->w - 3 && e.motion.x <= gpScreen->w && e.type==SDL_MOUSEMOTION){
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
										if( e.motion.y >=0 && e.motion.y <=3 && e.type==SDL_MOUSEMOTION){
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
										if(e.motion.y >= gpScreen->h - 3 && e.motion.y <= gpScreen->h && e.type==SDL_MOUSEMOTION){
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
										}

									//fin action du joueur

									if(tab[joueur_actu].humain==0){ 	//===================================================TOUR DU BOT===================================================//
										if(nb_tour>=VITESSE_JEU_BOT){
											if(sel==0){ //SELECTION
												for (int i=0;i<N;i++){ //deselectionne toutes les pieces
													for (int j=0;j<M;j++){
														if(terrain[i][j].piece && terrain[i][j].piece->select==1){
																terrain[i][j].piece->select=0;
														}
													}
												}
												int ini=0;
												for (int i=0;i<N;i++){ //selection d'une piece
													for (int j=0;j<M;j++){
														if(terrain[i][j].piece && terrain[i][j].piece->select==0 && terrain[i][j].piece->joueur==joueur_actu && ini==0 && terrain[i][j].piece->pts_action_actu>0 && rand()%tab[joueur_actu].nb_unite<=tab[joueur_actu].nb_unite/2){
															terrain[i][j].piece->select=1;
															x_bot=i;
															y_bot=j;
															ini=1;
														}
														if(i==N-1 && j==M-1 && ini==0)
															i=0;
													}
												}

												centrer_camera(terrain,terrain[x_bot][y_bot].xImg,terrain[x_bot][y_bot].yImg,*largeur,*hauteur);
												sel=1;
											}else{ //ACTION
												int nb_ennemies_portee=a_portee(terrain,x_bot,y_bot,joueur_actu);
												if((nb_ennemies_portee>=1 && terrain[x_bot][y_bot].piece->classe!=priest)|| (nb_ennemies_portee>=2 && terrain[x_bot][y_bot].piece->classe==priest) || (reste_allie(terrain,joueur_actu)==1 && terrain[x_bot][y_bot].piece->classe==priest)){
													fprintf(stderr,"ATTAQUE\n");
													attaquer_meilleur_cible(terrain,x_bot,y_bot,nb_ennemies_portee,joueur_actu,tab,aff_deg,tab_info_bash,variable2);
												}else{
													fprintf(stderr,"DEPLACEMENT\n");
													depla_atk_mov(terrain,x_bot,y_bot,joueur_actu,tab,tab_info_bash,variable2);
												}
												/*if(action_possible(terrain,joueur_actu)==0){
													fin_tour=1;
												}*/

												sel=0;
											}
											nb_tour=0;
											if(tab[joueur_actu].pts_action_actu<=0 || fin_tour==1){ //direction blockage
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
										if(tab[i].nb_unite<=0 && tab[i].id_joueur!=-1){
											tab[i].id_joueur=-2;
										}
									}


									nb_joueur_restant=0;
									for(int i=0;i<J;i++){		//fermeture de la fenetre qd il reste un joueur en vie seulement (a modifié pour retourner au menu qd le joueur gagne)
										if(tab[i].id_joueur!=-1 && tab[i].id_joueur!=-2){
											nb_joueur_restant++;
										}
									}

									if((jour_nuit%((DURE_JOUR_NUIT/24)*3))>=-20 && (jour_nuit%((DURE_JOUR_NUIT/24)*3))<=20){
										jour_nuit+=40;
										frame_anim_montre++;
									}
									if(frame_anim_montre>=8 && (jour_nuit%((DURE_JOUR_NUIT/24)*3))>=20 && (jour_nuit%((DURE_JOUR_NUIT/24)*3))<=60){
										frame_anim_montre=0;
									}






									
									
									
									if(ISO==1){
										affichage_principale_iso(renderer,pWindow,bordure,largeur,hauteur,tab,aff_deg,tab_info_bash,terrain,joueur_actu,image,compteur_anim,nb_joueur_restant,compteur_tour,frame_anim_montre,jour_nuit,variable2,compteur_bouton_cam,bloc_actu,e,page_actu);
									}else{
										affichage_principale(renderer,pWindow,bordure,largeur,hauteur,tab,aff_deg,tab_info_bash,terrain,joueur_actu,image,compteur_anim,nb_joueur_restant,compteur_tour,frame_anim_montre,jour_nuit,variable2,compteur_bouton_cam);
									}
									af_erreur(renderer,pWindow,hauteur,largeur,&compteur_erreur,image);




									for(int i=0;i<J;i++){
										if(tab[i].id_joueur==-2){
											tab[i].id_joueur=-1;
										}
									}


									if(nb_joueur_restant==1 && fin_partie==0){
										for(int i=0;i<J;i++){
											if(tab[i].id_joueur!=-1 && tab[i].id_joueur!=-2){
													sprintf(variable,"LE JOUEUR %d GAGNE EN %d TOURS !",i,compteur_tour);
				    							ajouter_ligne_bash(variable,tab_info_bash,info,variable2);
													fin_partie=1;
													tab[i].humain=1;
											}
										}
										//ecran de victoire/defaite
									}

									if(fin_tour==1 && fin_partie==1){
											running=Menu;
									}


									if((tab[joueur_actu].pts_action_actu<=0 && tab[joueur_actu].humain==0 )|| (tab[joueur_actu].pts_action_actu<=0 && fin_tour==1) || (fin_tour==1)){				//gestion des tours de jeu
										tab[joueur_actu].pts_action_max=0;
										for(int i=0;i<N;i++){
											for(int j=0;j<M;j++){
												if(terrain[i][j].piece && terrain[i][j].piece->joueur==joueur_actu){
														tab[joueur_actu].pts_action_max+=terrain[i][j].piece->pts_action_max;
														terrain[i][j].piece->pts_action_actu=terrain[i][j].piece->pts_action_max;
												}
											}
										}
										tab[joueur_actu].pts_action_actu=tab[joueur_actu].pts_action_max;

										do{
											joueur_actu=(joueur_actu+1)%J;
											if(joueur_actu==joueur_start){
												compteur_tour++;
												update_grille(terrain,compteur_tour,tab_info_bash,variable2);
											}
										}while(tab[joueur_actu].id_joueur==-1);
										if(tab[joueur_actu].humain==1){
											camera_sur_allie(terrain,joueur_actu,tab,largeur,hauteur);
										}
										tab[joueur_actu].pts_action_max=0;
										for(int i=0;i<N;i++){
											for(int j=0;j<M;j++){
												if(terrain[i][j].piece && terrain[i][j].piece->joueur==joueur_actu){
														tab[joueur_actu].pts_action_max+=terrain[i][j].piece->pts_action_max;
														terrain[i][j].piece->pts_action_actu=terrain[i][j].piece->pts_action_max;
												}
											}
										}
										tab[joueur_actu].pts_action_actu=tab[joueur_actu].pts_action_max;
										fin_tour=0;
									}


									//incrémentation a chaque tours
									nb_tour=(nb_tour+1)%(VITESSE_JEU_BOT*2);
									compteur_anim=compteur_anim%(VITESSE_ANIM*4);
									compteur_anim++;
									jour_nuit=(jour_nuit+1)%DURE_JOUR_NUIT;
									compteur_bouton_cam=(compteur_bouton_cam-1)%(-200);

									SDL_GetWindowSize(pWindow,largeur,hauteur); //initialisation des pointeur largeur et hauteur sur la taille de la fenetre
									test=(*largeur);
									COEF_AFFICHAGE=test / 1920.00;









									clean_degat_txt(aff_deg);
									SDL_RenderPresent(renderer);
									SDL_Delay(16);



								//===================================ACTUALISATION DES TOURS==========================================//

								}//fin boucle de jeu
							}
						}
					break;
				}
			}


			//================================================================================================================================================//
			//================================================================================================================================================//
			//=============================================================CHARGEMENT DE PARTIE===============================================================//
			//================================================================================================================================================//
			//================================================================================================================================================//

		}else if(running == Load){

			/*frame_anim_montre=4;
			jour_nuit=((DURE_JOUR_NUIT/24)*7);

		  FILE * fp;
		  fp=fopen("fichiers/saves/save_2201_1628", "r");
		  //on commence par save les variable principales de la partie et les types de cases
		  fscanf(fp,"N=%d M=%d J=%d J_HUMAIN=%d VITESSE_JEU_BOT=%d VITESSE_ANIM=%d DURE_JOUR_NUIT=%d compteur_tour=%d joueur_actu=%d\n",&N,&M,&J,&J_HUMAIN,&VITESSE_JEU_BOT,&VITESSE_ANIM,&DURE_JOUR_NUIT,&compteur_tour,&joueur_actu);
			fprintf(stderr,"ok\n");
			for(int i=0;i<N;i++){
		    for(int j=0;j<M;j++){
		      fscanf(fp,"{%d %d} type=%d xImg=%d yImg=%d x1=%d x2=%d x3=%d x4=%d y1=%d y2=%d y3=%d y4=%d\n",&i,&j,&terrain[i][j].type,&terrain[i][j].xImg,&terrain[i][j].yImg,&terrain[i][j].x1,&terrain[i][j].x2,&terrain[i][j].x3,&terrain[i][j].x4,&terrain[i][j].y1,&terrain[i][j].y2,&terrain[i][j].y3,&terrain[i][j].y4);
					//terrain[i][j].deplacement=0;
					//terrain[i][j].attaque=0;
				}
		  }
		  //on sauvegarde le tableau de joueurs
		  for(int i=0;i<J;i++){
		    fscanf(fp,"id_joueur=%d nb_unite=%d humain=%d pts_action_max=%d pts_action_actu=%d\n",&tab[i].id_joueur,&tab[i].nb_unite,&tab[i].humain,&tab[i].pts_action_max,&tab[i].pts_action_actu);
		  }
		  //on sauvegarde chaque piece
			int test=-1;
		  for(int i=0;i<N;i++){
		    for(int j=0;j<M;j++){
					fscanf(fp,"{%d %d} classe=%d",&i,&j,&test);
					fprintf(stderr,"%d\n",test);
					if(test!=-1){
						terrain[i][j].piece=malloc(sizeof(piece_t));
						terrain[i][j].piece->classe=test;
						fscanf(fp,"pdv=%d puissance=%d armure=%d block=%d portee=%d deplacement=%d joueur=%d direction=%d kill=%d frame=%d frame_interface=%d start_anim=%d \n",&terrain[i][j].piece->pdv,&terrain[i][j].piece->puissance,&terrain[i][j].piece->armure,&terrain[i][j].piece->block,&terrain[i][j].piece->portee,&terrain[i][j].piece->deplacement,&terrain[i][j].piece->joueur,&terrain[i][j].piece->direction,&terrain[i][j].piece->kill,&terrain[i][j].piece->frame,&terrain[i][j].piece->frame_interface,&terrain[i][j].piece->start_anim);
						terrain[i][j].piece->select=0;
					}else{
						terrain[i][j].piece=NULL;
					}
		    }
		  }

			for(int i=0;i<N;i++){
				for(int j=0;j<M;j++){
					if(terrain[i][j].piece)
						fprintf(stderr,"%d %d : pdv piece %d portee piece : %d\n",i,j,terrain[i][j].piece->pdv,terrain[i][j].piece->portee);
				}
			}
			exit(1);
		  fclose(fp);
			fprintf(stderr,"ok\n");
			running = Play_load;*/
			running = Menu2;

			//================================================================================================================================================//
			//================================================================================================================================================//
			//=============================================================HELP===============================================================================//
			//================================================================================================================================================//
			//================================================================================================================================================//
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
			//================================================================================================================================================//
			//================================================================================================================================================//
			//=============================================================ABOUT==============================================================================//
			//================================================================================================================================================//
			//================================================================================================================================================//
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
		}
		}
	}else {
		fprintf(stderr,"Erreur de création de la fenêtre: %s\n",SDL_GetError());
	}

	//Destruction de la fenetre
	SDL_DestroyWindow(pWindow);

//	TTF_CloseFont(police); /* Doit être avant TTF_Quit() */
	TTF_Quit();
  SDL_Quit();
	return 0;
}
