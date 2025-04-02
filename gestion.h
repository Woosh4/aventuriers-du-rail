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
void convert_tab_matrice(Board* bord);

/* updates the matrix to know wich route has been taken.
will need move_result to validate*/
void update_mat(Board* bord, Player_Info* info);

/* alloue tableau de int de taille len*/
void* allouertab(int len);

/* affiche tableau de int taille len*/
void print_tab(int* tab, int len);

/* initialises cards array with starting cards*/
void init_tab_cards(Player_Info* info, Board* bord);

void update_tab_cards(CardColor* tab, MoveResult* move_result);

/* détruit tableau*/
void destroy_tab(void* tab);

Player_Info* init_player_info(int playernumber);

void destroy_player_info(Player_Info* info);

/* you NEED to update the route matrix before calling this,
updates all fields except movedata, movedata needs to be the correct one for this turn
will need updating for jokers */
void update_player_info(Player_Info* info, Board* bord);

/* alloc board, to be called at the very beginning*/
Board* alloc_board();

/* init board, to call after alloc_board and sendgamesettings.*/
void init_board(Board* bord);

void destroy_board(Board* bord);

/* updates pickable cards and mat route*/
void update_board(Board* bord, Player_Info* info);

#endif