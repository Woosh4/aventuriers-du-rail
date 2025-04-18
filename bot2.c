#include "bot2.h"
#include <stdio.h>

void bot_2(Board* bord, Player_Info* info){
    //beginning of the game
    //picks all 3 objectives (to change, with: calculate EV, compare to an average?)

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

    // TODO: check if enough wagons are left before ?? picking objective, placing road?, other?
    
    //debug
    print_toplace(toplace);
    printf("MAX EV FOUND : N° %d, EV=%f\n", max_i, toplace[max_i]->ev);



    //cleanup
    destroy_toplace(toplace);
    return;
}