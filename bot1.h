#include "struct.h"
#include "gestion.h"

#ifndef bot1_h
#define bot1_h
/* needs amount of wagons remaining,
at the beggining takes all 3 objectives
places a route whenever possible
otherwise draws 2 blind cards*/
void bot_dumb1(route** mat, MoveData* mymove, CardColor* tab_color, MoveResult* moveresult, int nbcity, int* debut);

#endif