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
        info->cards[info->movedata->claimRoute.color] -= bord->MatRoute[info->movedata->claimRoute.from][info->movedata->claimRoute.to].length - info->movedata->claimRoute.nbLocomotives;
        info->cards[9] -= info->movedata->claimRoute.nbLocomotives;
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
            info->cards[info->movedata->drawCard]++;
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
    bord->gofast = 0;
    return bord;
}
/* init board, to call after alloc_board and sendgamesettings.*/
void init_board(Board* bord){
    getBoardState(bord->cards_pickable);
    bord->MatRoute = allouer_matrice_route(bord->gamedata->nbCities);
    bord->when = -1;
    bord->gofast = 0;
    convert_tab_matrice(bord);
}

void destroy_board(Board* bord){
    free(bord->cards_pickable);
    destroy_matrice_route(bord->MatRoute, bord->gamedata->nbCities);
    free(bord->gamedata->gameName);
    free(bord->gamedata->trackData);
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
            dijk[city2].color_forced = dijk[city1].color_forced;
            dijk[city2].points_road = dijk[city1].points_road;
        }
        return;
    }
    //route taken by opponent
    else if(bord->MatRoute[city1][city2].taken > 0){
        return;
    }
    //route is free to take and is shorter through city1
    else if(dijk[city2].weight >= (dijk[city1].weight + bord->MatRoute[city1][city2].length)){
        // less wagons
        if(dijk[city2].weight > (dijk[city1].weight + bord->MatRoute[city1][city2].length)){
            dijk[city2].weight = dijk[city1].weight + bord->MatRoute[city1][city2].length;
            dijk[city2].prev = city1;
            if(bord->MatRoute[city1][city2].color == 9) dijk[city2].color_forced = dijk[city1].color_forced; // no set color : same
            else dijk[city2].color_forced = dijk[city1].color_forced + bord->MatRoute[city1][city2].length; // has a set color : add the length
            dijk[city2].points_road = dijk[city1].points_road + points(bord->MatRoute[city1][city2].length); // update the points from placing roads
        }
        //if both have the same weight, choose the one that has the max points from roads placed,
        // and if equal the one with the least forced colors
        if(dijk[city2].weight == (dijk[city1].weight + bord->MatRoute[city1][city2].length)){
            if(dijk[city1].points_road+points(bord->MatRoute[city1][city2].length) > dijk[city2].points_road){
                dijk[city2].weight = dijk[city1].weight + bord->MatRoute[city1][city2].length;
                dijk[city2].prev = city1;
                dijk[city2].points_road = dijk[city1].points_road + points(bord->MatRoute[city1][city2].length);
                if(bord->MatRoute[city1][city2].color != 9){ // route has a set color
                    dijk[city2].color_forced =  dijk[city1].color_forced + bord->MatRoute[city1][city2].length;
                }
                else dijk[city2].color_forced = dijk[city1].color_forced;
            }
        }
    }
    return;
}

/* wagons/points*/
void update_weight_v2(Dijkstra_City* dijk, int city1, int city2, Board* bord){
    //route taken by player
    if(bord->MatRoute[city1][city2].taken == 0){
        if(dijk[city2].weight > dijk[city1].weight){
            dijk[city2].weight = dijk[city1].weight;
            dijk[city2].prev = city1;
            dijk[city2].color_forced = dijk[city1].color_forced;
            dijk[city2].points_road = dijk[city1].points_road;
            dijk[city2].nb_wagons = dijk[city1].nb_wagons;
        }
        return;
    }
    //route taken by opponent
    else if(bord->MatRoute[city1][city2].taken > 0){
        return;
    }
    //route is free to take and is faster through city 1
    else if((float)dijk[city2].weight > ((float)(dijk[city1].nb_wagons+bord->MatRoute[city1][city2].length)/20+(float)(dijk[city1].points_road+points(bord->MatRoute[city1][city2].length)))){
        dijk[city2].nb_wagons = dijk[city1].nb_wagons + bord->MatRoute[city1][city2].length;
        dijk[city2].points_road = dijk[city1].points_road + points(bord->MatRoute[city1][city2].length);
        dijk[city2].prev = city1;
        dijk[city2].weight = (float)dijk[city2].nb_wagons / 20+((float)(dijk[city2].points_road));
    }
    return;
}

