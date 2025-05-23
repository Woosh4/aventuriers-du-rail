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

void destroy_toplace(To_Place** toplace, Player_Info* info);

void destroy_place(To_Place* place);

/* updates priority in the toplace array for each road
currently: roads with a set color first, the the others*/
void update_priority(Board* bord, Player_Info* info, To_Place** toplace);

/* finds the index in toplace[max]->path of the road with the lowest priority*/
int find_min_priority(Board* bord, Player_Info* info, To_Place** toplace, int max);

//TODO ?

/* returns what color should be picked / placed
inputs : max, the index in toplace** ; pick: 1 if we are playing again after picking a card
1 2 3: color to be picked; -1 -2 -3: color ready to be placed
                         ; -11 -12 -13: color + some amount of joker to be placed (-color -10)
10 if no card available(pick random?), 11 if only joker, 12 if any color
skip road if any color can be used, then comes back later:

//1 assez de cartes pour poser, sinon
//2 cartes dispo à piocher, sinon (basculer pioche)
//3 assez de cartes avec les jokers, sinon          utiliser jokers plus tard?
//4 pioche pour la ville suivante (basculer pioche)
//5 pick random
 */
int search_color_pick(Board* bord, Player_Info* info, To_Place** toplace, int max, int pick);

/* returns how many jokers are needed to place the road in toplace[max]->path[road] of specific color.
color is to be between -11 and -18 for it work properly (convention used in search_color_pick)*/
int find_nb_joker(Board* bord, Player_Info* info, To_Place** toplace, int max, int road, int choice);

/* finds the next maximum ev and returns the associate position in the toplace array*/
int find_next_max_ev(To_Place** toplace, To_Place* current_ev);

/* updates the estimate length of each path, we assume the paths of lower priority have been taken following a dijkstra shortest path.*/
void update_To_place_len(To_Place** toplace, Board* bord, Player_Info* info);

/* returns the number of points for a route of length len*/
int points(int len);

/* calculate the expected value. obj_place is the position of the objective in the array*/
float ev_calculate(Player_Info* info, To_Place* place, int obj_place);

/* calculate ev from moveresult, to evaluate the objectives we just picked*/
float ev_calculate_result(Player_Info* info, To_Place* place, int obj_number);

/* calculate the EV based on the estimate length of the picked objectives*/
float ev_estimate_result(Player_Info* info, To_Place* place, int obj_number_result);

/* returns the max between the 2, if index==1 then return the index of the max*/
float max2(float val1, float val2, int index);

/* returns the max between the 3, if index==1 then return the index of the max*/
float max3(float val1, float val2, float val3, int index);

/* chooses new objectives depending on their estimate ev*/
void pick_new_objectives(To_Place** toplace, Player_Info* info, Board* bord);

#endif