#include <stdio.h>
#include <stdlib.h>
#include "ticketToRide.h"
#include "aventurier.h"

/* alloue un tableau de nbrail * 5 int 
not ok*/
int** allouer_matrice_route(int nbcity){
    int** tab;
    tab = malloc(nbcity * sizeof(route*));

    for(int i=0;i<nbcity;i++){
        tab[i] = malloc(nbcity * sizeof(route));
    }
return tab;
}

/* destroy matrice of size nbrail*5
not ok*/
void destroy_matricerail(int** tab,int nbrail){
    for(int i=0;i<nbrail;i++){
        free(tab[i]);
    }
    free(tab);
    return;
}

/* print matrice size nbrail*5 
not ok*/
void print_matricerail(int** tab, int nbrail){
    for(int i=0;i<nbrail;i++){
        printf("town 1 :%d ",tab[i][0]);
        printf("town 2 :%d ",tab[i][1]);
        printf("length : %d",tab[i][2]);
        printf("color 1 : %d",tab[i][3]);
        printf("color 2 : %d\n",tab[i][4]);
    }
    return;

}
/* fills a matrice size nbrail by 5 with values from an array of size (5*nbrail) 
not ok*/
void convert_tab_matrice(int** mat, int* tab, int nbrail){
    for(int i=0;i<nbrail*5;i=i+5){
        mat[i][0] = tab[i];
        mat[i][1] = tab[i+1];
        mat[i][2] = tab[i+2];
        mat[i][3] = tab[i+3];
        mat[i][4] = tab[i+4];
    }
}
/* alloue tableau de int de taille len*/
int* allouertab(int len){
    int* tab = malloc(len * sizeof(int));
    return tab;
}
/* affiche tableau de int taille len*/
void print_tab(int* tab, int len){
    for(int i=0;i<len;i++){
        printf("Valeur %d : %d\n",i,tab[i]);
    }
    return;
}
/* détruit tableau*/
void destroy_tab(int* tab){
    free(tab);
    return;
}

/* permet de jouer avec le terminal avec des scanf*/
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
        printf("draw objectives : which ones ?\n 1 Yes | 0 No\nfirst :");
        scanf("%d",&mymove->chooseObjectives[0]);
        printf("1 Yes | 0 No\nsecond :\n");
        scanf("%d",&mymove->chooseObjectives[1]);
        printf("1 Yes | 0 No\nthird:\n");
        scanf("%d",&mymove->chooseObjectives[2]);
    }
    return;
}

/* retourne 0 si c'est à moi de jouer, 1 l'autre joueur, 2 si partie finie
quand : 0=j'ai joué avant, 1= autre à joué avant
useless.. */
/*
int a_qui(MoveData* mymove, MoveData* opponent_move,int* quand){
    printf(" *quand = %d\n", *quand);
    if(mymove->action == 4){
        return 0;
    }
    else if(*quand == 1 && (mymove->action == 2 || (mymove->action == 3 && mymove->drawCard != 9))){
        *quand = 0;
        return 0;
    }

    else if(opponent_move->action == 4){
        return 1;
    }
    else if(*quand == 0 && (opponent_move->action == 2 || (opponent_move->action == 3) && opponent_move->drawCard != 9)){
        *quand = 1;
        return 1;
    }

    else if(*quand == 0){
        *quand = 1;
        return 1;
    }
    else if(*quand == 1){
        *quand = 0;
        return 0;
    }
    else return -1;
}*/

int main(){
    extern int DEBUG_LEVEL;
    DEBUG_LEVEL = INTERN_DEBUG;  
    int* envoi;
    int** mat_rail = allouer_matrice_route(36);
    int* colors = allouertab(5);
    int quand;

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
    sendName("Alexisv16");
    printf("Name sent.\n");
    sendGameSettings(MySettings, &mygamedata);
    printf("Game settings sent");

    printf("gamedata.starter = %d\n", mygamedata.starter);

    /* setup pour fonction aqui */
    if(mygamedata.starter == 1) quand = 0;
    else if (mygamedata.starter == 2) quand = 1;

    while(1){
        printBoard();
        
        select_move_manuel(&mymove);
        sendMove(&mymove,&mymoveresult);
        if((mymove.action == 4 || mymove.action == 2 || mymove.action == 3) && mymove.drawCard != 9  ){
            //printf("*****\n à qui : %d\n******\n",a_qui(&mymove,&opponent_move,&quand));
            select_move_manuel(&mymove);
            sendMove(&mymove,&mymoveresult);
        }
        //printf("*****\n à qui : %d\n******\n",a_qui(&mymove,&opponent_move,&quand));
        getMove(&opponent_move,&opponent_moveresult);
        if((opponent_move.action == 4 || opponent_move.action == 2 || opponent_move.action == 3) && opponent_move.drawCard != 8){
            //printf("*****\n à qui : %d\n******\n",a_qui(&mymove,&opponent_move,&quand));
            getMove(&opponent_move,&opponent_moveresult);
        }
    }

    destroy_matricerail(mat_rail,78);

    return 0;
}