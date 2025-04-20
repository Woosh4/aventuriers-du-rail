#include "bot2.h"
#include <stdio.h>

void bot_2(Board* bord, Player_Info* info){
    //beginning of the game
    //picks all 3 objectives (to change, with: calculate EV, compare to an average?)
    //TODO IMPORTANT: have an array with all the colors needed to fill an objective ?, to pick cards effectively?

    if(bord->when == -1){
        bord->when = 0;
        info->movedata->action = 4;
        sendMove(info->movedata,info->moveresult);
        info->movedata->action = 5;
        info->movedata->chooseObjectives[0] = 1;
        info->movedata->chooseObjectives[1] = 1;
        info->movedata->chooseObjectives[2] = 1;
        sendMove(info->movedata,info->moveresult);
        update_player_info(info, bord);
        return;
    }

    //init toplace
    To_Place** toplace = To_place_create(bord, info);
    int max_i = find_max_ev(toplace);
    // int road_i = search_index(bord, info, toplace, max_i, 0);
    // int road2_i = search_index(bord, info, toplace, max_i, 1);
    update_priority(bord, info, toplace);

    //find index in toplace, then find road index, then find (color to be picked OR color to be placed OR Else?)

    // TODO: check if enough wagons are left before ?? picking objective, placing road?, other?
    //change dijkstra : priority to (roads without color?, less roads?)
    // wtf happens if nothing in toplace, if city is blocked, ... debug
    // pioche objectifs: debut, milieu partie
    // have an array to know the order in which we want to place the roads instead of looking at the next city (change search color pick)


    //debug
    print_toplace(toplace);
    printf("MAX EV FOUND : N° %d, EV=%f\n", max_i, toplace[max_i]->ev);
    // printf("ROAD TO BE PLACED FOUND : INDEX IN TOPLACE[MAX]: %d, FROM:%d, TO:%d\n", road_i, toplace[max_i]->path[road_i], toplace[max_i]->path[road_i+1]);
    // printf("NEXT ROAD TO BE PLACED FOUND : INDEX IN TOPLACE[MAX]: %d, FROM:%d, TO:%d\n", road2_i, toplace[max_i]->path[road2_i], toplace[max_i]->path[road2_i+1]);

    //cleanup
    destroy_toplace(toplace);
    return;
}