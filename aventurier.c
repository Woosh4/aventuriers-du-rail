#include <stdio.h>
#include <stdlib.h>
#include "aventurier.h"


/* retourne 0 si c'est à moi de jouer, 1 l'autre joueur, 2 si partie finie
quand : 0: player to play after, 1: bot to play after, 2:start, 3 player played before, 4 bot played before

nop faire matrice de machine à état*/

int a_qui(MoveData* mymove, MoveData* opponent_move, GameData* mygamedata,int* quand){
    // start of the game
    printf("*quand = %d\n", *quand);
    if(*quand == 2){
        if(mygamedata->starter == 1){
            *quand = 1;
            return 1;
        } 
        else if(mygamedata->starter == 2){
            *quand = 0;
            return 0;
        }
    }
    // play again case
    if((*quand == 0) && (mymove->action == 4 || mymove->action == 2 || (mymove->action == 3 && mymove->drawCard != 9))){
        *quand = 1;
        return 0;
    }
    if((*quand == 1) && (opponent_move->action == 4 || opponent_move->action == 2 || (opponent_move->action == 3 && opponent_move->drawCard != 9))){
        *quand = 0;
        return 1;
    }
    // other's turn to play turn
    if(*quand == 0){
        *quand = 1;
        return 0;
    }
    if(*quand == 1){
        *quand = 0;
        return 1;
    }
    return -1;
}

/* todo : init matrice distance -1, reste init,
update mat : pour savoir a qui appartient la route*/
int main(){
    extern int DEBUG_LEVEL;
    DEBUG_LEVEL = INTERN_DEBUG;

    route** mat_route = allouer_matrice_route(36);
    CardColor* tab_cards = allouertab(10);
    CardColor* tab_cards_adv = allouertab(10);
    player_info* player_info_p1 = init_player_info();
    player_info* player_info_p2 = init_player_info();

    int quand = 2;

    MoveData mymove;
    MoveData opponent_move;
    MoveResult mymoveresult;
    MoveResult opponent_moveresult;

    GameData mygamedata;
    GameSettings MySettings = GameSettingsDefaults;
    MySettings.gameType = TRAINING;
    MySettings.starter = 0;
    MySettings.reconnect = 0;

    printf("init OK\n");

    int connect = connectToCGS("cgs.valentin-lelievre.com", 15001);
    printf("connected? : code %d\n", connect);
    sendName("Alexisv21");
    printf("Name sent.\n");
    sendGameSettings(MySettings, &mygamedata);
    printf("Game settings sent\n");

    printf("gamedata.starter = %d\n", mygamedata.starter);

    convert_tab_matrice(mat_route,mygamedata.trackData,78);
    init_tab_cards(tab_cards, &mygamedata);

    /* jeu bot*/
    while(1){
        //if other player starts
        if(quand == 2 && mygamedata.starter == 1){
            getMove(&opponent_move,&opponent_moveresult);
            update_mat(mat_route, &opponent_move);
            update_player_info(player_info_p2, &opponent_move, &opponent_moveresult, mat_route);
            if(opponent_move.action == 4 || opponent_move.action == 2 || (opponent_move.action == 3 && opponent_move.drawCard != 9)){
            getMove(&opponent_move,&opponent_moveresult);
            update_mat(mat_route,&opponent_move);
            update_player_info(player_info_p2, &opponent_move, &opponent_moveresult, mat_route);
            }
        }
        //main loop body
        printBoard();
        bot_dumb1(mat_route,&mymove, tab_cards,&mymoveresult,36,&quand);

        getMove(&opponent_move,&opponent_moveresult);
        update_mat(mat_route, &opponent_move);
        update_player_info(player_info_p1, &opponent_move, &opponent_moveresult, mat_route);
        if(opponent_move.action == 4 || opponent_move.action == 2 || (opponent_move.action == 3 && opponent_move.drawCard != 9)){
            getMove(&opponent_move,&opponent_moveresult);
            update_mat(mat_route,&opponent_move);
            update_player_info(player_info_p1, &opponent_move, &opponent_moveresult, mat_route);
        }
    }

    /* jeu manuel*/
    while(1){
        printBoard();
        //print_tab(tab_cards,10);
        
        printf("a qui ? : %d\n", a_qui(&mymove, &opponent_move, &mygamedata, &quand));
        select_move_manuel(&mymove);
        update_mat(mat_route,&mymove);
        sendMove(&mymove,&mymoveresult);
        if(mymove.action == 4 || mymove.action == 2 || (mymove.action == 3 && mymove.drawCard != 9  )){
            printf("a qui ? : %d\n", a_qui(&mymove, &opponent_move, &mygamedata, &quand));
            select_move_manuel(&mymove);
            update_mat(mat_route,&mymove);
            sendMove(&mymove,&mymoveresult);
        }
        printf("a qui ? : %d\n", a_qui(&mymove, &opponent_move, &mygamedata, &quand));
        getMove(&opponent_move,&opponent_moveresult);
        update_mat(mat_route,&opponent_move);
        if(opponent_move.action == 4 || opponent_move.action == 2 || (opponent_move.action == 3 && opponent_move.drawCard != 9)){
            printf("a qui ? : %d\n", a_qui(&mymove, &opponent_move, &mygamedata, &quand));
            getMove(&opponent_move,&opponent_moveresult);
            update_mat(mat_route,&opponent_move);
        }
    }

    destroy_matrice_route(mat_route,36);
    destroy_tab(tab_cards);
    destroy_tab(tab_cards_adv);
    destroy_player_info(player_info_p1);
    destroy_player_info(player_info_p2);
    return 0;
}