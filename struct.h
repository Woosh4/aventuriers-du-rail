#include "ticketToRide.h"

#ifndef struct_h
#define struct_h

typedef struct{
    int length;
    CardColor color;
    CardColor color2;
    int taken;
    // currently -1 free 0 taken by player 1 bot
    int weight;
    // fo Dijkstra

}route;

/* updated after getmove, other?*/
typedef struct{
    BoardState* cards_pickable; // cartes à piocher
    route** MatRoute; // matrix that represents all roads
    GameData* gamedata;
    int when; // =-1 at the begining, otherwise represents whose turn it is to play
}Board;

/* updated after board*/
typedef struct{
    MoveData* movedata;
    MoveResult* moveresult;
    int score;
    int nbwagons;
    int nbcards;
    CardColor* cards;
    int nbobjective;
    Objective* objective;
    int player_number;
}Player_Info;

typedef struct{
    int city1;
    int city2;
    int* path;
}To_Place;

// tab containing shortest + priority level to track which routes i want to take?
#endif