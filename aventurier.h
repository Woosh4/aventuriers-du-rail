#include "ticketToRide.h"

typedef struct route_{
    /* supprimer from to, ajouter autres structures board, */
    int from;
    int to;
    int length;
    CardColor color;
    CardColor color2;
    int taken;
    // 0 free, 1 player, 2 bot

}route;

typedef struct{
    int nbcity;
    int nbroute;
}board;

typedef struct{
    int score;
    int nbwagons;
    int nbcards;
    int* cards;
}player_info;