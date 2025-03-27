#include "ticketToRide.h"

#ifndef struct_h
#define struct_h

typedef struct{
    int length;
    CardColor color;
    CardColor color2;
    int taken;
    // currently 0 free 1 taken, goal -1 free, 0 player 1 bot

}route;

typedef struct{
    int nbcity;
    int nbroute;
    int* cards_pickable; // cartes à piocher
}board;

typedef struct{
    int score;
    int nbwagons;
    int nbcards;
    int* cards;
    int nbobjective;
}player_info;

#endif