#include "bot1.h"

/* todo: implement building roads using jokers*/
void bot_dumb1(Board* bord, Player_Info* info){
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

    for(int i=0; i<bord->gamedata->nbCities; i++){
        for(int j=0; j<bord->gamedata->nbCities; j++){
            // searches if a road can be placed
            if( bord->MatRoute[i][j].length > -1 &&
                bord->MatRoute[i][j].taken == -1 &&         
                (info->cards[bord->MatRoute[i][j].color] >= bord->MatRoute[i][j].length ||
                info->cards[bord->MatRoute[i][j].color2] >= bord->MatRoute[i][j].length)
                ){
                //place the found road

                info->movedata->action = 1;
                info->movedata->claimRoute.from = i;
                info->movedata->claimRoute.to = j;
                if(info->cards[bord->MatRoute[i][j].color2] >= bord->MatRoute[i][j].length){
                     info->movedata->claimRoute.color = bord->MatRoute[i][j].color2;
                }
                if(info->cards[bord->MatRoute[i][j].color] >= bord->MatRoute[i][j].length){
                    info->movedata->claimRoute.color = bord->MatRoute[i][j].color;
               }
                info->movedata->claimRoute.nbLocomotives = 0;
                sendMove(info->movedata,info->moveresult);
                update_board(bord, info);
                update_player_info(info, bord);

                return;
            }
        }
    }
    //if no road can be placed, pick a random card twice.
    info->movedata->action = 2;
    sendMove(info->movedata,info->moveresult);
    update_player_info(info, bord);
    sendMove(info->movedata,info->moveresult);
    update_player_info(info, bord);
    return;
}