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
    info->objective = malloc(20*sizeof(Objective*));
    for(int i=0; i<20; i++) info->objective[i] = malloc(sizeof(Objective));
    info->player_number = playernumber;
    return info;
}

void destroy_player_info(Player_Info* info){
    free(info->cards);
    free(info->movedata);
    free(info->moveresult);
    for(int i=0; i<20; i++) free(info->objective[i]); // problem?
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
                if(info->player_number == 0){
                    info->objective[info->nbobjective]->from = info->moveresult->objectives[i].from;
                    info->objective[info->nbobjective]->to = info->moveresult->objectives[i].to;
                    info->objective[info->nbobjective]->score = info->moveresult->objectives[i].score;
                }
                info->nbobjective++;
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
/* updates pickable cards, and mat route using the move data in info.
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
            //if a route is surrounded by the opponent and unreachable
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
        if(dijk[city2].weight > dijk[city1].weight){
             dijk[city2].weight = dijk[city1].weight;
            dijk[city2].prev = city1;
            //if(dijk[city2].prev == -1) dijk[city2].prev = city1;
        }
        return;
    }
    //route taken by opponent
    else if(bord->MatRoute[city1][city2].taken > 0){
        return;
    }
    //route is free to take
    else if(dijk[city2].weight > (dijk[city1].weight + bord->MatRoute[city1][city2].length)){
        dijk[city2].weight = dijk[city1].weight + bord->MatRoute[city1][city2].length;
        dijk[city2].prev = city1;
    }
    return;
}

/* dikstra to find the shortest path between city1 and city2
// IMPORTANT: add anti blocked for city 0 (of origin?)
add how many roads are needed ? (for speed, optimising toplace* malloc space, building of the full road)*/
To_Place* shortest(Board* bord, int city1, int city2){
    int city;
    To_Place* toplace = malloc(sizeof(To_Place));
    toplace->path = malloc(bord->gamedata->nbCities * 2 * sizeof(int)); // to be changed : too much allocated
    toplace->city1 = city1;
    toplace->city2 = city2;
    toplace->nbwagons = 0;

    //init checked + weight
    Dijkstra_City* dijkstra = malloc(bord->gamedata->nbCities * sizeof(Dijkstra_City));
    for(int i=0; i<bord->gamedata->nbCities; i++){
         dijkstra[i].checked = 0;
         dijkstra[i].weight = __INT_MAX__;
         dijkstra[i].prev = -1;
    }
    dijkstra[city1].weight = 0;

    //main loop
    while(!(all_checked(dijkstra, bord)) && dijkstra[city2].checked == 0){
        city = find_min(dijkstra, bord->gamedata->nbCities);
        dijkstra[city].checked = 1;
        for(int i=0; i<bord->gamedata->nbCities; i++){
            if(bord->MatRoute[city][i].length > 0){
                update_weight(dijkstra, city, i, bord);
            }
        }
    }
    // IMPORTANT: add anti blocked for city n°0 (of origin?)
    // detect if weight of city2(destination) = -1

    //build the full road (from city2 to city1)
    city = city2;
    int i=0;
    toplace->nbwagons = dijkstra[city2].weight;
    while(dijkstra[city].prev != -1){
        if(bord->MatRoute[city][dijkstra[city].prev].taken == -1){ //only add unconnected cities
            toplace->path[i] = city;
            toplace->path[i+1] = dijkstra[city].prev;
            i = i+2;
        }
        city = dijkstra[city].prev;
    }
    toplace->path[i] = -1;
    toplace->path[i+1] = -1;

    //debug : print found weight, checked, and prev for each city (with weight < to city2's)
    // for(int i=0; i<bord->gamedata->nbCities; i++){
    //     printf("Tab dijkstra : city:%d checked=%d, weight=%d, prev=%d\n", i, dijkstra[i].checked, dijkstra[i].weight, dijkstra[i].prev);
    // }

    //debug : print the path found
    // i = 0;
    // printf("----------\nPATH FOUND DEBUG:\n----------\nConnecting %d to %d\n", city1, city2);
    // while(toplace->path[i+1] != city1){
    //     printf("from %d to %d\n", toplace->path[i], toplace->path[i+1]);
    //     i = i+2;
    // }
    
    free(dijkstra);
    return toplace;
}

void print_toplace(To_Place** toplace){
    int j;
    printf("----------\nDEBUG: PRINT TOPLACE\n----------\n");
    for(int i=0; i<10; i++){
        j = 0;
        if(toplace[i] != NULL){ // check if toplace[i] is null
        printf("FROM: %d TO: %d NBWAGONS: %d EV: %f PRIORITY: %d\n", toplace[i]->city1, toplace[i]->city2, toplace[i]->nbwagons, toplace[i]->ev, toplace[i]->priority);
            if(toplace[i]->path != NULL){
                while(toplace[i]->path[j+1] != -1){
                    printf("from %d to %d ; ",toplace[i]->path[j], toplace[i]->path[j+1]);
                    j = j+2;
                }
                // printf("from %d to %d ; ",toplace[i]->path[j], toplace[i]->path[j+1]);
            }
        }
        printf("\n");
    }
    printf("----------\n");
}

To_Place** To_place_create(Board* bord, Player_Info* info){
    To_Place** toplace = malloc(10 * sizeof(To_Place*));
    //add objective roads
    for(int i=0; i<info->nbobjective; i++){
        toplace[i] = shortest(bord, info->objective[i]->from, info->objective[i]->to);
        toplace[i]->ev = (float)(info->objective[i]->score) / (float)(toplace[i]->nbwagons);
    }
    //add empty to fill up
    for(int i=info->nbobjective; i<10; i++){
        toplace[i] = NULL;
    }
    return toplace;
}

int search_index(Board* bord, Player_Info* info, To_Place** toplace, int max){
    int i = 0;
    int found = 0;

    //search for the road we want to build
    //search a road with a set color first
    while(toplace[max]->path[i+1] != -1 && !found){
        if(bord->MatRoute[toplace[max]->path[i]][toplace[max]->path[i+1]].color != 9){
            found = 1;
            i = i-2;
        }
        i = i+2;
    }
    //if not found take the first road (it should not have a set color if everything goes right)
    if(!found) i = 0;
    return i;
}

int search_color_pick(Board* bord, Player_Info* info, To_Place** toplace, int max, int index){
    int city1 = toplace[max]->path[index];
    int city2 = toplace[max]->path[index+1];
    int color = bord->MatRoute[city1][city2].color;
    int color2 = bord->MatRoute[city1][city2].color2;

    //found a road with a set color
    if(bord->MatRoute[city1][city2].color != 9){
        //PROBLEM: does not take into account what color is needed for future roads (if 2 colors available for 1 road (especially?))
        //enough to place
        if(info->cards[color] >= bord->MatRoute[city1][city2].length) return -color;
        if(color2 != 0 && info->cards[color2] >= bord->MatRoute[city1][city2].length) return -color2;

        //not enough : find which color to go for
        //go for color1
        if(info->cards[color] > info->cards[color2]){
            //AAAAAAAAAAAAAAAA : fonction seulement pour piocher la carte; prendre en compte le fait de piocher 2 fois: trouver une 2e route (avec find index à modifier)
        }
    }
    // enough wagons of desired color to place? is color available to pick?
    
    
    return ;
}

int find_max_ev(To_Place** toplace){
    int max = -1;
    int i = -1;
    for(int j=0; j<10; j++){ // 10 toplace in the array
        if(toplace[j] != NULL){
            if(toplace[j]->ev > max){
                max = toplace[j]->ev;
                i = j;
            }
        }
    }
    if(max == -1){
        printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAA\nERROR IN find_max_ev : MAX EV NOT FOUND\n");
        return -1;
    }
    return i;
}

/*OK it seems ?*/
void destroy_toplace(To_Place** toplace){
    for(int i=0; i<10; i++){ // 10 toplace in the array
        if(toplace[i] != NULL){
            free(toplace[i]->path);
            free(toplace[i]);
        }
    }
    free(toplace);
}