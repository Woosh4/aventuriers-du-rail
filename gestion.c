#include "gestion.h"
#include <stdio.h>
#include <stdlib.h>


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
            printf("(From:%d To:%d length:%d Color1: %d Color2:%d taken:%d)  ", i, j, (mat[i][j]).length, mat[i][j].color, mat[i][j].color2 ,(mat[i][j]).taken);
            }
            printf("]\n");
        }
    return;

}

/* converts data from the array to the matrix
from to length color1 color2 */
void convert_tab_matrice(Board* bord){
    for(int j=0; j<(bord->gamedata->nbCities); j++){
        for(int k=0; k<(bord->gamedata->nbCities); k++){
            bord->MatRoute[j][k].length = -1;
            bord->MatRoute[j][k].taken = -1;
            bord->MatRoute[j][k].color = -1;
            bord->MatRoute[j][k].color2 = -1;
        }
    }

    for(int i=0;i<(bord->gamedata->nbTracks)*5;i=i+5){
        bord->MatRoute[bord->gamedata->trackData[i]][bord->gamedata->trackData[i+1]].length = bord->gamedata->trackData[i+2];
        bord->MatRoute[bord->gamedata->trackData[i]][bord->gamedata->trackData[i+1]].color = bord->gamedata->trackData[i+3];
        bord->MatRoute[bord->gamedata->trackData[i]][bord->gamedata->trackData[i+1]].color2 = bord->gamedata->trackData[i+4];

        bord->MatRoute[bord->gamedata->trackData[i+1]][bord->gamedata->trackData[i]].length = bord->gamedata->trackData[i+2];
        bord->MatRoute[bord->gamedata->trackData[i+1]][bord->gamedata->trackData[i]].color = bord->gamedata->trackData[i+3];
        bord->MatRoute[bord->gamedata->trackData[i+1]][bord->gamedata->trackData[i]].color2 = bord->gamedata->trackData[i+4];
    }
    return;
}
/* updates the matrix to know wich route has been taken.
will need move_result to validate*/
void update_mat(Board* bord, Player_Info* info){
    if(info->movedata->action == 1){
        bord->MatRoute[info->movedata->claimRoute.from][info->movedata->claimRoute.to].taken = info->player_number;
        bord->MatRoute[info->movedata->claimRoute.to][info->movedata->claimRoute.from].taken = info->player_number;
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
void init_tab_cards(Player_Info* info, Board* bord){
    for(int i=0; i<10; i++){
        info->cards[i] = 0;
    }

    if(info->player_number == 0){
        for(int j=0; j<4; j++){
            info->cards[bord->gamedata->cards[j]] = info->cards[bord->gamedata->cards[j]] + 1;
        }
    }
    return;
}

/* détruit tableau*/
void destroy_tab(void* tab){
    free(tab);
    return;
}

/* to call whenever*/
Player_Info* init_player_info(int playernumber){
    Player_Info* info = malloc(sizeof(Player_Info));
    info->movedata = malloc(sizeof(MoveData));
    info->moveresult = malloc(sizeof(MoveResult));
    info->score = 0;
    info->nbwagons = 45;
    info->nbcards = 5;
    info->cards = malloc(10*sizeof(CardColor));
    for(int i=0; i<10; i++) info->cards[i] = 0;
    info->nbobjective = 0;
    info->objective = malloc(20*sizeof(Objective));
    info->player_number = playernumber;
}

void destroy_player_info(Player_Info* info){
    free(info->cards);
    free(info->movedata);
    free(info->moveresult);
    free(info->objective);
    free(info);
    return;
}
/* you NEED to update the route matrix before calling this,
updates all fields except movedata, movedata needs to be the correct one for this turn
will need updating for jokers */
void update_player_info(Player_Info* info, Board* bord){
    // route is built
    switch(info->movedata->action){
        case 1:
        switch(bord->MatRoute[info->movedata->claimRoute.from][info->movedata->claimRoute.to].length){
            case 1:
            info->score += 1;
            break;

            case 2:
            info->score += 2;
            break;

            case 3:
            info->score += 4;
            break;

            case 4:
            info->score += 7;
            break;

            case 5:
            info->score += 10;
            break;

            case 6:
            info->score += 15;
            break;
        }

        info->nbwagons -= bord->MatRoute[info->movedata->claimRoute.from][info->movedata->claimRoute.to].length;
        info->nbcards -= bord->MatRoute[info->movedata->claimRoute.from][info->movedata->claimRoute.to].length;
        info->cards[info->movedata->claimRoute.color] -= bord->MatRoute[info->movedata->claimRoute.from][info->movedata->claimRoute.to].length;
        return;
        break;
    
    case 2:
        info->nbcards ++;
        if(info->player_number == 0){
            info->cards[info->moveresult->card]++;
        }
        return;
        break;

    case 3:
        info->nbcards++;
        if(info->player_number == 0){
            info->cards[info->moveresult->card]++;
        }
        return;
        break;
     
    case 5:
        for(int i=0; i<3; i++){
            if(info->movedata->chooseObjectives[i] == 1){
                info->nbobjective++;
                if(info->player_number == 0){
                    info->objective[info->nbobjective].from = info->moveresult->objectives[i].from;
                    info->objective[info->nbobjective].to = info->moveresult->objectives[i].to;
                    info->objective[info->nbobjective].score = info->moveresult->objectives[i].score;
                }
            }
        }
        return;
        break;
    }
    return;
}
/* alloc board, to be called at the very beginning*/
Board* alloc_board(){
    Board* bord = malloc(sizeof(Board));
    bord->cards_pickable = malloc(5*sizeof(CardColor));
    bord->gamedata = (GameData*)malloc(sizeof(GameData));
    bord->when = -1;
    return bord;
}
/* init board, to call after alloc_board and sendgamesettings.*/
void init_board(Board* bord){
    getBoardState(bord->cards_pickable);
    bord->MatRoute = allouer_matrice_route(bord->gamedata->nbCities);
    convert_tab_matrice(bord);
}

void destroy_board(Board* bord){
    free(bord->MatRoute);
    free(bord->cards_pickable);
    free(bord->gamedata);
    free(bord);
}
/* updates pickable cards and mat route,
update 'when' with a function ?*/
void update_board(Board* bord, Player_Info* info){
    getBoardState(bord->cards_pickable);
    update_mat(bord, info);
}

/* finds the city with the minimum weight that has not been visited yet*/
int find_min(Dijkstra_City* dijk, int nbcity){
    int min = __INT_MAX__;
    int index = -1;
    for(int i=0; i<nbcity; i++){
        if(dijk[i].checked == 0 && dijk[i].weight < min){
            min = dijk[i].weight;
            index = i;
        }
    }
    return index;
}

/* checks if the array is filled with only 0s*/
int all_checked(Dijkstra_City* array, Board* bord){
    int blocked;
    for(int i=0; i<bord->gamedata->nbCities; i++){
        if(array[i].checked == 0){
            blocked = 1;
            for(int j=0; j<bord->gamedata->nbCities; j++){
                if(bord->MatRoute[i][j].taken < 1 && bord->MatRoute[i][j].length > 0) blocked = 0;
            }
            if(blocked) array[i].checked = 1;
            else return 0;
        }
    }
    return 1;
}

/* updates the distance of city2 if the route through city1 is shorter*/
void update_weight(Dijkstra_City* dijk, int city1, int city2, Board* bord){
    //route taken by player
    if(bord->MatRoute[city1][city2].taken == 0){
        if(dijk[city2].weight > dijk[city1].weight) dijk[city2].weight = dijk[city1].weight;
        return;
    }
    //route taken by opponent
    else if(bord->MatRoute[city1][city2].taken > 0){
        return;
    }
    //route is free to take
    if(dijk[city2].weight > (dijk[city1].weight + bord->MatRoute[city1][city2].length)){
        dijk[city2].weight = dijk[city1].weight + bord->MatRoute[city1][city2].length;
        dijk[city2].prev = city1;
    }
    return;
}

/* dikstra to find the shortest path between city1 and city2*/
To_Place* shortest(Board* bord, int city1, int city2){
    int city;
    To_Place* toplace = malloc(sizeof(To_Place));
    toplace->city1 = city1;
    toplace->city2 = city2;

    //init checked + weight
    Dijkstra_City* dijkstra = malloc(bord->gamedata->nbCities * sizeof(Dijkstra_City));
    for(int i=0; i<bord->gamedata->nbCities; i++){
         dijkstra[i].checked = 0;
         dijkstra[i].weight = __INT_MAX__;
    }
    dijkstra[city1].weight = 0;

    //main loop
    // enlever après && pour parcourir tout
    while(!(all_checked(dijkstra, bord)) && dijkstra[city2].checked == 0){
        city = find_min(dijkstra, bord->gamedata->nbCities);
        dijkstra[city].checked = 1;
        for(int i=0; i<bord->gamedata->nbCities; i++){
            if(bord->MatRoute[city][i].length > 0){
                update_weight(dijkstra, city, i, bord);
            }
        }
    }

    // print debug
    for(int i=0; i<bord->gamedata->nbCities; i++){
        printf("Tab dijkstra : city:%d checked=%d, weight=%d, prev=%d\n", i, dijkstra[i].checked, dijkstra[i].weight, dijkstra[i].prev);
    }
    free(dijkstra);
}