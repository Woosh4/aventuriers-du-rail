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
            printf("(From:%d To:%d length:%d taken:%d)  ", i, j, (mat[i][j]).length, (mat[i][j]).taken);
            }
            printf("]\n");
        }
    return;

}


/* converts data from the array to the matrix
from to length color1 color2 */
void convert_tab_matrice(route** mat, int* tab, int nbrail){
    for(int i=0;i<nbrail*5;i=i+5){
        mat[tab[i]][tab[i+1]].length = tab[i+2];
        mat[tab[i]][tab[i+1]].color = tab[i+3];
        mat[tab[i]][tab[i+1]].color2 = tab[i+4];

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
