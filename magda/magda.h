#ifndef MAGDA
#define MAGDA

#include "../common.h"

#define max(x, y) ((x) > (y) ? (x) : (y))
#define min(x, y) ((x) < (y) ? (x) : (y))

// minimax_res minimax(ui8, ui8);
int heuristic_value_of_node(void);
void minimax_move_player(player *, matrix_point);
void minimax_unmove_player(player *, matrix_point);
// void minimax_place_wall(ui8, active_player);
// void minimax_remove_wall(ui8, active_player);
void minimax_place_wall(wall);
void minimax_unplace_wall(wall);
void minimax_swap_turn(void);
ui8 minimax_game_over(void);
int MIN(ui8);
int MAX(ui8);
move minimax(ui8);
int state_eval(void);
#endif