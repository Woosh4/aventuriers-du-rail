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

/* todo : update when, machine à état pour les tours
problem? : can only update board after move has been done
needs amount of wagons remaining, for bot1, others
update all*/
int main(){
    extern int DEBUG_LEVEL;
    DEBUG_LEVEL = INTERN_DEBUG;

    Board* board = alloc_board();
    board->gamesettings.gameType = TRAINING;

    /* alloc info player ?*/

    printf("Allocs OK\n");

    int connect = connectToCGS("cgs.valentin-lelievre.com", 15001);
    printf("connected? : code %d\n", connect);
    sendName("Alexisv36");
    printf("Name sent.\n");
    sendGameSettings(board->gamesettings, board->gamedata);
    printf("Game settings sent\n");

    init_board(board);
    Player_Info* info_p0 = init_player_info(0);
    Player_Info* info_p1 = init_player_info(1);

    print_tab(info_p0->cards, 10);
    print_matrice_route(board->MatRoute,board->gamedata->nbCities);
    init_tab_cards(info_p0, board);


    printf("init finished");


    /* jeu bot*/
    while(1){
        //if other player starts
        if(board->when == -1 && board->gamedata->starter == 1){
            getMove(info_p1->movedata,info_p1->moveresult);
            update_board(board, info_p1);
            update_player_info(info_p1, board);
            if(info_p1->movedata->action == 4 ||
            info_p1->movedata->action == 2 ||
            (info_p1->movedata->action == 3 && info_p1->movedata->drawCard != 9))
            {
                getMove(info_p1->movedata,info_p1->moveresult);
                update_board(board, info_p1);
                update_player_info(info_p1, board);
            }
        }
        //main loop body
        printBoard();
        bot_dumb1(board, info_p0);

        getMove(info_p1->movedata, info_p1->moveresult);
        update_board(board, info_p1);
        update_player_info(info_p1, board);
        if(info_p1->movedata->action == 4 || info_p1->movedata->action == 2 || (info_p1->movedata->action == 3 && info_p1->movedata->drawCard != 9)){
            getMove(info_p1->movedata, info_p1->moveresult);
            update_board(board, info_p1);
            update_player_info(info_p1, board);
        }
    }

    /* jeu manuel*/
    while(1){
        printBoard();
        //print_tab(tab_cards,10);
        
        //printf("a qui ? : %d\n", a_qui(&mymove, &opponent_move, &mygamedata, &quand));
        select_move_manuel(info_p0);
        update_board(board, info_p0);
        update_player_info(info_p0, board);
        sendMove(info_p0->movedata,info_p0->moveresult);
        if(info_p0->movedata->action == 4 || info_p0->movedata->action == 2 || (info_p0->movedata->action == 3 && info_p0->movedata->drawCard != 9  )){
            //printf("a qui ? : %d\n", a_qui(&mymove, &opponent_move, &mygamedata, &quand));
            select_move_manuel(info_p0);
            update_board(board, info_p0);
            update_player_info(info_p0, board);
            sendMove(info_p0->movedata,info_p0->moveresult);
        }
        //printf("a qui ? : %d\n", a_qui(&mymove, &opponent_move, &mygamedata, &quand));
        getMove(info_p1->movedata, info_p1->moveresult);
        update_board(board, info_p1);
        update_player_info(info_p1, board);
        if(info_p1->movedata->action == 4 || info_p1->movedata->action == 2 || (info_p1->movedata->action == 3 && info_p1->movedata->drawCard != 9)){
            //printf("a qui ? : %d\n", a_qui(&mymove, &opponent_move, &mygamedata, &quand));
            getMove(info_p1->movedata, info_p1->moveresult);
            update_board(board, info_p1);
            update_player_info(info_p1, board);
        }
    }

    destroy_board(board);
    destroy_player_info(info_p0);
    destroy_player_info(info_p1);
    return 0;
}