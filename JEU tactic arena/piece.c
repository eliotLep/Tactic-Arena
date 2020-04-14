#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "piece.h"







piece_t * init_piece(classe_t classe,int id_joueur){
    int pos=((rand()%(4)) + 1);
    piece_t * piece=malloc(sizeof(piece_t));
    int frame=((rand()%2)+1);
    int frame2=((rand()%10)+1);
    int start_anim=((rand()%VITESSE_ANIM));
    switch (classe){
        case knight:
            piece->classe = knight;
            piece->pdv=35;
            piece->puissance = 20;
            piece->armure=40;
            piece->block=80;
            piece->portee=1;
            piece->deplacement=3;
            piece->select=0;
            piece->joueur=id_joueur;
            piece->direction=pos;
            piece->kill=0;
            piece->frame=frame;
            piece->frame_interface=frame;
            piece->start_anim=start_anim;
            piece->pts_action_max=PTS_ACTION_UNI;
            piece->pts_action_actu=piece->pts_action_max;
        break;
        case scout:
            piece->classe = scout;
            piece->pdv=32;
            piece->puissance = 24;
            piece->armure=15;
            piece->block=30;
            piece->portee=5;
            piece->deplacement=4;
            piece->select=0;
            piece->joueur=id_joueur;
            piece->direction=pos;
            piece->kill=0;
            piece->frame=frame;
            piece->frame_interface=frame;
            piece->start_anim=start_anim;
            piece->pts_action_max=PTS_ACTION_UNI;
            piece->pts_action_actu=piece->pts_action_max;
        break;
        case priest:
            piece->classe = priest;
            piece->pdv=34;
            piece->puissance = 14;
            piece->armure=15;
            piece->block=10;
            piece->portee=3;
            piece->deplacement=3;
            piece->select=0;
            piece->joueur=id_joueur;
            piece->direction=pos;
            piece->kill=0;
            piece->frame=frame;
            piece->frame_interface=frame;
            piece->start_anim=start_anim;
            piece->pts_action_max=PTS_ACTION_UNI;
            piece->pts_action_actu=piece->pts_action_max;
        break;
        case magician:
            piece->classe = magician;
            piece->pdv=30;
            piece->puissance = 28;
            piece->armure=8;
            piece->block=40;
            piece->portee=4;
            piece->deplacement=3;
            piece->select=0;
            piece->joueur=id_joueur;
            piece->direction=pos;
            piece->kill=0;
            piece->frame=frame;
            piece->frame_interface=frame;
            piece->start_anim=start_anim;
            piece->pts_action_max=PTS_ACTION_UNI;
            piece->pts_action_actu=piece->pts_action_max;
        break;
        case faucheuse:
            piece->classe = faucheuse;
            piece->pdv=25;
            piece->puissance = 32;
            piece->armure=10;
            piece->block=30;
            piece->portee=2;
            piece->deplacement=4;
            piece->select=0;
            piece->joueur=id_joueur;
            piece->direction=pos;
            piece->kill=0;
            piece->frame=frame;
            piece->frame_interface=frame;
            piece->start_anim=start_anim;
            piece->pts_action_max=PTS_ACTION_UNI;
            piece->pts_action_actu=piece->pts_action_max;
        break;
        case ange:
            piece->classe = ange;
            piece->pdv=40;
            piece->puissance = 22;
            piece->armure=5;
            piece->block=30;
            piece->portee=1;
            piece->deplacement=3;
            piece->select=0;
            piece->joueur=id_joueur;
            piece->direction=pos;
            piece->kill=0;
            piece->frame=frame2;
            piece->frame_interface=frame2;
            piece->start_anim=start_anim;
            piece->pts_action_max=PTS_ACTION_UNI;
            piece->pts_action_actu=piece->pts_action_max;
        break;
        default: printf("Cette classe n'existe pas.\n");
    }
    return (piece);
}

int piece_existe(piece_t * piece){
    return(!(piece==NULL));
}


int destruction_piece(piece_t * piece){
    if(piece_existe(piece)){
        free(piece);
        (piece)=NULL;
        return 1;
    }
    return 0;
}

