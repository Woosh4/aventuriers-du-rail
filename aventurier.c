#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aventurier.h"

/* todo :
problem? : can only update board after move has been done
needs amount of wagons remaining, for bot1, others
update all*/
int main(){
    extern int DEBUG_LEVEL;
    DEBUG_LEVEL = INTERN_DEBUG;

    int NBGAMES = 1000; //number of games to play for the loop
    int PRINT_WINRATE_FILE = 1; // to generate a file with : seed of current game, nbgames won, lost, winrate.
    int PRINT_INFO = 1; // toggles debug prints (only for server prints : they slow down the program the most)
    int TOURNAMENT_TEST = 0; // autoconnects to tournament Test, used to play against other players

    int win_cpt = 0; // needed by PRINT_WINRATE_FILE, counter of wins
    int lost_cpt = 0; // needed by PRINT_WINRATE_FILE
    if(!PRINT_INFO) DEBUG_LEVEL = NO_DEBUG;

    //file to extract the objectives and their points
    // FILE* file_objectives = fopen("objectives.txt","w");
    // fprintf(file_objectives, "City1: City2: Points: \n");

    Board* board = alloc_board();

    if(PRINT_INFO) printf("Allocs OK\n");

    // connect v2
    int connect = connectToCGS("82.29.170.160", 15001, "Le_Alexcouille_2000");
    if(PRINT_INFO) printf("connected? : code %d\n", connect);

    //LOOP TO PLAY MULTIPLE GAMES
    for(int nbgame=0; nbgame<NBGAMES; nbgame++){


    if(TOURNAMENT_TEST) sendGameSettings("TOURNAMENT Test", board->gamedata);
    else sendGameSettings("TRAINING NICE_BOT", board->gamedata);
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
 
    if(PRINT_INFO) printf("init OK\n");
    if(PRINT_INFO) printf("Seed = %d\n", board->gamedata->gameSeed);

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
        bot_3(board, info_p0, info_p1);
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