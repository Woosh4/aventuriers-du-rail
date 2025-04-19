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

/* updates pickable cards, and mat route using the move data in info.
update 'when' with a function ?*/
void update_board(Board* bord, Player_Info* info);

/* returns the index of the route with the lowest weight for a given line*/
int find_min(Dijkstra_City* dijk, int nbcity);

/* checks if the array is filled with only 0s*/
int all_checked(Dijkstra_City* array, Board* bord);

/* updates the distance of city2 if the route through city1 is shorter*/
void update_weight(Dijkstra_City* dijk, int city1, int city2, Board* bord);

/* returns the shortest path between city1 and city2*/
To_Place* shortest(Board* bord, int city1, int city2);

void print_toplace(To_Place** toplace);

/* create an array of toplace and fills it using dijkstra, also fills EV*/
To_Place** To_place_create(Board* bord, Player_Info* info);

/*finds maximum EV in the toplace array and returns its position, returns -1 if error*/
int find_max_ev(To_Place** toplace);

void destroy_toplace(To_Place** toplace);

//TODO

/*returns the index of the road in toplace[maxEV]->path we want to place next (with num=0),
if num=1 gives the second road instead, returns 0 if no route with a set color available*/
int search_index(Board* bord, Player_Info* info, To_Place** toplace, int max, int num);

/* returns what color should be picked / placed
inputs : max, the index in toplace**; index, the index of the road in toplace[max] we want to place
1 2 3: color to be picked; -1 -2 -3: color ready to be placed
10 if no card available(pick random?), 11 if only joker, 12 if any color
skip road if any color can be used, then comes back later: */
int search_color_pick(Board* bord, Player_Info* info, To_Place** toplace, int max, int index, int index2);
#endif