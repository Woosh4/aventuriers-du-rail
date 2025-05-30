#include "bot3.h"
#include <stdio.h>

void bot_3(Board* bord, Player_Info* info){
    //beginning of the game
    //TODO IMPORTANT: have an array with all the colors needed to fill an objective ?, to pick cards effectively?

    //init toplace
    int replay = 0;
    To_Place** toplace = To_place_create(bord, info);


    //if no objectives left pick some more (not if gofast)
    if(toplace[0] == NULL){
        pick_new_objectives(toplace, info, bord);
        
        sendMove(info->movedata,info->moveresult);
        update_player_info(info, bord);
        destroy_toplace(toplace, info);
        return;
    }

    update_priority(bord, info, toplace);
    getBoardState(bord->cards_pickable);
    Action_order* action = search_color_pick_v2(bord, info, toplace, 0);
    replay = decode_action(info, action);

    //debug
    print_toplace(toplace);
    //

    sendMove(info->movedata, info->moveresult);
    update_board(bord, info);
    update_player_info(info, bord);
    getBoardState(bord->cards_pickable);

    if(replay){
        action = search_color_pick_v2(bord, info, toplace, 1);
        decode_action(info, action); // frees action

        //debug
        print_toplace(toplace);
        //

        sendMove(info->movedata, info->moveresult);
        update_board(bord, info);
        update_player_info(info, bord);
    }

    //cleanup
    destroy_toplace(toplace, info);
    return;
}