/* dikstra to find the shortest path between city1 and city2
// IMPORTANT: add anti blocked for city 0 (of origin?)
add how many roads are needed ? (for speed, optimising toplace* malloc space, building of the full road)*/
To_Place* shortest(Board* bord, int city1, int city2, int* blocked){
    int city;
    To_Place* toplace = malloc(sizeof(To_Place));
    toplace->path = malloc(bord->gamedata->nbCities * 2 * sizeof(int));
    toplace->priority = malloc(bord->gamedata->nbCities * sizeof(int));
    for(int i=0; i<bord->gamedata->nbCities; i++) toplace->priority[i] = -1;
    toplace->col = malloc(10* sizeof(CardColor));
    for(int i=0; i<10; i++) toplace->col[i] = 0;
    toplace->city1 = city1;
    toplace->city2 = city2;
    toplace->nbwagons = 0;
    toplace->points_place = 0;

    //init checked + weight
    Dijkstra_City* dijkstra = malloc(bord->gamedata->nbCities * sizeof(Dijkstra_City));
    for(int i=0; i<bord->gamedata->nbCities; i++){
         dijkstra[i].checked = 0;
         dijkstra[i].weight = __FLT_MAX__;
         dijkstra[i].prev = -1;
         dijkstra[i].color_forced = 0;
         dijkstra[i].points_road = 0;
         dijkstra[i].nb_wagons = 0;
    }
    dijkstra[city1].weight = 0;

    //main loop
    while(!(all_checked(dijkstra, bord)) && dijkstra[city2].checked == 0){
        city = find_min(dijkstra, bord->gamedata->nbCities);
        if(city == -1){ // the city is blocked and unreachable. too bad..
            free(dijkstra);
            destroy_place(toplace);
            *blocked = 1;
            return NULL;
        }
        dijkstra[city].checked = 1;
        for(int i=0; i<bord->gamedata->nbCities; i++){
            if(bord->MatRoute[city][i].length > 0){
                update_weight(dijkstra, city, i, bord);
            }
        }
    }

    //build the full road (from city2 to city1)
    city = city2;
    int i=0;
    toplace->nbwagons = dijkstra[city2].weight;
    while(dijkstra[city].prev != -1){
        if(bord->MatRoute[city][dijkstra[city].prev].taken == -1){ //only add unconnected cities
            toplace->path[i] = city;
            toplace->path[i+1] = dijkstra[city].prev;
            toplace->points_place += points(bord->MatRoute[i][i+1].length);
            if(bord->MatRoute[dijkstra[city].prev][city].color != 9){ // if road has a color
                toplace->col[bord->MatRoute[dijkstra[city].prev][city].color] += bord->MatRoute[dijkstra[city].prev][city].length; // add the length to the color count
                if(bord->MatRoute[dijkstra[city].prev][city].color2 != 0 && bord->MatRoute[dijkstra[city].prev][city].color2 != 9){ // road also has a second color
                    toplace->col[bord->MatRoute[dijkstra[city].prev][city].color2] += bord->MatRoute[dijkstra[city].prev][city].length;
                }
            }
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
    for(int i=0; i<20; i++){
        j = 0;
        if(toplace[i] != NULL){ // check if toplace[i] is null
        printf("FROM: %d TO: %d NBWAGONS: %d EV: %f\n", toplace[i]->city1, toplace[i]->city2, toplace[i]->nbwagons, toplace[i]->ev);
            if(toplace[i]->path != NULL){
                while(toplace[i]->path[j+1] != -1){
                    printf("from %d to %d ; ",toplace[i]->path[j], toplace[i]->path[j+1]);
                    j = j+2;
                }
            }
            printf("\n");
            if(toplace[i]->priority != NULL){
                j = 0;
                while(toplace[i]->priority[j] != -1){
                    printf("priority: %d ;", toplace[i]->priority[j]);
                    j++;
                }
            }
            printf("\n\n");
        } 
    }
    printf("----------\n");
}

To_Place** To_place_create(Board* bord, Player_Info* info){
    To_Place** toplace = malloc(20 * sizeof(To_Place*));
    int index = 0;
    int blocked = 0;
    //add objective roads
    for(int i=0; i<info->nbobjective; i++){
        toplace[index] = shortest(bord, info->objective[i]->from, info->objective[i]->to, &blocked);
        if(toplace[index] != NULL){
            toplace[index]->ev = ev_calculate(info, toplace[index], i);
            // if the 2 cities are already connected do not add them to toplace. (reset)
            // same if one of them is blocked off
            if(toplace[index]->nbwagons == 0 || toplace[index]->nbwagons > 100){
                destroy_place(toplace[index]);
                index--;
            }
        index++;
        }

        else{
            destroy_place(toplace[index]);
        }
    }
    //add empty to fill up
    for(int i=index; i<20; i++){
        toplace[i] = NULL;
    }
    update_To_place_len(toplace, bord, info); // add the estimate distance

    return toplace;
}

void update_priority(Board* bord, Player_Info* info, To_Place** toplace){
    int priority;
    int j;

    for(int i=0; i<20; i++){ // for each path of the toplace array :
        j = 0;
        priority = 0;
        
        if(toplace[i] != NULL){
            //search roads with a set color first, with highest length first (=6)
            for(int l=6 ; l>0 ; l--){
                while(toplace[i]->path[j+1] != -1){
                    if(bord->MatRoute[toplace[i]->path[j]][toplace[i]->path[j+1]].color != 9 && bord->MatRoute[toplace[i]->path[j]][toplace[i]->path[j+1]].length == l){
                        toplace[i]->priority[j/2] = priority;
                        priority ++;
                    }
                    j = j+2;
                }
            }

            //add the remaining roads (without a set color) keeping the order. (more resilient if the opponent tries to block?)
            j = 0;
            while(toplace[i]->path[j+1] != -1){
                if(toplace[i]->priority[j/2] == -1){
                    toplace[i]->priority[j/2] = priority;
                    priority++;
                }
                j = j+2;
            }
        }
    }
    return;
}

int find_min_priority(Board* bord, Player_Info* info, To_Place** toplace, int max){
    int i = 0;
    //find the road with the lowest priority (=0)
    while(toplace[max]->priority[i] != -1){
        if(toplace[max]->priority[i] == 0) return 2*i;
        i++;
    }
    printf("ERROR : FIND MIN PRIORITY NOT FOUND\n");
    return -1;
}

int search_color_pick(Board* bord, Player_Info* info, To_Place** toplace, int max, int pick){
    int priority = 0;
    int i = 0;
    int found = 0;
    int nb_road = 0;

    int city1;
    int city2;
    int color1;
    int color2;

    //find the number of roads
    while(toplace[max]->priority[i] != -1){
        nb_road++;
        i++;
    }
    for(int k=0; k<nb_road; k++){
        i = 0;
        found = 0;

        //find the road with the lowest priority that has not been checked yet
        while(toplace[max]->priority[i] != -1 && !found){
            if(toplace[max]->priority[i] == priority){
                i--; //neutralize i++
                found = 1; //stop while loop
                priority++; // next time search for 1 priority higher
            }
            i++;
        }

        //debug
        // printf("NOW CHECKING ROAD OF INDEX: %d\n", i);

        //for convenience
        city1 = toplace[max]->path[2*i];
        city2 = toplace[max]->path[2*i+1];
        color1 = bord->MatRoute[city1][city2].color;
        color2 = bord->MatRoute[city1][city2].color2;

        //not good, should never be true
        if(toplace[max]->priority[i] == -1){
            printf("AAAAA\nAAAAA\nAAAAA\nNOT GOOD: PROBLEM IN SEARCH_COLOR_PICK\n");
            //pick random just in case..
            return 10;
        }

        //found road has a set color
        if(color1 != 9){
            //already enough to place
            if(!pick){
                if(info->cards[color1] >= bord->MatRoute[city1][city2].length && info->nbwagons >= bord->MatRoute[city1][city2].length) return (-color1);
                if(color2 != 0 && (info->cards[color2] >= bord->MatRoute[city1][city2].length && info->nbwagons >= bord->MatRoute[city1][city2].length)) return (-color2);
            }

            //TO CHANGE: WHAT TO DO IF ROAD HAS 2 COLORS ?
            //not enough to place, can we pick a visible card ?
            for(int j=0; j<5; j++){
                if(bord->cards_pickable->card[j] == color1) return color1;
                if(bord->cards_pickable->card[j] == color2) return color2;
            }

            //can't pick a visible card, try with a joker, but only if we have more cards than wagons : to ensure we don't waste those jokers early
            if(!pick && info->nbcards >= info->nbwagons){
                if(info->cards[color1] + info->cards[9] >= bord->MatRoute[city1][city2].length) return (-color1 -10);
                if((color2 != 0 && (info->cards[color2] + info->cards[9])) >= bord->MatRoute[city1][city2].length) return (-color2 -10);
            }

            //try to pick for next route (loop)
            pick = 1;
        }

        //found road does NOT have a set color
        if(color1 == 9){
            // pick a random card if pick=1
            if(pick) return 10;

            //try to place the road if priority = 0
            if(toplace[max]->priority[i] == 0){
                for(int j=1; j<9; j++){
                    if(info->cards[j] >= bord->MatRoute[city1][city2].length) return (-j);
                }
            }
            //else pick random
            return 10;
        }
    }

    printf("AAAAA\nAAAAA\nAAAAA\n GOT TO THE END OF SEARCH_COLOR_PICK\n");
    return 10;
    ////////////////////////////////////////////////////////////
    // penser à: jokers(check si on peut piocher pour garder le joker), , 2 couleurs sur 1 route
    //1 assez de cartes pour poser, sinon
    //2 cartes dispo à piocher, sinon (basculer pioche)
    //3 assez de cartes avec les jokers, sinon
    //4 pioche pour la ville suivante (basculer pioche)
    //5 pick random
}

Action_order* search_color_pick_v2(Board* bord, Player_Info* info, To_Place** toplace, int pick){
    int city1;
    int city2;
    int color1;
    int color2; // for convenience

    int priority;
    int pos_priority;

    Action_order* action = malloc(sizeof(Action));
    action->city1 = -1;
    action->city2 = -1;
    action->color = -1;
    action->joker = 0;
    action->move = -1;

    int pos_toplace = find_max_ev(toplace);

    while(pos_toplace != -1){ // check every To_place in the array, from highest to lowest EV
        priority = 0;
        pos_priority = find_priority(bord, toplace[pos_toplace], priority); // find the first one
        while(pos_priority != -1){ // check every road in toplace[pos_toplace], starting at priority =0, then 1,2..
            city1 = toplace[pos_toplace]->path[2*pos_priority];
            city2 = toplace[pos_toplace]->path[2*pos_priority+1];
            color1 = bord->MatRoute[city1][city2].color;
            color2 = bord->MatRoute[city1][city2].color2; // for convenience

            if(color1 != 9 && !pick){ // road has a color : try to place it !!!!!!!!!! check how many colors we need
                if(color2 != 9 && color2 != 0){ // no color 2 : check only color1
                    // we have enough cards to place (check if enough wagons left as well)
                    if(info->cards[color1] >= bord->MatRoute[city1][city2].length &&
                    info->nbwagons >= bord->MatRoute[city1][city2].length){
                        action->city1 = city1;
                        action->city2 = city2;
                        action->color = color1;
                        action->joker = 0;
                        action->move = 1;
                        return action;
                    }
                }
                if(toplace[pos_toplace]->col[color1] >= toplace[pos_toplace]->col[color2]){ // if 2 colors: 1 is better
                    if(info->cards[color1] >= bord->MatRoute[city1][city2].length &&
                    info->nbwagons >= bord->MatRoute[city1][city2].length){ // we can place color1
                        action->city1 = city1;
                        action->city2 = city2;
                        action->color = color1;
                        action->joker = 0;
                        action->move = 1;
                        return action;
                    }
                    // enough for color 2
                    if(info->cards[color2] >= bord->MatRoute[city1][city2].length &&
                    info->nbwagons >= bord->MatRoute[city1][city2].length){
                        action->city1 = city1;
                        action->city2 = city2;
                        action->color = color2;
                        action->joker = 0;
                        action->move = 1;
                        return action;
                    }
                }
                else{ // color 2 is checked first
                    if(info->cards[color2] >= bord->MatRoute[city1][city2].length &&
                    info->nbwagons >= bord->MatRoute[city1][city2].length){
                        action->city1 = city1;
                        action->city2 = city2;
                        action->color = color2;
                        action->joker = 0;
                        action->move = 1;
                        return action;
                    }
                    if(info->cards[color1] >= bord->MatRoute[city1][city2].length &&
                    info->nbwagons >= bord->MatRoute[city1][city2].length){ // we can place color1
                        action->city1 = city1;
                        action->city2 = city2;
                        action->color = color1;
                        action->joker = 0;
                        action->move = 1;
                        return action;
                    }
                }
            }

            if(color1 == 9 && !pick){ // road does not have a set color : try to place with card surplus
                for(int j=1; j<9; j++){
                    if(((int)info->cards[j] - (int)toplace[pos_toplace]->col[j]) >= (int)bord->MatRoute[city1][city2].length){ // enough surplus : need-actual >= cost
                        action->city1 = city1;
                        action->city2 = city2;
                        action->color = j;
                        action->joker = 0;
                        action->move = 1;
                        return action;
                    }
                }
            }

            // could not place roads : try picking a visible card
            int col_max = 9999;
            for(int j=0; j<10; j++){
                int i = find_next_max_color(toplace[pos_toplace]->col, col_max); // used to store the max associated with the color we need the most
                col_max = toplace[pos_toplace]->col[i];
                if(i!=0 && i!=9){ // if max color is not a joker or none :
                    for(int k=0; k<5; k++){ // check pickable cards
                        if(bord->cards_pickable->card[k] == i){
                            action->city1 = -1;
                            action->city2 = -1;
                            action->color = i;
                            action->joker = 0;
                            action->move = 3;
                            return action;
                        }
                    }
                }
            }

            // could not place nor pick a colored card : try with a joker
            if(info->nbcards >= info->nbwagons && !pick){ // if we have more cards than wagons remaining (no need to waste the jokers early)
                if(color1 != 9){
                    if(color2 == 9 || color2 == 0){ // if color 2 does not exist : only check color 1
                        if(info->cards[color1] + info->cards[9] >= bord->MatRoute[city1][city2].length){ // enough for color1
                            action->city1 = city1;
                            action->city2 = city2;
                            action->color = color1;
                            action->joker = bord->MatRoute[city1][city2].length - info->cards[color1];
                            action->move = 1;
                            return action;
                        }
                    } // else check for both colors

                    if(toplace[pos_toplace]->col[color1]-info->cards[color1] >= toplace[pos_toplace]->col[color2]-info->cards[color2]){ // choose between color 1 and color 2 : 2 first
                        if(info->cards[color2] + info->cards[9] >= bord->MatRoute[city1][city2].length){ // enough for color2
                            action->city1 = city1;
                            action->city2 = city2;
                            action->color = color2;
                            action->joker = bord->MatRoute[city1][city2].length - info->cards[color2];
                            action->move = 1;
                            return action;
                        }
                        if(info->cards[color1] + info->cards[9] >= bord->MatRoute[city1][city2].length){ // enough for color1
                            action->city1 = city1;
                            action->city2 = city2;
                            action->color = color1;
                            action->joker = bord->MatRoute[city1][city2].length - info->cards[color1];
                            action->move = 1;
                            return action;
                        }
                    }
                    else{ // check 1 then 2
                        if(info->cards[color1] + info->cards[9] >= bord->MatRoute[city1][city2].length){ // enough for color2
                            action->city1 = city1;
                            action->city2 = city2;
                            action->color = color1;
                            action->joker = bord->MatRoute[city1][city2].length - info->cards[color1];
                            action->move = 1;
                            return action;
                        }
                        if(info->cards[color2] + info->cards[9] >= bord->MatRoute[city1][city2].length){ // enough for color1
                            action->city1 = city1;
                            action->city2 = city2;
                            action->color = color2;
                            action->joker = bord->MatRoute[city1][city2].length - info->cards[color2];
                            action->move = 1;
                            return action;
                        }
                    }
                }
            }

            priority++; // prepare to check next one
            pos_priority = find_priority(bord, toplace[pos_toplace], priority);
        }

        pos_toplace = find_next_max_ev(toplace, toplace[pos_toplace]);
    }

    // if we could not do any specific move (or a bug happened) pick a random card
    if(!bord->gofast){
        action->city1 = -1;
        action->city2 = -1;
        action->color = -1;
        action->joker = 0;
        action->move = 2;
        return action;
    }

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! put gofast before main loop ?
    // gofast : the game is almost over ! instead of picking a random card, try to finish the game as fast as possible
    // first, try to place a random road that is as long as possible (we start at nbwagons length : not optimised but avoids some bugs)
    if(!pick){
        for(int j=info->nbwagons; j>=0; j--){
            for(int k=0; k<bord->gamedata->nbCities; k++){
                for(int l=0; l<bord->gamedata->nbCities; l++){
                    city1 = k;
                    city2 = j;
                    color1 = bord->MatRoute[city1][city2].color;
                    color2 = bord->MatRoute[city1][city2].color2;

                    if(bord->MatRoute[city1][city2].length == j){
                        if(color1 != 9 && bord->MatRoute[city1][city2].taken < 0 && info->cards[color1] >= bord->MatRoute[city1][city2].length){ // enough color 1 to place
                            action->city1 = city1;
                            action->city2 = city2;
                            action->color = color1;
                            action->joker = 0;
                            action->move = 1;
                            return action;
                        }
                        else if(color2 != 9 && color2 != 0 && bord->MatRoute[city1][city2].taken < 0 && info->cards[color2] >= bord->MatRoute[city1][city2].length){ // enough color2 to place
                            action->city1 = city1;
                            action->city2 = city2;
                            action->color = color2;
                            action->joker = 0;
                            action->move = 1;
                            return action;
                        }
                        else if(color1 == 9){
                            for(int k=1; k<9; k++){
                                if(bord->MatRoute[city1][city2].taken < 0 && info->cards[k] >= bord->MatRoute[city1][city2].length){
                                    action->city1 = city1;
                                    action->city2 = city2;
                                    action->color = k;
                                    action->joker = 0;
                                    action->move = 1;
                                    return action;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // if we can't place, search a color to pick : take a color we already have (not perfect but will do)
    int coolor = find_next_max_color(info->cards, 999);
    while(coolor != -1){
        for(int k=0; k<5; k++){
            if(bord->cards_pickable->card[k] == coolor){
                action->city1 = -1;
                action->city2 = -1;
                action->color = coolor;
                action->joker = 0;
                action->move = 3;
                return action;
            }
        }
        coolor = find_next_max_color(info->cards, info->cards[coolor]);
    }
    // if nothing was found pick random (should never happen)
    action->city1 = -1;
    action->city2 = -1;
    action->color = -1;
    action->joker = 0;
    action->move = 2;
    return action;
}

int find_max_ev(To_Place** toplace){
    float max = -1;
    int i = -1;
    for(int j=0; j<20; j++){ // 20 toplace in the array
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
void destroy_toplace(To_Place** toplace, Player_Info* info){
    for(int i=0; i<info->nbobjective; i++){ // nbobjective toplace allocated in the array (for now)
        if(toplace[i] != NULL){
            free(toplace[i]->path);
            free(toplace[i]->priority);
            free(toplace[i]->col);
            free(toplace[i]);
        }
    }
    free(toplace);
}

void destroy_place(To_Place* place){
    if(place != NULL){
        free(place->path);
        free(place->priority);
        if(place->col) free(place->col);
    }
    free(place);
}

int find_nb_joker(Board* bord, Player_Info* info, To_Place** toplace, int max, int road, int choice){
    // decode the choice to color
    choice = -choice -10;
    int joker = (bord->MatRoute[toplace[max]->path[road]][toplace[max]->path[road+1]].length - info->cards[choice]);
    if(joker < 0) joker = 0;
    return joker;
}

int find_next_max_ev(To_Place** toplace, To_Place* current_ev){
    int pos_ev;
    for(int i=0; i<20; i++){
        if(toplace[i] != NULL && toplace[i] == current_ev){
            pos_ev = i;
        }
    }
    
    int j = -1;
    float ev_found = -1;
    for(int i=0; i<20; i++){
        if(toplace[i] != NULL && // found a better one
            toplace[i]->ev > ev_found && // higher EV than current
            (toplace[i]->ev < current_ev->ev || (toplace[i]->ev == current_ev->ev && i > pos_ev)))
            { 
            j = i;
            ev_found = toplace[i]->ev;
        }
    }
    // returns -1 if nothing found (used in update_to_place_len, maybe somewhee else)
    return j; // all good
}

void update_To_place_len(To_Place** toplace, Board* bord, Player_Info* info){
    if(toplace == NULL || toplace[0] == NULL) return;

    To_Place* place = toplace[find_max_ev(toplace)];
    To_Place* temp;
    int i;
    int blocked = 0;
    // make a copy of the route matrix
    route** mat_copy = allouer_matrice_route(bord->gamedata->nbCities);
    for(int cpt1=0; cpt1< bord->gamedata->nbCities; cpt1++){
        for(int cpt2=0; cpt2< bord->gamedata->nbCities; cpt2++){
            mat_copy[cpt1][cpt2] = bord->MatRoute[cpt1][cpt2];
        }
    }
    // make a copy of Board in order to use the shortest function
    Board bord_copy;
    bord_copy.gamedata = bord->gamedata;
    bord_copy.MatRoute = mat_copy;

    while(find_next_max_ev(toplace, place) != -1){ //iterate on all items in toplace.
        i = 0;
        // find the length for this one
        temp = shortest(&bord_copy, place->city1, place->city2, &blocked);
        if(temp != NULL){
            place->length_est = 0;
            place->length_est = temp->nbwagons;
            while(temp->path[i] != -1){
                mat_copy[temp->path[i]][temp->path[i+1]].taken = 0; // fill up matrix to pretend the road is taken (is actually free)
                mat_copy[temp->path[i+1]][temp->path[i]].taken = 0;
                i+=2;
            }
            destroy_place(temp); // to check !
        }
        place = toplace[find_next_max_ev(toplace, place)];
    }
    //needs to do the last one. (we stop when there is no next one)
    i = 0;
    temp = shortest(&bord_copy, place->city1, place->city2, &blocked);
    place->length_est = 0;
    place->length_est = temp->nbwagons;
    while(temp->path[i] != -1){
        mat_copy[temp->path[i]][temp->path[i+1]].taken = 0;
        mat_copy[temp->path[i+1]][temp->path[i]].taken = 0;
        i+=2;
    }
    destroy_place(temp);
    destroy_matrice_route(mat_copy, bord->gamedata->nbCities);

    return;
}

int points(int len){
    switch(len){
        case 1:
        return 1;
    
        case 2:
        return 2;

        case 3:
        return 4;

        case 4:
        return 7;

        case 5:
        return 10;

        case 6:
        return 15;
    }
}

float ev_calculate(Player_Info* info, To_Place* place, int obj_number){
    float points_road = (float)place->points_place; // points for placing roads
    float obj_points = (float)info->objective[obj_number]->score; // points from the objective
    float length = (float)place->nbwagons;

    if(length == 0) return (float)__INT_MAX__;
    return (points_road+2*obj_points)/length;
}

float ev_calculate_result(Player_Info* info, To_Place* place, int obj_number_result){
    float points_road = (float)place->points_place; // points for placing roads
    float obj_points = (float)info->moveresult->objectives[obj_number_result].score; // points from the objective
    float length = (float)place->nbwagons;

    if(length == 0) return (float)__INT_MAX__;
    return (points_road+2*obj_points)/length;
}

float max3(float val1, float val2, float val3, int index){
    if(val1 && val2 && val3){ // if none are null (can happen if a picked objective is already completed)
        if(val1 >= val2 && val1 >= val3){
            if(index) return 0;
            return val1;
        }
        if(val2 >= val1 && val2 >= val3){
            if(index) return 1;
            return val2;
        }
        if(index) return 2;
        return val3;
    }
}

float max2(float val1, float val2, int index){
    if(val1 >= val2){
        if(index) return 0;
        return val1;
    }
    if(index) return 1;
    return val2;
}

float ev_estimate_result(Player_Info* info, To_Place* place, int obj_number_result){
    if(place == NULL) return -1;
    float points_road = (float)place->points_place; // points for placing roads
    float obj_points = (float)info->moveresult->objectives[obj_number_result].score; // points from the objective
    float length = (float)place->length_est;

    if(length == 0) return (float)__INT_MAX__;
    return (points_road+2*obj_points)/length;
}

void pick_new_objectives(To_Place** toplace, Player_Info* info, Board* bord){
    info->movedata->action = 4;
    sendMove(info->movedata,info->moveresult);

    info->movedata->action = 5;
    // make another toplace
    int blocked = 0;
    int wagon_cpt = info->nbwagons;
    To_Place** toplace_copy = malloc(20* sizeof(To_Place*));
    for(int i=0; i<20; i++){
        toplace_copy[i] = NULL;
    }

    for(int j=0; j<3; j++){
        toplace_copy[j] = shortest(bord, info->moveresult->objectives[j].from, info->moveresult->objectives[j].to, &blocked);
        if(toplace_copy[j] != NULL){
            toplace_copy[j]->ev = ev_calculate_result(info, toplace_copy[j], j);
        }
        else{
            toplace_copy[j] = malloc(sizeof(To_Place));
            if(blocked){
                blocked = 0;
                toplace_copy[j]->ev = -99999;
                toplace_copy[j]->points_place = -99999;
            }
            else toplace_copy[j]->ev = __INT_MAX__;
            toplace_copy[j]->city1 = info->moveresult->objectives[j].from;
            toplace_copy[j]->city2 = info->moveresult->objectives[j].to;
            toplace_copy[j]->length_est = 1; // not 0 to avoid it being confused with an already connected road
            toplace_copy[j]->nbwagons = 1;
            toplace_copy[j]->path = malloc(sizeof(int)); // not used : just to conform with the destroy function
            toplace_copy[j]->priority = malloc(sizeof(int));
            toplace_copy[j]->col = malloc(sizeof(int));
        }
    }
    toplace_copy[3] = NULL;

    // calculate estimate distances
    update_To_place_len(toplace_copy, bord, info);

    // set all objectives to not taken as default
    info->movedata->chooseObjectives[0] = 0;
    info->movedata->chooseObjectives[1] = 0;
    info->movedata->chooseObjectives[2] = 0;

    // calculate the estimated EVs
    int max_ev = max3(toplace_copy[0]->ev,toplace_copy[1]->ev,toplace_copy[2]->ev, 1);
    int mid_ev = (max_ev+1)%3;
    if(max2(toplace_copy[(max_ev+1)%3]->ev, toplace_copy[(max_ev+2)%3]->ev, 1)) mid_ev = (max_ev+1)%3;
    int min_ev = max_ev^mid_ev^3;

    int max_ev_est = min_ev; // to know which objective has the highest estimate EV (assuming the max EV is already placed of course!)
    if(ev_estimate_result(info,toplace_copy[mid_ev],mid_ev >= ev_estimate_result(info,toplace_copy[min_ev],min_ev))){
        max_ev_est = mid_ev;
    }
    
    // always pick the best one : max current ev(we are forced to pick at least one)
    info->movedata->chooseObjectives[max_ev] = 1;
    wagon_cpt -= toplace_copy[max_ev]->length_est; // estimate of how much wagons we will have once the path is completed
    // !! length_est is a bit risky. maybe nbwagons is safer

    // take the second best one if we have enough wagons (no check on EV_est value for now)
    if(wagon_cpt - toplace_copy[max_ev_est]->length_est >= 8 &&
        ev_estimate_result(info,toplace_copy[max_ev_est],max_ev_est)/(float)toplace_copy[max_ev]->nbwagons > 0.8 
    ){
        info->movedata->chooseObjectives[max_ev_est] = 1;
        wagon_cpt -= toplace_copy[max_ev_est]->length_est;
    }

    // same for the third one
    if(wagon_cpt - toplace_copy[max_ev^max_ev_est^3]->length_est >= 8 &&
        ev_estimate_result(info,toplace_copy[max_ev^max_ev_est^3],max_ev^max_ev_est^3)/(float)toplace_copy[max_ev]->nbwagons > 0.8 
    ){
        info->movedata->chooseObjectives[max_ev^max_ev_est^3] = 1;
        wagon_cpt -= toplace_copy[max_ev^max_ev_est^3]->length_est;
    }

    // if at the beginning of the game : make sure we take at least 2, max current ev and max estimated ev. (should be always OK but we never know..)
    if(bord->when == -1){
        bord->when = 0;
        info->movedata->chooseObjectives[max_ev] = 1;
        info->movedata->chooseObjectives[max_ev_est] = 1;
    }
    
    // cleanup
    for(int i=0; i<3; i++){
        destroy_place(toplace_copy[i]);
    }
    free(toplace_copy);
    return;
}

int find_priority(Board* bord ,To_Place* place, int priority){
    if(place == NULL) return -1;
    if(place->priority == NULL) return -1;
    for(int i=0; bord->gamedata->nbCities; i++){
        if(place->priority[i] == priority) return i; // found
        if(place->priority[i] == -1) return -1; // got to the end
    }
    printf("ERROR IN FIND_PRIORITY : NOT FOUND\n");
    return -1;
}

int find_next_max_color(CardColor* col, int current_max){
    int pos_max;
    for(int i=0; i<10; i++){
        if(col[i] == current_max){
            pos_max = i;
        }
    }
    
    int j = -1;
    float max_found = -1;
    for(int i=0; i<10; i++){
        if(col[i] > max_found && // higher than current
            (col[i] < current_max || (col[i] == current_max && i > pos_max)))
            { 
            j = i;
            max_found = col[i];
        }
    }
    // returns -1 if nothing found (used in update_to_place_len, maybe somewhee else)
    return j; // all good
}

int decode_action(Player_Info* info, Action_order* action){
    int ret = 0;
    info->movedata->action = action->move;
    if(action->move == 1){
        info->movedata->claimRoute.from = action->city1;
        info->movedata->claimRoute.to = action->city2;
        info->movedata->claimRoute.color = action->color;
        info->movedata->claimRoute.nbLocomotives = action->joker;
    }
    if(action->move == 3){
        info->movedata->drawCard = action->color;
    }
    if(action->move == 2 || (action->move == 3 && action->color != 9)) ret = 1; // if we replay (picking objectives is managed by another function)

    free(action);
    return ret;
}

Action_order* search_color_pick_v3(Board* bord, Player_Info* info, To_Place** toplace, int pick, Player_Info* info_opp){
    int city1;
    int city2;
    int color1;
    int color2;
    int length; // for convenience

    Action_order* action = malloc(sizeof(Action));
    action->city1 = -1;
    action->city2 = -1;
    action->color = -1;
    action->joker = 0;
    action->move = -1;

    int pos_toplace = find_max_ev(toplace); // position of max EV
    int priority = 0; // priority we are checking
    int pos_priority = -1;
    if(pos_toplace != -1) pos_priority = find_priority(bord, toplace[pos_toplace], priority); //position of current priority
    int max_surplus;

    while(pos_priority != -1){ // check all roads in the path
        city1 = toplace[pos_toplace]->path[2*pos_priority];
        city2 = toplace[pos_toplace]->path[2*pos_priority+1];
        color1 = bord->MatRoute[city1][city2].color;
        color2 = bord->MatRoute[city1][city2].color2;
        length = bord->MatRoute[city1][city2].length;

        // // // // //

        if(color1 != 9){ // road has a set color
            if(!pick && info->nbwagons >= length){ // not pick : try to place (without jokers!)
                if(color2 != 9 && color2 != 0){ // no second color : check for color1 only
                    if((int)info->cards[color1] >= length){
                        action->city1 = city1;
                        action->city2 = city2;
                        action->color = color1;
                        action->joker = 0;
                        action->move = 1;
                        return action;
                    }
                }
                // there is a second color : determine if we check color1 or color2 first
                if((int)toplace[pos_toplace]->col[color1] >= (int)toplace[pos_toplace]->col[color2]){ // need more of color1 : 2 first
                    if((int)info->cards[color2] >= length){
                        action->city1 = city1;
                        action->city2 = city2;
                        action->color = color2;
                        action->joker = 0;
                        action->move = 1;
                        return action;
                    }
                    if((int)info->cards[color1] >= length){ // then check 1
                        action->city1 = city1;
                        action->city2 = city2;
                        action->color = color1;
                        action->joker = 0;
                        action->move = 1;
                        return action;
                    }
                }
                else{ // check 1 then 2
                    if((int)info->cards[color1] >= length){ // 1
                        action->city1 = city1;
                        action->city2 = city2;
                        action->color = color1;
                        action->joker = 0;
                        action->move = 1;
                        return action;
                    }
                    if((int)info->cards[color2] >= length){ // then 2
                        action->city1 = city1;
                        action->city2 = city2;
                        action->color = color2;
                        action->joker = 0;
                        action->move = 1;
                        return action;
                    }
                }
            }
            // could not place the colored road without jokers : try to pick a card with the right color for the road
            // only if it is late enough in the game, else pick random
            if((info->nbwagons - info->nbcards <= 10 || info_opp->nbwagons < 15) && info->nbcards <= 48){
                // no color 2
                if(color2 == 9 || color2 == 0){
                    for(int i=0; i<5; i++){
                        if((int)bord->cards_pickable->card[i] == color1){
                            action->city1 = -1;
                            action->city2 = -1;
                            action->color = color1;
                            action->joker = 0;
                            action->move = 3;
                            return action;
                        }
                    }
                }
                // need more of color1 (check 1 first then 2)
                else if((int)toplace[pos_toplace]->col[color1] - (int)info->cards[color1] >= (int)toplace[pos_toplace]->col[color2] - (int)info->cards[color2]){
                    for(int i=0; i<5; i++){
                        if((int)bord->cards_pickable->card[i] == color1){
                            action->city1 = -1;
                            action->city2 = -1;
                            action->color = color1;
                            action->joker = 0;
                            action->move = 3;
                            return action;
                        }
                    }
                    for(int i=0; i<5; i++){
                        if((int)bord->cards_pickable->card[i] == color2){
                            action->city1 = -1;
                            action->city2 = -1;
                            action->color = color2;
                            action->joker = 0;
                            action->move = 3;
                            return action;
                        }
                    }
                }
                // else check 2 first
                if((int)toplace[pos_toplace]->col[color1] - (int)info->cards[color1] < (int)toplace[pos_toplace]->col[color2] - (int)info->cards[color2]){
                    for(int i=0; i<5; i++){
                        if((int)bord->cards_pickable->card[i] == color2){
                            action->city1 = -1;
                            action->city2 = -1;
                            action->color = color2;
                            action->joker = 0;
                            action->move = 3;
                            return action;
                        }
                    }
                    for(int i=0; i<5; i++){
                        if((int)bord->cards_pickable->card[i] == color1){
                            action->city1 = -1;
                            action->city2 = -1;
                            action->color = color1;
                            action->joker = 0;
                            action->move = 3;
                            return action;
                        }
                    }
                }
            }
            else if(info->nbcards <= 48){ // it is not late enough in the game : we prefer taking random cards for the hope of some jokers
                action->city1 = -1;
                action->city2 = -1;
                action->color = -1;
                action->joker = 0;
                action->move = 2;
                return action;
            }
        }

        // // // // //

        // if the road does not have a set color
        if(color1 == 9){
            // try to place it (without using a joker)
            if(!pick && info->nbwagons >= length){
                // find the maximum surplus (= what we need - what we have)
                max_surplus = -1;
                for(int i=1; i<9; i++){
                    if((int)info->cards[i] - (int)toplace[pos_toplace]->col[i] >= length && (int)info->cards[i] - (int)toplace[pos_toplace]->col[i] > max_surplus){ // enough surplus
                        max_surplus = (int)info->cards[i] - (int)toplace[pos_toplace]->col[i];
                        action->city1 = city1;
                        action->city2 = city2;
                        action->color = i;
                        action->joker = 0;
                        action->move = 1;
                    }
                }
                if(max_surplus > -1) return action; // found a way to place
            }
            // else pick random (to change ?)
            if(!bord->gofast){
                action->city1 = -1;
                action->city2 = -1;
                action->color = -1;
                action->joker = 0;
                action->move = 2;
                return action;
            }
        }

        // // // // //
        // could not place the road or pick a satisfying card : try to place with some jokers

        // try to place with a joker : if has a color, enough wagons remaining, and more cards than wagons (to not waste the jokers early)
        if(color1 != 9 && !pick && info->nbwagons >= length && info->nbcards >= info->nbwagons){
            if(color2 == 9 || color2 == 0){ // no second color
                if((int)info->cards[color1] + (int)info->cards[9] >= length){
                    action->city1 = city1;
                    action->city2 = city2;
                    action->color = color1;
                    action->joker = length - (int)info->cards[color1];
                    action->move = 1;
                    return action;
                }
            }
            else{ // there is a second color
                // need more of color 1 : use jokers on color2
                if((int)toplace[pos_toplace]->col[color1] - (int)info->cards[color1] >= (int)toplace[pos_toplace]->col[color2] - (int)info->cards[color2]){
                    // 2 first then 1
                    if((int)info->cards[color2] + (int)info->cards[9] >= length){
                        action->city1 = city1;
                        action->city2 = city2;
                        action->color = color2;
                        action->joker = length - (int)info->cards[color2];
                        action->move = 1;
                        return action;
                    }
                    if((int)info->cards[color1] + (int)info->cards[9] >= length){
                        action->city1 = city1;
                        action->city2 = city2;
                        action->color = color1;
                        action->joker = length - (int)info->cards[color1];
                        action->move = 1;
                        return action;
                    }
                }
                else{ // 1 then 2
                    if((int)info->cards[color1] + (int)info->cards[9] >= length){
                        action->city1 = city1;
                        action->city2 = city2;
                        action->color = color1;
                        action->joker = length - (int)info->cards[color1];
                        action->move = 1;
                        return action;
                    }
                    if((int)info->cards[color2] + (int)info->cards[9] >= length){
                        action->city1 = city1;
                        action->city2 = city2;
                        action->color = color2;
                        action->joker = length - (int)info->cards[color2];
                        action->move = 1;
                        return action;
                    }
                }
            }
        }

    priority++;
    pos_priority = find_priority(bord, toplace[pos_toplace], priority); //position of current priority
    } // end of the loop for all roads in the path

    // // // // //

    if(bord->gofast){
        // place as fast as possible, else pick a card to place as fast as possible
        if(!pick){
            for(int j=info->nbwagons; j>=0; j--){
                for(int k=0; k<bord->gamedata->nbCities; k++){
                    for(int l=0; l<bord->gamedata->nbCities; l++){
                        city1 = k;
                        city2 = l;
                        color1 = bord->MatRoute[city1][city2].color;
                        color2 = bord->MatRoute[city1][city2].color2;

                        if(bord->MatRoute[city1][city2].length == j){
                            if(color1 != 9 && bord->MatRoute[city1][city2].taken < 0 && info->cards[color1] >= bord->MatRoute[city1][city2].length){ // enough color 1 to place
                                action->city1 = city1;
                                action->city2 = city2;
                                action->color = color1;
                                action->joker = 0;
                                action->move = 1;
                                return action;
                            }
                            else if(color2 != 9 && color2 != 0 && bord->MatRoute[city1][city2].taken < 0 && info->cards[color2] >= bord->MatRoute[city1][city2].length){ // enough color2 to place
                                action->city1 = city1;
                                action->city2 = city2;
                                action->color = color2;
                                action->joker = 0;
                                action->move = 1;
                                return action;
                            }
                            else if(color1 == 9){
                                for(int k=1; k<9; k++){
                                    if(bord->MatRoute[city1][city2].taken < 0 && info->cards[k] >= bord->MatRoute[city1][city2].length){
                                        action->city1 = city1;
                                        action->city2 = city2;
                                        action->color = k;
                                        action->joker = 0;
                                        action->move = 1;
                                        return action;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        // if we can't place, search a color to pick : take the color of which we have the most. (if not available the second we have the most etc)
        // we hope to place a road with no set color
        int coolor = find_next_max_color(info->cards, 999);
        while(coolor != -1){
            for(int k=0; k<5; k++){
                if(bord->cards_pickable->card[k] == coolor && coolor != 9){
                    action->city1 = -1;
                    action->city2 = -1;
                    action->color = coolor;
                    action->joker = 0;
                    action->move = 3;
                    return action;
                }
            }
            coolor = find_next_max_color(info->cards, info->cards[coolor]);
        }
    }

    // last resort (almost): pick random
    if(info->nbcards <= 48 || (info->nbcards <= 49 && pick)){
        action->city1 = -1;
        action->city2 = -1;
        action->color = -1;
        action->joker = 0;
        action->move = 2;
        return action;
    }

    // very last resort : place a random road (we can't have more than 50 cards : this is the last thing we can do)
    for(int j=0; j<bord->gamedata->nbCities; j++){
        for(int k=0; k<bord->gamedata->nbCities; k++){
            city1 = j;
            city2 = k;
            color1 = bord->MatRoute[city1][city2].color;
            color2 = bord->MatRoute[city1][city2].color2;

            if(color1 != 9 && bord->MatRoute[city1][city2].taken < 0 && info->cards[color1] >= bord->MatRoute[city1][city2].length){ // enough color 1 to place
                action->city1 = city1;
                action->city2 = city2;
                action->color = color1;
                action->joker = 0;
                action->move = 1;
                return action;
            }
            else if(color2 != 9 && color2 != 0 && bord->MatRoute[city1][city2].taken < 0 && info->cards[color2] >= bord->MatRoute[city1][city2].length){ // enough color2 to place (and there is a color2)
                action->city1 = city1;
                action->city2 = city2;
                action->color = color2;
                action->joker = 0;
                action->move = 1;
                return action;
            }
            else if(color1 == 9){
                for(int k=1; k<9; k++){
                    if(bord->MatRoute[city1][city2].taken < 0 && info->cards[k] >= bord->MatRoute[city1][city2].length){
                        action->city1 = city1;
                        action->city2 = city2;
                        action->color = k;
                        action->joker = 0;
                        action->move = 1;
                        return action;
                    }
                }
            }
        }
    }
    for(int i=0; i<10; i++) printf("AAAAA\n");
    printf("NOT GOOD : GOT TO THE END OF SEARCH_COLOR_PICK V3");
    return NULL;
}



