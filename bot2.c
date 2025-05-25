#include "bot2.h"
#include <stdio.h>

void bot_2(Board* bord, Player_Info* info){
    //beginning of the game
    //picks all 3 objectives (to change, with: calculate EV, compare to an average?)
    //TODO IMPORTANT: have an array with all the colors needed to fill an objective ?, to pick cards effectively?

    // if(bord->when == -1){
    //     bord->when = 0;
    //     info->movedata->action = 4;
    //     sendMove(info->movedata,info->moveresult);
    //     info->movedata->action = 5;
    //     info->movedata->chooseObjectives[0] = 1;
    //     info->movedata->chooseObjectives[1] = 1;
    //     info->movedata->chooseObjectives[2] = 1;
    //     sendMove(info->movedata,info->moveresult);
    //     update_player_info(info, bord);
    //     return;
    // }

    //init toplace
    int replay = 0;
    To_Place** toplace = To_place_create(bord, info);


    //if no objective left pick 1 more TO BE CHANGED ---------------
    if(toplace[0] == NULL){
        // info->movedata->action = 4;
        // sendMove(info->movedata,info->moveresult);

        pick_new_objectives(toplace, info, bord);
        
        // info->movedata->action = 5;
        // info->movedata->chooseObjectives[0] = 1;
        // info->movedata->chooseObjectives[1] = 0;
        // info->movedata->chooseObjectives[2] = 0;
        sendMove(info->movedata,info->moveresult);
        update_player_info(info, bord);
        destroy_toplace(toplace, info);
        return;
    }

    int max_i = find_max_ev(toplace);
    update_priority(bord, info, toplace);
    int road = find_min_priority(bord, info, toplace, max_i);
    getBoardState(bord->cards_pickable);
    int choice = search_color_pick(bord, info, toplace, max_i, 0);

    //pick visible card
    if(choice >= 1 && choice <= 8){
        info->movedata->action = 3;
        info->movedata->drawCard = choice;
        replay = 1;
    }
    //draw blind card
    else if(choice == 10){
        info->movedata->action = 2;
        replay = 1;
    }
    //place road without joker
    else if(choice <= -1 && choice >= -8){
        info->movedata->action = 1;
        info->movedata->claimRoute.from = toplace[max_i]->path[road];
        info->movedata->claimRoute.to = toplace[max_i]->path[road+1];
        info->movedata->claimRoute.color = -choice;
        info->movedata->claimRoute.nbLocomotives = 0;
    }
    //place route with joker
    else if(choice <= -11 && choice >= -18){
        info->movedata->action = 1;
        info->movedata->claimRoute.from = toplace[max_i]->path[road];
        info->movedata->claimRoute.to = toplace[max_i]->path[road+1];
        info->movedata->claimRoute.color = -choice -10;
        info->movedata->claimRoute.nbLocomotives = find_nb_joker(bord, info, toplace, max_i, road, choice);
    }

    ////////bunch of debug
    printf("CHOICE MADE BY SEARCH_COLOR : %d\n", choice);
    print_toplace(toplace);
    printf("MAX EV FOUND : N° %d, EV=%f\n", max_i, toplace[max_i]->ev);
    printf("ROAD TO BE PLACED FOUND : INDEX IN TOPLACE[MAX]: %d, FROM:%d, TO:%d\n", road, toplace[max_i]->path[road], toplace[max_i]->path[road+1]);
    ///////////
    sendMove(info->movedata, info->moveresult);
    update_board(bord, info);
    update_player_info(info, bord);
    getBoardState(bord->cards_pickable);

    if(replay){
        // take a new choice (with pick=1 !)
        choice = search_color_pick(bord, info, toplace, max_i, 1);

        //pick visible card
        if(choice >= 1 && choice <= 8){
            info->movedata->action = 3;
            info->movedata->drawCard = choice;
            replay = 1;
        }
        //draw blind card
        else if(choice == 10){
            info->movedata->action = 2;
            replay = 1;
        }

        ////////bunch of debug
        printf("CHOICE MADE BY SEARCH_COLOR : %d\n", choice);
        print_toplace(toplace);
        printf("MAX EV FOUND : N° %d, EV=%f\n", max_i, toplace[max_i]->ev);
        printf("ROAD TO BE PLACED FOUND : INDEX IN TOPLACE[MAX]: %d, FROM:%d, TO:%d\n", road, toplace[max_i]->path[road], toplace[max_i]->path[road+1]);
        ///////////
        sendMove(info->movedata, info->moveresult);
        update_board(bord, info);
        update_player_info(info, bord);
    }

    //find index in toplace, then find road index, then find (color to be picked OR color to be placed OR Else?)

    // TODO: check if enough wagons are left before ?? picking objective, placing road?, other?
    //change dijkstra : priority to (roads without color?, less roads?)
    // wtf happens if nothing in toplace, if city is blocked, ... debug
    // pioche objectifs: debut, milieu partie
    // tableau avec nb couleurs nécessaires pour le chemin entier → routes de plusieurs couleurs
    // route sans couleur: pick random ou pioche couleur ?


    //debug
    // print_toplace(toplace);
    // printf("MAX EV FOUND : N° %d, EV=%f\n", max_i, toplace[max_i]->ev);
    // printf("ROAD TO BE PLACED FOUND : INDEX IN TOPLACE[MAX]: %d, FROM:%d, TO:%d\n", road, toplace[max_i]->path[road], toplace[max_i]->path[road+1]);

    //cleanup
    destroy_toplace(toplace, info);
    return;
}