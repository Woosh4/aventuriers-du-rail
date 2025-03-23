#include "ticketToRide.h"

typedef struct route_{
    int from;
    int to;
    int length;
    CardColor color;
    CardColor color2;
    int taken;
    // 0 free, 1 player, 2 bot

}route;