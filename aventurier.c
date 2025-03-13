#include <stdio.h>
#include <stdlib.h>
#include "ticketToRide.h"

/*GameData* gamedata = {
    //nom partie
    "partie Alexis";
    //game seed
    1234;
    // starter 1 ou 2
    2;
    //nb cities
    20;
    //nb tracks
    40;


};*/

void print_gamedata(GameData data){
    printf("game name : %s\n",data.gameName);
    printf("game seed : %d\n",data.gameSeed);
    printf("starter : 0 you, 1 other player : %d\n",data.starter);
    printf("number of cities : %d\n",data.nbCities);
    printf("nombre de voies : %d\n",data.nbTracks);
    printf("number of cards : Nan\n");
    printf("trackdata : Nan\n");
    printf("cardscolor : Nan\n");
}

int main(){
    extern int DEBUG_LEVEL;
    DEBUG_LEVEL = MESSAGE;  
    int* envoi;
    MoveData mymove;
    MoveResult mymoveresult;
    GameData mygamedata;
    GameSettings MySettings = GameSettingsDefaults;
    MySettings.gameType = TRAINING;
    MySettings.reconnect = 0;

    printf("init OK\n");

    int connect = connectToCGS("cgs.valentin-lelievre.com", 15001);
    printf("connected? : code %d\n", connect);
    sendName("Alexisv10");
    printf("Name sent.\n");
    sendGameSettings(MySettings, &mygamedata);
    printf("Game settings sent");

    

    while(1){
        print_gamedata(mygamedata);
        printBoard();
        mymove.action = scanf("move: CLAIM_ROUTE:1, DRAW_BLIND_CARD:2, DRAW_CARD:3, DRAW_OBJECTIVES:4, CHOOSE_OBJECTIVES:5\n");

        sendMove(&mymove,&mymoveresult);
    }

    sendMessage("fin");

    return 0;
}