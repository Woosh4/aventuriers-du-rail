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

/* alloue un tableau de nbrail * 5 int */
int** allouer_matricerail(int nbrail){
    int** tab;
    tab = malloc(nbrail * sizeof(int*));

    for(int i=0;i<nbrail;i++){
        tab[i] = malloc(5 * sizeof(int));
    }
return tab;
}
/* destroy matrice of size nbrail*5*/
void destroy_matricerail(int** tab,int nbrail){
    for(int i=0;i<nbrail;i++){
        free(tab[i]);
    }
    free(tab);
    return;
}
/* print matrice size nbrail*5 */
void print_matricerail(int** tab, int nbrail){
    for(int i=0;i<nbrail;i++){
        printf("town 1 :%d ",tab[i][0]);
        printf("town 2 :%d ",tab[i][1]);
        printf("lenght : %d",tab[i][2]);
        printf("color 1 : %d",tab[i][3]);
        printf("color 2 : %d\n",tab[i][4]);
    }
    return;
}
/* fills a matrice size nbrail by 5 with values from an array of size (5*nbrail) */
void convert_tab_matrice(int** mat, int* tab, int nbrail){
    for(int i=0;i<nbrail*5;i=i+5){
        mat[i][0] = tab[i];
        mat[i][1] = tab[i+1];
        mat[i][2] = tab[i+2];
        mat[i][3] = tab[i+3];
        mat[i][4] = tab[i+4];
    }
}

int* allouertab(int len){
    int* tab = malloc(len * sizeof(int));
    return tab;
}

void print_tab(int* tab, int len){
    for(int i=0;i<len;i++){
        printf("Valeur %d : %d\n",i,tab[i]);
    }
    return;
}

void destroy_tab(int* tab){
    free(tab);
    return;
}

void print_gamedata(GameData data){
    printf("game name : %s\n",data.gameName);
    printf("game seed : %d\n",data.gameSeed);
    printf("starter : 0 you, 1 other player : %d\n",data.starter);
    printf("number of cities : %d\n",data.nbCities);
    printf("nombre de voies : %d\n",data.nbTracks);
    //print_tab(data.trackData,78*5);
    //print_tab(data.cards,4);
}

void select_move_manuel(MoveData* mymove){
    ClaimRouteMove claim_route;

    printf("choose action :\n1 | claim route\n2 | draw blind card\n3 | draw card\n4 | draw objectives\n5 | choose objectives\n");
    scanf("%d",&mymove->action);
    if(mymove->action == 1){
        printf("choose route : from where ?\n");
        scanf("%d",&claim_route.from);
        printf("choose route : to where ?\n");
        scanf("%d",&claim_route.to);
        printf("choose route : what color ?\n");
        scanf("%d",&claim_route.color);
        printf("choose route : nb locomotives ?\n");
        scanf("%d",&claim_route.nbLocomotives);
        mymove->claimRoute = claim_route;
    }
    if(mymove->action == 3){
        printf("draw card : what color ?\n");
        scanf("%d",&mymove->drawCard);
    }
    if(mymove->action == 5){
        printf("draw objectives : which ones ?\n first :");
        scanf("%d",&mymove->chooseObjectives[0]);
        printf("second :\n");
        scanf("%d",&mymove->chooseObjectives[1]);
        printf("third:\n");
        scanf("%d",&mymove->chooseObjectives[2]);
    }
    return;
}






int main(){
    extern int DEBUG_LEVEL;
    DEBUG_LEVEL = INTERN_DEBUG;  
    int* envoi;
    int** mat_rail = allouer_matricerail(78);
    int* colors = allouertab(5);

    MoveData mymove;
    MoveData opponent_move;
    MoveResult mymoveresult;
    MoveResult opponent_moveresult;

    GameData mygamedata;
    GameSettings MySettings = GameSettingsDefaults;
    MySettings.gameType = TRAINING;
    MySettings.starter = 1;
    MySettings.reconnect = 0;

    printf("init OK\n");

    int connect = connectToCGS("cgs.valentin-lelievre.com", 15001);
    printf("connected? : code %d\n", connect);
    sendName("Alexisv10");
    printf("Name sent.\n");
    sendGameSettings(MySettings, &mygamedata);
    printf("Game settings sent");

    

    while(1){
        //print_gamedata(mygamedata);
        printBoard();

        //mymove.action = scanf("move: CLAIM_ROUTE:1, DRAW_BLIND_CARD:2, DRAW_CARD:3, DRAW_OBJECTIVES:4, CHOOSE_OBJECTIVES:5\n");
        
        select_move_manuel(&mymove);
        sendMove(&mymove,&mymoveresult);
        if(mymove.action == 4 || mymove.action == 2 || mymove.action == 3){
            select_move_manuel(&mymove);
            sendMove(&mymove,&mymoveresult);
        }
    
        getMove(&opponent_move,&opponent_moveresult);
        if(opponent_move.action == 4 || opponent_move.action == 2 || opponent_move.action == 3) getMove(&opponent_move,&opponent_moveresult);
    }


    destroy_matricerail(mat_rail,78);

    return 0;
}