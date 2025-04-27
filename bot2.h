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

// ideal algorithm? (not implemented)
// 
// PICKING STARTING OBJECTIVES :
// calculate EV : ((objective points + path points) /wagons) for shortest path of each objective
// always pick the 2 with the highest EV (forced)
// how to decide for the 3rd one ? determine the average EV for all objectives with an empty board, if higher: take, if lower: pass
// 
// 
// 
// ROAD PLACEMENT :
// find the road we want to place (color 1st; then any color roads) in the current objective
// when to activate gofast? half of wagons remaining? half of opponents wagons? mix of both? less wagons?
//
// if color :
// 1 try to place the colored road
// 2 if failed, try to pick a card of the right color
// 3 if failed, try to pick a card for the next road of color (in same objective) (trigger pick mode)
// 4 if failed, try to place the current road with a joker (not gofast)
// 5 if all failed:
// - pick a random card (not gofast) ?
// - try to pick a joker (gofast) otherwise pick random ?

// if the road has no set color (meaning all roads with a set color in the current objective have been placed)
// 1 try to place the road (what color?? very important)
// 2 if can't / chose not to place it pick a cards (specific color? random? joker? with gofast)
// 
// NO OBJECTIVES LEFT :
// decide :
// - pick new objectives (nb wagons left? compared to opponent?), how many?
// - go for longest road
// - block the opponent (will take a long time to implement)
