#include "bot2.h"

void bot_2(Board* bord, Player_Info* info){
    //beginning of the game
    //picks all 3 objectives (to change)

    // if(bord->when == -1){
    //     bord->when = 0;
    //     info->movedata->action = 4;
    //     sendMove(info->movedata,info->moveresult);
    //     info->movedata->action = 5;
    //     info->movedata->chooseObjectives[0] = 1;
    //     info->movedata->chooseObjectives[1] = 1;
    //     info->movedata->chooseObjectives[2] = 1;
    //     sendMove(info->movedata,info->moveresult);
    //     return;
    // }

    //init toplace
    To_Place** toplace = To_place_create(bord, info);

    //debug
    print_toplace(toplace);



    //cleanup
    for(int i=0; i<10; i++){
        if(toplace[i != NULL]){
            free(toplace[i]->path);
            free(toplace[i]);
        }
    }
    free(toplace);
}