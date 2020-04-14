#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "interface.h"

SDL_Surface *gpScreen;
SDL_Renderer *renderer;
TTF_Font *font;
FILE *fp2;
SDL_Color r = {255,100,0,255};
SDL_Color o = {255,165,0,255};
SDL_Color b = {255,255,255,255};

void loadImage(image_t image[Z],SDL_Renderer *renderer){
	fp2=fopen ("images/lien_image.txt","r");
	for(int i=0;i<Z && !feof(fp2);i++){

		fscanf(fp2, "%s", image[i].loc_image);
		image[i].texture=IMG_LoadTexture(renderer,image[i].loc_image);
	}

	fclose(fp2);
}

void afficher_img(int x,int y,int h,int w,char img[50],image_t image[Z],SDL_Renderer *renderer,float coefZoom,int anim,SDL_Rect img_anim){
	if(x>-3000 && x<3000 && y<3000 && y>-3000){
		SDL_Rect imgDestRect;
		imgDestRect.x = x;
		imgDestRect.y = y;
		imgDestRect.w=w*coefZoom;
		imgDestRect.h=h*coefZoom;
		for(int i=0;i<Z;i++){
			if(!strcmp(image[i].loc_image,img)){
				if(anim==0){
					SDL_RenderCopy(renderer, image[i].texture, NULL, &imgDestRect);
				}else{
					SDL_RenderCopy(renderer, image[i].texture,&img_anim, &imgDestRect);
				}
				return;
			}
		}
	}
}

void AfficherText(char* message, char* fontFile, SDL_Color color, int fontSize,SDL_Renderer *renderer,int x,int y)
{
	SDL_Rect txtDestRect;
	font = TTF_OpenFont(fontFile, fontSize);
	if (font == NULL)
		fprintf(stderr, "erreur chargement font\n");

	SDL_Surface *surf = TTF_RenderText_Blended(font, message, color);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);

	SDL_FreeSurface(surf);
	TTF_CloseFont(font);

	txtDestRect.x = x;
	txtDestRect.y = y;
	SDL_QueryTexture(texture,NULL,NULL,&txtDestRect.w,&txtDestRect.h);
	SDL_RenderCopy(renderer, texture, NULL, &txtDestRect);
	SDL_DestroyTexture(texture);
}

