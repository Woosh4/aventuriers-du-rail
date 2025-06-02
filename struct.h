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
    int gofast; // 1 if we want to expedite the game
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
    int points_place; // points from road placements : used for EV
    int length_est; // estimate of the length of the path (in nb wagons) : we assume the paths with a lower priority are already taken.
    CardColor* col; // array with how much of each color we need to build the path
}To_Place;

typedef struct{
    int checked;
    float weight; // weight calculated
    int nb_wagons; // number of wagons from origin city
    int color_forced; // nb of wagons of a set color we are forced to use (i try to minimize it to gain speed during road placement)
    int points_road; // how many points do we get from placing roads? for Dijkstra
    int prev; // previous city, to rebuild the path
}Dijkstra_City;

typedef struct{
    int move; // place, pick random..
    int city1;
    int city2;
    CardColor color;
    int joker; // nb of joker needed
}Action_order;

#endif