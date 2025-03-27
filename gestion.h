#include "struct.h"

#ifndef gestion_h
#define gestion_h

/* alloue matrice de taille nbcity*nbcity */
route** allouer_matrice_route(int nbcity);

/* destroy matrice of size nbcity*X */
void destroy_matrice_route(route** mat,int nbcity);

/* print matrice sized nbcity*nbcity */
void print_matrice_route(route** mat, int nbcity);

/* converts data from the array to the matrix
and initialises distance as -1, taken as -1 */
void convert_tab_matrice(route** mat, int* tab, int nbrail, int nbcity);

/* updates the matrix to know wich route has been taken.
will need move_result to validate*/
void update_mat(route** mat, MoveData* movedata, int player);

/* alloue tableau de int de taille len*/
void* allouertab(int len);

/* affiche tableau de int taille len*/
void print_tab(int* tab, int len);

/* initialises cards array with starting cards*/
void init_tab_cards(CardColor* tab, GameData* gamedata);

void update_tab_cards(CardColor* tab, MoveResult* move_result);

/* détruit tableau*/
void destroy_tab(void* tab);

player_info* init_player_info();

void destroy_player_info(player_info* info);

/* you NEED to update the route matrix before calling this,
card array not operational,
choose objective seems not to be seeable for other players*/
void update_player_info(player_info* info, MoveData* movedata, MoveResult* moveresult, route** mat_route);

#endif