#include "bot1.h"

void bot_dumb1(route** mat, MoveData* mymove, CardColor* tab_color, MoveResult* moveresult, int nbcity, int* debut){
    if(*debut == 2){
        *debut = 0;
        mymove->action = 4;
        sendMove(mymove, moveresult);
        mymove->action = 5;
        mymove->chooseObjectives[0] = 1;
        mymove->chooseObjectives[1] = 1;
        mymove->chooseObjectives[2] = 1;
        sendMove(mymove,moveresult);
        return;
    }

    for(int i=0; i<nbcity; i++){
        for(int j=0; j<nbcity; j++){
            // searches if a road can be placed
            if((mat[i][j].from!=0 ||
                mat[i][j].to!=0) &&
                mat[i][j].taken==0 &&
                mat[i][j].length>0 &&
                (tab_color[mat[i][j].color] >= mat[i][j].length || tab_color[mat[i][j].color2] >= mat[i][j].length)
                ){
                //place the found road
                mat[i][j].taken = 1;
                mat[j][i].taken = 1;

                mymove->action = 1;
                mymove->claimRoute.from = i;
                mymove->claimRoute.to = j;
                if(tab_color[mat[i][j].color2] >= mat[i][j].length){
                     mymove->claimRoute.color = mat[i][j].color2;
                     tab_color[mat[i][j].color2] -= mat[i][j].length;
                }
                if(tab_color[mat[i][j].color] >= mat[i][j].length){
                    mymove->claimRoute.color = mat[i][j].color;
                    tab_color[mat[i][j].color] -= mat[i][j].length;
                }
                mymove->claimRoute.nbLocomotives = 0;
                sendMove(mymove, moveresult);
                update_mat(mat, mymove);
                return;
            }
        }
    }
    //if no road can be placed, pick a random card twice.
    mymove->action = 2;
    sendMove(mymove,moveresult);
    update_tab_cards(tab_color,moveresult);
    sendMove(mymove,moveresult);
    update_tab_cards(tab_color,moveresult);
    return;
}