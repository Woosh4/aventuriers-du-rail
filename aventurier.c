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
    return;
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
void* allouertab(int len){
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
void destroy_tab(void* tab){
    free(tab);
    return;
}

/* permet de jouer avec le terminal avec des scanf*/
void select_move_manuel(MoveData* mymove){
    ClaimRouteMove claim_route;

    printf("choose action :\n1 | claim route\n2 | draw blind card\n3 | draw card\n4 | draw objectives\n5 | choose objectives\n");
    scanf("%d",(int*)&mymove->action);
    if(mymove->action == 1){
        printf("choose route : from where ?\n");
        scanf("%d",(int*)&claim_route.from);
        printf("choose route : to where ?\n");
        scanf("%d",(int*)&claim_route.to);
        printf("choose route : what color ?\n");
        scanf("%d",(int*)&claim_route.color);
        printf("choose route : nb locomotives ?\n");
        scanf("%d",(int*)&claim_route.nbLocomotives);
        mymove->claimRoute = claim_route;
    }
    if(mymove->action == 3){
        printf("draw card : what color ?\n");
        scanf("%d",(int*)&mymove->drawCard);
    }
    if(mymove->action == 5){
        printf("draw objectives : which ones ?\nN°1 : 1 Yes | 0 No\n");
        scanf("%d",(int*)&mymove->chooseObjectives[0]);
        printf("N°2 : 1 Yes | 0 No\n");
        scanf("%d",(int*)&mymove->chooseObjectives[1]);
        printf("N°3 : 1 Yes | 0 No\n");
        scanf("%d",(int*)&mymove->chooseObjectives[2]);
    }
    return;
}

/* besoin nombre de wagons*/
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

/* retourne 0 si c'est à moi de jouer, 1 l'autre joueur, 2 si partie finie
quand : 0: player to play after, 1: bot to play after, 2:start, 3 player played before, 4 bot played before*/

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

player_info* init_player_info(){
    player_info* info = malloc(sizeof(player_info*));
    info->score = 0;
    info->nbwagons = 45;
    info->nbcards = 5;
    info->cards = allouertab(10);
    info->nbobjective = 0;
}

void destroy_player_info(player_info* info){
    free(info->cards);
    free(info);
    return;
}
/* you NEED to update the route matrix before calling this,
card array not operational,
choose objective seems not to be seeable for other players*/
void update_player_info(player_info* info, MoveData* movedata, MoveResult* moveresult, route** mat_route){
    // route is built
    if(movedata->action == 1){
        if(mat_route[movedata->claimRoute.from][movedata->claimRoute.to].length == 1) info->score += 1;
        if(mat_route[movedata->claimRoute.from][movedata->claimRoute.to].length == 2) info->score += 2;
        if(mat_route[movedata->claimRoute.from][movedata->claimRoute.to].length == 3) info->score += 4;
        if(mat_route[movedata->claimRoute.from][movedata->claimRoute.to].length == 4) info->score += 7;
        if(mat_route[movedata->claimRoute.from][movedata->claimRoute.to].length == 5) info->score += 10;
        if(mat_route[movedata->claimRoute.from][movedata->claimRoute.to].length == 6) info->score += 15;
        info->nbwagons -= mat_route[movedata->claimRoute.from][movedata->claimRoute.to].length;
        info->nbcards -= mat_route[movedata->claimRoute.from][movedata->claimRoute.to].length;
        return;
    }
    if(movedata->action == 2){
        info->nbcards ++;
        return;
    }
    if(movedata->action == 3){
        info->nbcards++;
        info->cards[moveresult->card]++;
        return;
    }
    if(movedata->action == 5){
        for(int i=0; i<3; i++){
            if(movedata->chooseObjectives[i] == 1) info->nbobjective++;
        }
        return;
    }
    return;
}

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