int draw_menu(SDL_Renderer *renderer,SDL_Window* pWindow){
	gpScreen = SDL_GetWindowSurface(pWindow);
	int longeur = gpScreen->h/5;
	int fonsize = longeur/2;
	int y = fonsize/2;
	int x = gpScreen->w/10;
	SDL_SetRenderDrawColor(renderer, 50, 50, 50, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderDrawLine(renderer, 0, longeur, gpScreen->w, longeur);
	SDL_RenderDrawLine(renderer, 0, longeur*2, gpScreen->w, longeur*2);
	SDL_RenderDrawLine(renderer, 0, longeur*3, gpScreen->w, longeur*3);
	SDL_RenderDrawLine(renderer, 0, longeur*4, gpScreen->w, longeur*4);

	AfficherText("TACTICS ARENA", "arial.ttf",r,fonsize,renderer,x*2.5,y);
	AfficherText("NEW GAME", "arial.ttf",o,fonsize-10,renderer,x*3,y*5);
	AfficherText("LOAD GAME", "arial.ttf",o,fonsize-10,renderer,x*3,y*9);
	AfficherText("ABOUT US", "arial.ttf",o,fonsize-10,renderer,x*3,y*13);
	AfficherText("QUIT", "arial.ttf",o,fonsize-10,renderer,x*3,y*17);

  SDL_RenderPresent(renderer);
	return longeur;
}

void about(SDL_Renderer *renderer,SDL_Window* pWindow){
	gpScreen = SDL_GetWindowSurface(pWindow);
	int longeur = gpScreen->h/5;
	int fonsize = longeur/4;
	int y = fonsize/2;
	int x = gpScreen->w/10;
	SDL_SetRenderDrawColor(renderer, 50, 50, 50, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

	AfficherText("L2 INFO Conduite de Projet", "arial.ttf",r,fonsize,renderer,x*3,y);
	AfficherText("Tactics Arena", "arial.ttf",r,fonsize,renderer,x*3,y*4);
	AfficherText("LEPOITTEVIN ELIOT", "arial.ttf",o,fonsize-15,renderer,x*3,y*8);
	AfficherText("LIBAN WILLHEM", "arial.ttf",o,fonsize-15,renderer,x*3,y*10);
	AfficherText("LIU YICHENG", "arial.ttf",o,fonsize-15,renderer,x*3,y*12);
	AfficherText("BACK", "arial.ttf",b,15,renderer,gpScreen->w - 60,gpScreen->h - 30);
  SDL_RenderPresent(renderer);
}

void ingame_menu(SDL_Renderer *renderer,SDL_Window* pWindow){
	gpScreen = SDL_GetWindowSurface(pWindow);
	SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_Rect menu;
    menu.x = gpScreen->w - 260;
    menu.y = gpScreen->h - 330;
    menu.w = 200;
    menu.h = 300;
    SDL_RenderFillRect(renderer,&menu);

		int n = 300/4;
		SDL_SetRenderDrawColor(renderer, 50, 50, 50, SDL_ALPHA_OPAQUE);
	  SDL_RenderDrawLine(renderer, menu.x, menu.y+n, menu.x+200, menu.y+n);
		SDL_RenderDrawLine(renderer, menu.x, menu.y+n*2, menu.x+200, menu.y+n*2);
		SDL_RenderDrawLine(renderer, menu.x, menu.y+n*3, menu.x+200, menu.y+n*3);

		AfficherText("Reculer", "arial.ttf",o,menu.h/10,renderer,menu.x+20,menu.y+menu.h/13);
		AfficherText("Save Game", "arial.ttf",o,menu.h/10,renderer,menu.x+20,menu.y+n+menu.h/13);
		AfficherText("Help", "arial.ttf",o,menu.h/10,renderer,menu.x+20,menu.y+n*2+menu.h/13);
		AfficherText("Quit", "arial.ttf",o,menu.h/10,renderer,menu.x+20,menu.y+n*3+menu.h/13);
		SDL_RenderPresent(renderer);
}

void help(SDL_Renderer *renderer,SDL_Window* pWindow){
	gpScreen = SDL_GetWindowSurface(pWindow);
	int longeur = gpScreen->h/5;
	int fonsize = longeur/4;
	int y = fonsize/2;
	int x = gpScreen->w/10;
	SDL_SetRenderDrawColor(renderer, 50, 50, 50, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

	AfficherText("Comment jouer TACTICS ARENA?", "arial.ttf",r,fonsize,renderer,x*3,y);
	AfficherText(".......................", "arial.ttf",r,fonsize,renderer,x*3,y*4);
	AfficherText("BACK", "arial.ttf",b,15,renderer,gpScreen->w - 60,gpScreen->h - 30);
  SDL_RenderPresent(renderer);
}

void mod_menu(SDL_Renderer *renderer,SDL_Window* pWindow,image_t *image){
	char variable[80];
	SDL_Color o = {0,0,0,0};
	SDL_Rect useless;
	gpScreen = SDL_GetWindowSurface(pWindow);
	int largeur=gpScreen->w/100;
	int hauteur=gpScreen->h/100;
  SDL_RenderClear(renderer);
	afficher_img(0,0,gpScreen->h,gpScreen->w,"images/menu.png",image,renderer,1,0,useless);

	largeur=gpScreen->w/100 * 14 ;
	hauteur=gpScreen->h/100 * 18;
	AfficherText("Humains: ", "arial.ttf",o,40*COEF_AFFICHAGE,renderer,largeur,hauteur);
	afficher_img((largeur+400*COEF_AFFICHAGE),hauteur,60*COEF_AFFICHAGE,60*COEF_AFFICHAGE,"images/bouton_plus.png",image,renderer,1,0,useless);
	afficher_img((largeur+470*COEF_AFFICHAGE),hauteur,60*COEF_AFFICHAGE,60*COEF_AFFICHAGE,"images/bouton_moins.png",image,renderer,1,0,useless);
	sprintf(variable, "%d",  J_HUMAIN);
	AfficherText(variable, "arial.ttf",o,40*COEF_AFFICHAGE,renderer,(largeur+300*COEF_AFFICHAGE),hauteur);

	largeur=gpScreen->w/100 * 58;
	hauteur=gpScreen->h/100 * 18;
	AfficherText("points actions: ", "arial.ttf",o,40*COEF_AFFICHAGE,renderer,largeur,hauteur);
	afficher_img((largeur+400*COEF_AFFICHAGE),hauteur,60*COEF_AFFICHAGE,60*COEF_AFFICHAGE,"images/bouton_plus.png",image,renderer,1,0,useless);
	afficher_img((largeur+470*COEF_AFFICHAGE),hauteur,60*COEF_AFFICHAGE,60*COEF_AFFICHAGE,"images/bouton_moins.png",image,renderer,1,0,useless);
	sprintf(variable, "%d",  PTS_ACTION_UNI);
	AfficherText(variable, "arial.ttf",o,40*COEF_AFFICHAGE,renderer,(largeur+300*COEF_AFFICHAGE),hauteur);

	largeur=gpScreen->w/100 * 14;
	hauteur=gpScreen->h/100 * 34;
	AfficherText("ordis: ", "arial.ttf",o,40*COEF_AFFICHAGE,renderer,largeur,hauteur);
	afficher_img(largeur+400*COEF_AFFICHAGE,hauteur,60*COEF_AFFICHAGE,60*COEF_AFFICHAGE,"images/bouton_plus.png",image,renderer,1,0,useless);
	afficher_img(largeur+470*COEF_AFFICHAGE,hauteur,60*COEF_AFFICHAGE,60*COEF_AFFICHAGE,"images/bouton_moins.png",image,renderer,1,0,useless);
	sprintf(variable, "%d",  J_BOT);
	AfficherText(variable, "arial.ttf",o,40*COEF_AFFICHAGE,renderer,largeur+300*COEF_AFFICHAGE,hauteur);

	largeur=gpScreen->w/100 * 58;
	hauteur=gpScreen->h/100 * 34;
	AfficherText("Max pretre: ", "arial.ttf",o,40*COEF_AFFICHAGE,renderer,largeur,hauteur);
	afficher_img(largeur+400*COEF_AFFICHAGE,hauteur,60*COEF_AFFICHAGE,60*COEF_AFFICHAGE,"images/bouton_plus.png",image,renderer,1,0,useless);
	afficher_img(largeur+470*COEF_AFFICHAGE,hauteur,60*COEF_AFFICHAGE,60*COEF_AFFICHAGE,"images/bouton_moins.png",image,renderer,1,0,useless);
	sprintf(variable, "%d",  NB_MAX_PRIEST);
	AfficherText(variable, "arial.ttf",o,40*COEF_AFFICHAGE,renderer,largeur+300*COEF_AFFICHAGE,hauteur);


	largeur=gpScreen->w/100 * 14;
	hauteur=gpScreen->h/100 * 50;
	AfficherText("Unites: ", "arial.ttf",o,40*COEF_AFFICHAGE,renderer,largeur,hauteur);
	afficher_img(largeur+400*COEF_AFFICHAGE,hauteur,60*COEF_AFFICHAGE,60*COEF_AFFICHAGE,"images/bouton_plus.png",image,renderer,1,0,useless);
	afficher_img(largeur+470*COEF_AFFICHAGE,hauteur,60*COEF_AFFICHAGE,60*COEF_AFFICHAGE,"images/bouton_moins.png",image,renderer,1,0,useless);
	sprintf(variable, "%d",  NB_UNITE);
	AfficherText(variable, "arial.ttf",o,40*COEF_AFFICHAGE,renderer,largeur+300*COEF_AFFICHAGE,hauteur);

		largeur=gpScreen->w/100 * 58;
	hauteur=gpScreen->h/100 * 50;
	AfficherText("duree jour/nuit: ", "arial.ttf",o,40*COEF_AFFICHAGE,renderer,largeur,hauteur);
	afficher_img(largeur+400*COEF_AFFICHAGE,hauteur,60*COEF_AFFICHAGE,60*COEF_AFFICHAGE,"images/bouton_plus.png",image,renderer,1,0,useless);
	afficher_img(largeur+470*COEF_AFFICHAGE,hauteur,60*COEF_AFFICHAGE,60*COEF_AFFICHAGE,"images/bouton_moins.png",image,renderer,1,0,useless);
	sprintf(variable, "%d",  DURE_JOUR_NUIT);
	AfficherText(variable, "arial.ttf",o,40*COEF_AFFICHAGE,renderer,largeur+300*COEF_AFFICHAGE,hauteur);


	largeur=gpScreen->w/100 * 14;
	hauteur=gpScreen->h/100 * 66;
	AfficherText("hauteur grille:", "arial.ttf",o,40*COEF_AFFICHAGE,renderer,largeur,hauteur);
	afficher_img(largeur+400*COEF_AFFICHAGE,hauteur,60*COEF_AFFICHAGE,60*COEF_AFFICHAGE,"images/bouton_plus.png",image,renderer,1,0,useless);
	afficher_img(largeur+470*COEF_AFFICHAGE,hauteur,60*COEF_AFFICHAGE,60*COEF_AFFICHAGE,"images/bouton_moins.png",image,renderer,1,0,useless);
	sprintf(variable, "%d",  M);
	AfficherText(variable, "arial.ttf",o,40*COEF_AFFICHAGE,renderer,largeur+300*COEF_AFFICHAGE,hauteur);

	largeur=gpScreen->w/100 * 14;
	hauteur=gpScreen->h/100 * 79;
	AfficherText("largeur grille: ", "arial.ttf",o,40*COEF_AFFICHAGE,renderer,largeur,hauteur);
	afficher_img(largeur+400*COEF_AFFICHAGE,hauteur,60*COEF_AFFICHAGE,60*COEF_AFFICHAGE,"images/bouton_plus.png",image,renderer,1,0,useless);
	afficher_img(largeur+470*COEF_AFFICHAGE,hauteur,60*COEF_AFFICHAGE,60*COEF_AFFICHAGE,"images/bouton_moins.png",image,renderer,1,0,useless);
	sprintf(variable, "%d",  N);
	AfficherText(variable, "arial.ttf",o,40*COEF_AFFICHAGE,renderer,largeur+300*COEF_AFFICHAGE,hauteur);


	largeur=gpScreen->w/100 * 50;
	hauteur=gpScreen->h/100 * 71;
	afficher_img(largeur,hauteur,150*COEF_AFFICHAGE,150*COEF_AFFICHAGE,"images/option.png",image,renderer,1,0,useless);


	largeur=gpScreen->w/100 * 66;
	hauteur=gpScreen->h/100 * 81;
	AfficherText("Suivant", "arial.ttf",r,70*COEF_AFFICHAGE,renderer,largeur,hauteur);

	largeur=gpScreen->w/100 * 64;
	hauteur=gpScreen->h/100 * 68;
	AfficherText("Precedent", "arial.ttf",r,70*COEF_AFFICHAGE,renderer,largeur,hauteur);

  SDL_RenderPresent(renderer);
}




void mod_option(SDL_Renderer *renderer,SDL_Window* pWindow,image_t *image){
	SDL_Color o = {0,0,0,0};
	char variable[80];
	SDL_Rect useless;
	gpScreen = SDL_GetWindowSurface(pWindow);
	int largeur=gpScreen->w/100;
	int hauteur=gpScreen->h/100;
  SDL_RenderClear(renderer);
	afficher_img(0,0,gpScreen->h,gpScreen->w,"images/menu.png",image,renderer,1,0,useless);

	largeur=gpScreen->w/100 * 14;
	hauteur=gpScreen->h/100 * 18;
	AfficherText("espace gen: ", "arial.ttf",o,40*COEF_AFFICHAGE,renderer,largeur,hauteur);
	afficher_img(largeur+400*COEF_AFFICHAGE,hauteur,60*COEF_AFFICHAGE,60*COEF_AFFICHAGE,"images/bouton_plus.png",image,renderer,1,0,useless);
	afficher_img(largeur+470*COEF_AFFICHAGE,hauteur,60*COEF_AFFICHAGE,60*COEF_AFFICHAGE,"images/bouton_moins.png",image,renderer,1,0,useless);
	sprintf(variable, "%d",  ESPACE_GEN);
	AfficherText(variable, "arial.ttf",o,40*COEF_AFFICHAGE,renderer,largeur+300*COEF_AFFICHAGE,hauteur);

	largeur=gpScreen->w/100 * 58;
	hauteur=gpScreen->h/100 * 18;
	AfficherText("vitesse bots: ", "arial.ttf",o,40*COEF_AFFICHAGE,renderer,largeur,hauteur);
	afficher_img(largeur+400*COEF_AFFICHAGE,hauteur,60*COEF_AFFICHAGE,60*COEF_AFFICHAGE,"images/bouton_plus.png",image,renderer,1,0,useless);
	afficher_img(largeur+470*COEF_AFFICHAGE,hauteur,60*COEF_AFFICHAGE,60*COEF_AFFICHAGE,"images/bouton_moins.png",image,renderer,1,0,useless);
	sprintf(variable, "%d",  VITESSE_JEU_BOT);
	AfficherText(variable, "arial.ttf",o,40*COEF_AFFICHAGE,renderer,largeur+300*COEF_AFFICHAGE,hauteur);

	largeur=gpScreen->w/100 * 14;
	hauteur=gpScreen->h/100 * 34;
	AfficherText("lignes bash: ", "arial.ttf",o,40*COEF_AFFICHAGE,renderer,largeur,hauteur);
	afficher_img(largeur+400*COEF_AFFICHAGE,hauteur,60*COEF_AFFICHAGE,60*COEF_AFFICHAGE,"images/bouton_plus.png",image,renderer,1,0,useless);
	afficher_img(largeur+470*COEF_AFFICHAGE,hauteur,60*COEF_AFFICHAGE,60*COEF_AFFICHAGE,"images/bouton_moins.png",image,renderer,1,0,useless);
	sprintf(variable, "%d",  TAILLE_TAB_BASH);
	AfficherText(variable, "arial.ttf",o,40*COEF_AFFICHAGE,renderer,largeur+300*COEF_AFFICHAGE,hauteur);

	largeur=gpScreen->w/100 * 58;
	hauteur=gpScreen->h/100 * 34;
	AfficherText("scroll bash: ", "arial.ttf",o,40*COEF_AFFICHAGE,renderer,largeur,hauteur);
	afficher_img(largeur+400*COEF_AFFICHAGE,hauteur,60*COEF_AFFICHAGE,60*COEF_AFFICHAGE,"images/bouton_plus.png",image,renderer,1,0,useless);
	afficher_img(largeur+470*COEF_AFFICHAGE,hauteur,60*COEF_AFFICHAGE,60*COEF_AFFICHAGE,"images/bouton_moins.png",image,renderer,1,0,useless);
	sprintf(variable, "%d",  BASH_SCROLL_SPEED);
	AfficherText(variable, "arial.ttf",o,40*COEF_AFFICHAGE,renderer,largeur+300*COEF_AFFICHAGE,hauteur);


	largeur=gpScreen->w/100 * 14;
	hauteur=gpScreen->h/100 * 50;
	AfficherText("isometrique: ", "arial.ttf",o,40*COEF_AFFICHAGE,renderer,largeur,hauteur);
	afficher_img(largeur+400*COEF_AFFICHAGE,hauteur,60*COEF_AFFICHAGE,60*COEF_AFFICHAGE,"images/bouton_plus.png",image,renderer,1,0,useless);
	afficher_img(largeur+470*COEF_AFFICHAGE,hauteur,60*COEF_AFFICHAGE,60*COEF_AFFICHAGE,"images/bouton_moins.png",image,renderer,1,0,useless);
	sprintf(variable, "%d",  ISO);
	AfficherText(variable, "arial.ttf",o,40*COEF_AFFICHAGE,renderer,largeur+300*COEF_AFFICHAGE,hauteur);

	largeur=gpScreen->w/100 * 58;
	hauteur=gpScreen->h/100 * 50;
	AfficherText("vitesse menu: ", "arial.ttf",o,40*COEF_AFFICHAGE,renderer,largeur,hauteur);
	afficher_img(largeur+400*COEF_AFFICHAGE,hauteur,60*COEF_AFFICHAGE,60*COEF_AFFICHAGE,"images/bouton_plus.png",image,renderer,1,0,useless);
	afficher_img(largeur+470*COEF_AFFICHAGE,hauteur,60*COEF_AFFICHAGE,60*COEF_AFFICHAGE,"images/bouton_moins.png",image,renderer,1,0,useless);
	sprintf(variable, "%d",  VITESSE_INCREMENTATION);
	AfficherText(variable, "arial.ttf",o,40,renderer,largeur+300*COEF_AFFICHAGE,hauteur);

	largeur=gpScreen->w/100 * 64;
	hauteur=gpScreen->h/100 * 68;
	AfficherText("Precedent", "arial.ttf",r,70*COEF_AFFICHAGE,renderer,largeur,hauteur);

  SDL_RenderPresent(renderer);
}





void unit_menu(SDL_Renderer *renderer,SDL_Window* pWindow, joueur_unite_t joueur_unite){
	
	char variable[50];
	gpScreen = SDL_GetWindowSurface(pWindow);
	int longeur = gpScreen->h/5;
	int fonsize = longeur/3.5;
	int y = fonsize/2;
	int x = gpScreen->w/10;

	SDL_SetRenderDrawColor(renderer, 50, 50, 50, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

	AfficherText("joueur ", "arial.ttf",o,fonsize,renderer,x*4,y);
	sprintf(variable, "%d", joueur_unite.id_joueur);
	AfficherText(variable, "arial.ttf",o,fonsize,renderer,x*5.2,y);

	AfficherText("<", "arial.ttf",b,fonsize,renderer,x*3,y);
	AfficherText(">", "arial.ttf",b,fonsize,renderer,x*6,y);

	AfficherText("unit restant : ", "arial.ttf",o,fonsize-10,renderer,x*3.5,y*4);
	sprintf(variable, "%d", joueur_unite.restant_unite);
	AfficherText(variable, "arial.ttf",o,fonsize,renderer,x*5.5,y*4);

	AfficherText("knight", "arial.ttf",o,fonsize,renderer,x,y*9);
	AfficherText("<", "arial.ttf",b,fonsize,renderer,x,y*14);
	AfficherText(">", "arial.ttf",b,fonsize,renderer,x+fonsize*3,y*14);
	sprintf(variable, "%d", joueur_unite.unit[0]);
	AfficherText(variable, "arial.ttf",r,fonsize,renderer,x+fonsize*1.5,y*14);

	AfficherText("scout", "arial.ttf",o,fonsize,renderer,x*4,y*9);
	AfficherText("<", "arial.ttf",b,fonsize,renderer,x*4,y*14);
	AfficherText(">", "arial.ttf",b,fonsize,renderer,x*4+fonsize*3,y*14);
	sprintf(variable, "%d", joueur_unite.unit[1]);
	AfficherText(variable, "arial.ttf",r,fonsize,renderer,x*4+fonsize*1.5,y*14);

	AfficherText("priest", "arial.ttf",o,fonsize,renderer,x*7,y*9);
	AfficherText("<", "arial.ttf",b,fonsize,renderer,x*7,y*14);
	AfficherText(">", "arial.ttf",b,fonsize,renderer,x*7+fonsize*3,y*14);
	sprintf(variable, "%d", joueur_unite.unit[2]);
	AfficherText(variable, "arial.ttf",r,fonsize,renderer,x*7+fonsize*1.5,y*14);

	AfficherText("magician", "arial.ttf",o,fonsize,renderer,x,y*18);
	AfficherText("<", "arial.ttf",b,fonsize,renderer,x,y*25);
	AfficherText(">", "arial.ttf",b,fonsize,renderer,x+fonsize*3,y*25);
	sprintf(variable, "%d", joueur_unite.unit[3]);
	AfficherText(variable, "arial.ttf",r,fonsize,renderer,x+fonsize*1.5,y*25);

	AfficherText("faucheuse", "arial.ttf",o,fonsize,renderer,x*4,y*18);
	AfficherText("<", "arial.ttf",b,fonsize,renderer,x*4,y*25);
	AfficherText(">", "arial.ttf",b,fonsize,renderer,x*4+fonsize*3,y*25);
	sprintf(variable, "%d", joueur_unite.unit[4]);
	AfficherText(variable, "arial.ttf",r,fonsize,renderer,x*4+fonsize*1.5,y*25);

	AfficherText("ange", "arial.ttf",o,fonsize,renderer,x*7,y*18);
	AfficherText("<", "arial.ttf",b,fonsize,renderer,x*7,y*25);
	AfficherText(">", "arial.ttf",b,fonsize,renderer,x*7+fonsize*3,y*25);
	sprintf(variable, "%d", joueur_unite.unit[5]);
	AfficherText(variable, "arial.ttf",r,fonsize,renderer,x*7+fonsize*1.5,y*25);

	AfficherText("Precedent", "arial.ttf",b,fonsize,renderer,x,y*30);
	AfficherText("Suivant", "arial.ttf",b,fonsize,renderer,x*7,y*30);

	SDL_RenderPresent(renderer);
}

void init_joueur_unit(joueur_unite_t joueur_unite[],int J,int NB_UNIT){
	for(int i=0;i<J;i++){
		joueur_unite[i].id_joueur = i;
		joueur_unite[i].restant_unite = NB_UNIT;
		for(int j=0;j<nb_class;j++){
			joueur_unite[i].unit[j] = 0;
		}
	}
}
