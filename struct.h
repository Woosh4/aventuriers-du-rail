#include "ticketToRide.h"

#ifndef struct_h
#define struct_h

typedef struct{
    int length;
    CardColor color;
    CardColor color2;
    int taken;
    // currently -1 free 0 taken by player 1 bot

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
    Objective** objective;
    int player_number;
}Player_Info;

typedef struct{
    int city1; //filled with dijkstra
    int city2; //filled with dijkstra
    float ev; // points / nbwagons for placing a road, how many for blocking a road?
    int nbwagons; //filled with dijkstra
    int* path; //filled with dijkstra
    int* priority; // array to know the order in which we want to place the routes
    // path : [city1, city2, city2, city3, ...,cityx, -1, -1]= road from 1→2 then 2→3
}To_Place;

typedef struct{
    int checked;
    int weight;
    int prev;
}Dijkstra_City;
// tab containing shortest + priority level to track which routes i want to take?
#endif