#include "struct.h"
#include "gestion.h"

#ifndef bot2_h
#define bot2_h
/* 
at the beggining takes all 3 objectives
place routes between objectives with dijkstra, ordered by points / wagons
draw visible cards for current route, place routes in order to connect the objectives
takes a new objective when none left (count nb wagons remaining + expected value)*/
void bot_2(Board* bord, Player_Info* info);
#endif