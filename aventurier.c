#include <stdio.h>
#include <stdlib.h>
#include "ticketToRide.h"
#include "aventurier.h"

/* alloue un tableau de nbrail * 5 int */
route** allouer_matrice_route(int nbcity){
    route** tab;
    tab = malloc(nbcity * sizeof(route*));

    for(int i=0;i<nbcity;i++){
        tab[i] = malloc(nbcity * sizeof(route));
    }
return tab;
}

/* destroy matrice of size nbcity*X */
void destroy_matrice_route(route** mat,int nbcity){
    for(int i=0;i<nbcity;i++){
        free(mat[i]);
    }
    free(mat);
    return;
}

/* print matrice sized nbcity*nbcity */
void print_matrice_route(route** mat, int nbcity){
    for(int i=0;i<nbcity;i++){
        printf("[ ");
        for(int j=0; j<nbcity; j++){
            printf("(From:%d To:%d lenght:%d taken:%d)  ", (mat[i][j]).from, (mat[i][j]).to, (mat[i][j]).length, (mat[i][j]).taken);
            }
            printf("]\n");
        }
    return;

}
/* converts data from the array to the matrix
from to length color1 color2 */
void convert_tab_matrice(route** mat, int* tab, int nbrail){
    for(int i=0;i<nbrail*5;i=i+5){
        mat[tab[i]][tab[i+1]].from = tab[i];
        mat[tab[i]][tab[i+1]].to = tab[i+1];
        mat[tab[i]][tab[i+1]].length = tab[i+2];
        mat[tab[i]][tab[i+1]].color = tab[i+3];
        mat[tab[i]][tab[i+1]].color2 = tab[i+4];

        mat[tab[i+1]][tab[i]].from = tab[i];
        mat[tab[i+1]][tab[i]].to = tab[i+1];
        mat[tab[i+1]][tab[i]].length = tab[i+2];
        mat[tab[i+1]][tab[i]].color = tab[i+3];
        mat[tab[i+1]][tab[i]].color2 = tab[i+4];
    }
    return 0;
}
/* updates the matrix to know wich route has been taken.
will need move_result to validate*/
void update_mat(route** mat, MoveData* movedata){
    if(movedata->action == 1){
        mat[movedata->claimRoute.to][movedata->claimRoute.from].taken = 1;
        mat[movedata->claimRoute.from][movedata->claimRoute.to].taken = 1;
    }
    return;
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
/* initialises cards array with starting cards*/
void init_tab_cards(CardColor* tab, GameData* gamedata){
    for(int i=0; i<10; i++){
        tab[i] = 0;
    }

    for(int i=0; i<4; i++){
        tab[gamedata->cards[i]] ++;
    }
    return;
}
void update_tab_cards(CardColor* tab, MoveResult* move_result){
    printf("move result cards:\n%d", move_result->card);
    tab[move_result->card] = tab[move_result->card] + 1;
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

/* besoin inventaire de cartes, autre?*/
void bot_dumb1(route** mat, MoveData* mymove, int nbcity){
    for(int i=0; i<nbcity; i++){
        for(int j=0; j<nbcity; j++){
            if((mat[i][j].from!=0 || mat[i][j].to!=0) && mat[i][j].taken==0){ // && ... verif assez de cartes
                mat[i][j].taken = 1;
                mat[j][i].taken = 1;
                mymove->action = 1;
                // ...
            }
        }
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

    int** mat_route = allouer_matrice_route(36);
    CardColor* tab_cards = allouertab(10);
    CardColor* tab_cards_adv = allouertab(10);
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
    sendName("Alexisv17");
    printf("Name sent.\n");
    sendGameSettings(MySettings, &mygamedata);
    printf("Game settings sent\n");

    printf("gamedata.starter = %d\n", mygamedata.starter);

    convert_tab_matrice(mat_route,mygamedata.trackData,78);
    init_tab_cards(tab_cards, &mygamedata);

    /* setup pour fonction aqui */
    if(mygamedata.starter == 1) quand = 0;
    else if (mygamedata.starter == 2) quand = 1;

    while(1){
        printBoard();
        print_tab(tab_cards,10);
        
        select_move_manuel(&mymove);
        printf("update mat\n");
        update_mat(mat_route,&mymove);
        printf("update tab cards\n");
        sendMove(&mymove,&mymoveresult);
        if((mymove.action == 4 || mymove.action == 2 || mymove.action == 3) && mymove.drawCard != 9  ){
            //printf("*****\n à qui : %d\n******\n",a_qui(&mymove,&opponent_move,&quand));
            select_move_manuel(&mymove);
            update_mat(mat_route,&mymove);
            sendMove(&mymove,&mymoveresult);
        }
        //printf("*****\n à qui : %d\n******\n",a_qui(&mymove,&opponent_move,&quand));
        getMove(&opponent_move,&opponent_moveresult);
        update_mat(mat_route,&opponent_moveresult);
        if((opponent_move.action == 4 || opponent_move.action == 2 || opponent_move.action == 3) && opponent_move.drawCard != 8){
            //printf("*****\n à qui : %d\n******\n",a_qui(&mymove,&opponent_move,&quand));
            getMove(&opponent_move,&opponent_moveresult);
            update_mat(mat_route,&opponent_moveresult);
        }
    }

    destroy_matrice_route(mat_route,36);
    destroy_tab(tab_cards);
    destroy_tab(tab_cards_adv);
    return 0;
}