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

    int NBGAMES = 1; //number of games to play for the loop
    int PRINT_WINRATE_FILE = 1; // to generate a file with : seed of current game, nbgames won, lost, winrate.
    int PRINT_INFO = 1; // toggles debug prints
    int TOURNAMENT_TEST = 0;

    int win_cpt = 0; // needed by PRINT_WINRATE_FILE
    int lost_cpt = 0; // needed by PRINT_WINRATE_FILE
    if(!PRINT_INFO) DEBUG_LEVEL = NO_DEBUG;

    //file to extract the objectives and their points
    // FILE* file_objectives = fopen("objectives.txt","w");
    // fprintf(file_objectives, "City1: City2: Points: \n");

    Board* board = alloc_board();

    if(PRINT_INFO) printf("Allocs OK\n");

    // connect v2
    int connect = connectToCGS("82.29.170.160", 15001, "Alexisv45");
    if(PRINT_INFO) printf("connected? : code %d\n", connect);

    //LOOP TO PLAY MULTIPLE GAMES
    for(int nbgame=0; nbgame<NBGAMES; nbgame++){

    if(!TOURNAMENT_TEST) sendGameSettings("TRAINING NICE_BOT", board->gamedata);
    else sendGameSettings("TOURNAMENT Test", board->gamedata);
    //ip 82.29.170.160 web:8889
    //sendGameSettings("TRAINING PLAY_RANDOM", board->gamedata);
    //sendGameSettings("TRAINING DO_NOTHING", board->gamedata);
    //créer le tournoi sur le web
    //sendGameSettings("TRAINING NICE_BOT seed=16717173 start=0 delay=0" map=small, board->gamedata);
    if(PRINT_INFO) printf("Game settings sent\n");

    if(PRINT_WINRATE_FILE){
        // store the seed in winrate.txt : useful for debug. will add win/loss/winrate at the end of the game
        FILE* file_winrate = fopen("winrate.txt", "w");
        if(win_cpt||lost_cpt) fprintf(file_winrate, "SEED: %d, WON: %d, LOST:%d, WINRATE: %f %%",board->gamedata->gameSeed, win_cpt, lost_cpt, 100*(float)win_cpt/(float)(win_cpt+lost_cpt));
        else fprintf(file_winrate, "SEED: %d, WON: %d, LOST:%d",board->gamedata->gameSeed, win_cpt, lost_cpt);
        fclose(file_winrate); // close and reopen to keep the seed in case of seg fault
    }

    init_board(board);
    Player_Info* info_p0 = init_player_info(0);
    Player_Info* info_p1 = init_player_info(1);

    init_tab_cards(info_p0, board);
 
    if(PRINT_INFO) printf("init OK");

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
        if(PRINT_INFO) printBoard();
        // bot_dumb1(board, info_p0);
        bot_2(board, info_p0);
        if(info_p0->moveresult->state) break;

        getMove(info_p1->movedata, info_p1->moveresult);
        if(info_p1->moveresult->state) break;
        update_board(board, info_p1);
        update_player_info(info_p1, board);
        if(info_p1->movedata->action == 4 || info_p1->movedata->action == 2 || (info_p1->movedata->action == 3 && info_p1->movedata->drawCard != 9)){
            getMove(info_p1->movedata, info_p1->moveresult);
            if(info_p1->moveresult->state) break;
            update_board(board, info_p1);
            update_player_info(info_p1, board);
        }
    }

    /* jeu manuel*/
    // while(1){
    //     printBoard();
    //     //print_tab(tab_cards,10);
        
    //     //printf("a qui ? : %d\n", a_qui(&mymove, &opponent_move, &mygamedata, &quand));
    //     select_move_manuel(info_p0);
    //     update_board(board, info_p0);
    //     update_player_info(info_p0, board);
    //     sendMove(info_p0->movedata,info_p0->moveresult);
    //     if(info_p0->movedata->action == 4 || info_p0->movedata->action == 2 || (info_p0->movedata->action == 3 && info_p0->movedata->drawCard != 9  )){
    //         //printf("a qui ? : %d\n", a_qui(&mymove, &opponent_move, &mygamedata, &quand));
    //         getBoardState(board->cards_pickable);
    //         select_move_manuel(info_p0);
    //         update_board(board, info_p0);
    //         update_player_info(info_p0, board);
    //         sendMove(info_p0->movedata,info_p0->moveresult);
    //     }
    //     //printf("a qui ? : %d\n", a_qui(&mymove, &opponent_move, &mygamedata, &quand));
    //     getMove(info_p1->movedata, info_p1->moveresult);
    //     update_board(board, info_p1);
    //     update_player_info(info_p1, board);
    //     if(info_p1->movedata->action == 4 || info_p1->movedata->action == 2 || (info_p1->movedata->action == 3 && info_p1->movedata->drawCard != 9)){
    //         //printf("a qui ? : %d\n", a_qui(&mymove, &opponent_move, &mygamedata, &quand));
    //         getMove(info_p1->movedata, info_p1->moveresult);
    //         update_board(board, info_p1);
    //         update_player_info(info_p1, board);
    //     }
    // }

    if(PRINT_WINRATE_FILE){ // keeps track of games won/lost
        if(info_p0->moveresult->state != 0){
            if(info_p0->moveresult->state == 1) win_cpt++;
            else lost_cpt++;
        }
        else if(info_p1->moveresult->state != 0){
            if(info_p1->moveresult->state == 1) lost_cpt++;
            else win_cpt++;
        }
    }

    destroy_player_info(info_p0);
    destroy_player_info(info_p1);

    if(PRINT_WINRATE_FILE){
        FILE * file_winrate = fopen("winrate.txt", "w");
        fprintf(file_winrate, "SEED: %d, WON: %d, LOST:%d, WINRATE: %f %%",board->gamedata->gameSeed, win_cpt, lost_cpt, 100*(float)win_cpt/(float)(win_cpt+lost_cpt));
        fclose(file_winrate);
    }

    } //end of the loop to play multiple games

    int quit = quitGame();
    if(PRINT_INFO) printf("Quit Game : code %d\n",quit);

    destroy_board(board);
    
    return 0;
}