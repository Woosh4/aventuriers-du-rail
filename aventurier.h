#include "ticketToRide.h"

typedef struct route_{
    int from;
    int to;
    int longueur;
    CardColor color;
    CardColor color2;
    int occupied;
    // 0 free, 1 player, 2 bot

}route;